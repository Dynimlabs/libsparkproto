// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <libsparkproto/network.h>
#include <libsparkproto/image.pb.h>

namespace libspark {

namespace protocol {

class ImageSet;
class ImageStreamProtocolImpl {

public:
    /**
     * @brief Construct a new ImageStreamProtocolImpl object
     * 
     * @param device 
     */
    ImageStreamProtocolImpl(const std::string &address, const std::string &service);

    /**
     * @brief Destroy the ImageStreamProtocolImpl object
     * 
     */
    virtual ~ImageStreamProtocolImpl();

    /**
     * @brief Start stream ImageSet from spark,
     * default just left image is streamed if client don't call setStreamType function
     * default ImageFormat is FORMAT_MONO_8_BIT if client don't call setImageFormat function
     * If stream can not start, a exception is thrown, see more exception libspark::protocol::SparkException
     */
    void start();

    /**
     * @brief Stop stream
     * 
     */
    void stop();

    /**
     * @brief Receive ImageSet from spark with timeout
     * when timeout is not set or -1, function will block until ImageSet received
     * 
     * @param imgSet 
     * @param timeout
     * see more libspark::protocol::ImageSet
     */
    void recvImageSet(ImageSet &imgSet, int timeout=-1);

    /**
     * @brief Set the StreamType
     * 
     * @param streamType 
     */
    void setStreamType(int32_t streamType);

    /**
     * @brief Set the ImageFormat
     * 
     * @param imgFormat 
     */
    void setImageFormat(int32_t imgFormat);

private:
    template<typename TRequest, typename TResponse>
    void callStreamRequest(const TRequest& requestMsg, TResponse &responseMsg);

    // streamsocket
    SOCKET _sock;
    std::string _address;
    std::string _service;

    StreamStartRequest _streamRequest;
};

} // namespace protocol
} // namespace libspark
