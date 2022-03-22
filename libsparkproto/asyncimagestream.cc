// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/asyncimagestream.h>
#include <libsparkproto/asyncimagestreamimpl.h>

namespace libspark {

namespace protocol {

AsyncImageStream::AsyncImageStream(std::shared_ptr<DeviceInfo> pDevice)
    :_pImpl(new AsyncImageStreamImpl(pDevice)) {

}

AsyncImageStream::~AsyncImageStream() {

}

void AsyncImageStream::start() {
    _pImpl->start();
}

void AsyncImageStream::stop() {
    _pImpl->stop();
}

void AsyncImageStream::setStreamType(int32_t streamType) {
    _pImpl->setStreamType(streamType);
}

void AsyncImageStream::setImageFormat(int32_t imgFormat) {
    _pImpl->setImageFormat(imgFormat);
}

void AsyncImageStream::registerEvent(const std::shared_ptr<IImageEvent> &imgEvent) {
    _pImpl->registerEvent(imgEvent);
}

void AsyncImageStream::unregisterEvent(const std::shared_ptr<IImageEvent> &imgEvent) {
    _pImpl->unregisterEvent(imgEvent);
}

} // namespace protocol
} // namespace libspark
