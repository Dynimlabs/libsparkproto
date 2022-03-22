// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/imagestreamprotocolimpl.h>
#include <libsparkproto/exception.h>
#include <libsparkproto/log.h>
#include <libsparkproto/imageset.h>

namespace libspark {

namespace protocol {

ImageStreamProtocolImpl::ImageStreamProtocolImpl(const std::string &address, const std::string &service)
    : _sock(-1), _address(address), _service(service) {

    _streamRequest.set_streamtype(STREAM_LEFT);
    _streamRequest.set_imgformat(ImageFormat::FORMAT_RGB);
}

ImageStreamProtocolImpl::~ImageStreamProtocolImpl() {
    network::closeConnection(_sock);
}

void ImageStreamProtocolImpl::start() {

    // just allow a connection
    if(_sock > 0) {
        throw SparkError("A connection existed, it need to stop before restarting again");
    }
    // open a connection to device
    _sock = network::connectTcpSocket(_address, _service);

    StreamRequest requestMsg;
    StreamResponse responseMsg;
    StreamStartRequest *startMsg = new StreamStartRequest(_streamRequest);
    requestMsg.set_allocated_requeststart(startMsg);

    callStreamRequest(requestMsg, responseMsg);

    if(responseMsg.code() != StreamResponse::RESPONSE_OK) {
        throw SparkError("failure starting stream, detail message: " + std::string(responseMsg.message()));
    }
    LOG_INFO("imagestream is starting with streamid: %d", responseMsg.streamid());
}

void ImageStreamProtocolImpl::stop() {
    // Close socket to stop stream for reasons:
    // 1. since image payload size is very large, so it's difficult to send a new request to stop stream in a half-duplex connection.
    // we should upgrade to fullduplex protocol as websocket.
    // 2. Also, stream can not be stopped immediately after a stop request sent since image payload can be delay.

    network::closeConnection(_sock);
}

void ImageStreamProtocolImpl::recvImageSet(ImageSet &imgSet, int timeout) {
    
    (void) timeout;
    
    // receive 4 bytes for header, is size of response message
    int resSize;
    u_char hBuff[4];
    network::recvFixedFrom(_sock, hBuff, 4);
    memcpy(&resSize, hBuff, 4);
    if(resSize <= 0) {
        throw SparkError("payload size is zero");
    }

    // receive ImageSetMeta
    std::unique_ptr<char[]> pResBuff = std::make_unique<char[]>(resSize);
    network::recvFixedFrom(_sock, pResBuff.get(), resSize);

    std::unique_ptr<ImageSetMeta> meta = std::make_unique<ImageSetMeta>();
    int ret = meta->ParseFromArray(pResBuff.get(), resSize);
    if(!ret) {
        throw SparkError("failed parse ImageSetMeta from socket");
    }


    // receive buffers
    if(meta->has_left()){
        ImageSet::Buffer &buff = imgSet.getMutableBuffer(ImageSet::BUFFER_LEFT);
        buff.resize(meta->left().buffsize());
        network::recvFixedFrom(_sock, buff.data(), buff.size());
    }

    if(meta->has_right()){
        ImageSet::Buffer &buff = imgSet.getMutableBuffer(ImageSet::BUFFER_RIGHT);
        buff.resize(meta->right().buffsize());
        network::recvFixedFrom(_sock, buff.data(), buff.size());
    }

    if(meta->has_depth()){
        ImageSet::Buffer &buff = imgSet.getMutableBuffer(ImageSet::BUFFER_DEPTH);
        buff.resize(meta->depth().buffsize());
        network::recvFixedFrom(_sock, buff.data(), buff.size());
    }

    if(meta->has_disparity()){
        ImageSet::Buffer &buff = imgSet.getMutableBuffer(ImageSet::BUFFER_DISPARITY);
        buff.resize(meta->disparity().buffsize());
        network::recvFixedFrom(_sock, buff.data(), buff.size());
    }
    
    imgSet.setAllocatedMeta(std::move(meta));
}

void ImageStreamProtocolImpl::setStreamType(int32_t streamType) {
    if(streamType < 1)
        throw SparkError("passed a invalid StreamType, see more libspark::protocol::StreamType");

    _streamRequest.set_streamtype(streamType);
}

void ImageStreamProtocolImpl::setImageFormat(int32_t imgFormat) {
    if(!ImageFormat_IsValid(imgFormat))
        throw SparkError("passed a invalid ImageFormat, see more libspark::protocol::ImageFormat");

    _streamRequest.set_imgformat(ImageFormat(imgFormat));
}

template<typename TRequest, typename TResponse>
void ImageStreamProtocolImpl::callStreamRequest(const TRequest& requestMsg, TResponse &responseMsg) {
    // serialize requestMsg to buff to send to socket
    int32_t requestSize = requestMsg.ByteSize();
    int32_t requestFullSize = requestSize + 4;
    std::unique_ptr<char[]> reqBuff = std::make_unique<char[]>(requestFullSize);
    memcpy(reqBuff.get(), &requestSize, 4);
    requestMsg.SerializeToArray(reqBuff.get() + 4, requestSize);

    // send msg to socket
    network::sendFixedTo(_sock, reqBuff.get(), requestFullSize);

    // receive 4 bytes for header, is size of response message
    int resSize;
    u_char hBuff[4];
    network::recvFixedFrom(_sock, hBuff, 4);
    memcpy(&resSize, hBuff, 4);
    if(resSize <= 0) {
        throw SparkError("nothing replied from device");
    }

    // receive sequence of size bytes in header
    std::unique_ptr<char[]> pResBuff = std::make_unique<char[]>(resSize);
    network::recvFixedFrom(_sock, pResBuff.get(), resSize);

    // parse response
    bool ret = responseMsg.ParseFromArray(pResBuff.get(), resSize);
    if(!ret) {
        responseMsg.PrintDebugString();
        throw SparkError("failure when parsing response message from buffer");
    }
}

} // namespace protocol
} // namespace libspark
