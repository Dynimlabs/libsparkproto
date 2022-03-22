// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/asyncimagestreamimpl.h>
#include <libsparkproto/imagestreamprotocol.h>
#include <libsparkproto/iimageevent.h>

namespace libspark {

namespace protocol {

AsyncImageStreamImpl::AsyncImageStreamImpl(std::shared_ptr<DeviceInfo> pDevice) 
    : _pImgStream(new ImageStreamProtocol(pDevice)){

}

AsyncImageStreamImpl::~AsyncImageStreamImpl() {

    // call and wait until stream stop if streaming is going
    _streamLock.lock();
    if(_isStreaming) {
        _streamLock.unlock();

        stop();
    }
    else {
        _streamLock.unlock();
    }
}

void AsyncImageStreamImpl::start() {
    _pImgStream->start();

    _streamThreading = std::thread([&](){
        _streamLock.lock();
        _isStreaming = true;
        _streamLock.unlock();

        while(1) {
            _streamLock.lock();
            if(!_isStreaming) {
                _streamLock.unlock();
                break;
            }
            _streamLock.unlock();

            // receive Image
            ImageSet imgSet;
            _pImgStream->recvImageSet(imgSet);
            if(_pImageEvent) {
                _pImageEvent->onImageEvent(imgSet);
            }
        };
    });
}

void AsyncImageStreamImpl::stop() {
    _streamLock.lock();
    _isStreaming = false;
    _streamLock.unlock();

    if(_streamThreading.joinable())
        _streamThreading.join();

    // send request to stop stream
    _pImgStream->stop();
}

void AsyncImageStreamImpl::setStreamType(int32_t streamType) {
    _pImgStream->setStreamType(streamType);
}

void AsyncImageStreamImpl::setImageFormat(int32_t imgFormat) {
    _pImgStream->setImageFormat(imgFormat);
}

void AsyncImageStreamImpl::registerEvent(const std::shared_ptr<IImageEvent> &imgEvent) {
    _pImageEvent = imgEvent;
}

void AsyncImageStreamImpl::unregisterEvent(const std::shared_ptr<IImageEvent> &imgEvent) {
    (void)imgEvent;
    _pImageEvent.reset();
}

} // namespace protocol
} // namespace libspark
