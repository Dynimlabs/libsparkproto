// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/imagestreamprotocol.h>
#include <libsparkproto/imagestreamprotocolimpl.h>
#include <libsparkproto/constants.h>
#include <libsparkproto/exception.h>

namespace libspark {

namespace protocol {

ImageStreamProtocol::ImageStreamProtocol(std::shared_ptr<DeviceInfo> pDevice) {
    if(!pDevice->isCompatible())
        throw SparkError("The library is not compatible with Spark firmware, please upgrade new version of libsparkpro");

    _pImpl = std::make_unique<ImageStreamProtocolImpl>(pDevice->getIpAdress(), std::to_string(IMAGEDATA_PORT));
}

ImageStreamProtocol::~ImageStreamProtocol() {

}

void ImageStreamProtocol::start() {
    _pImpl->start();
}

void ImageStreamProtocol::stop() {
    _pImpl->stop();
}

void ImageStreamProtocol::recvImageSet(ImageSet &imgSet, int timeout) {
    _pImpl->recvImageSet(imgSet, timeout);
}

void ImageStreamProtocol::setStreamType(int32_t streamType) {
    _pImpl->setStreamType(streamType);
}

void ImageStreamProtocol::setImageFormat(int32_t imgFormat) {
    _pImpl->setImageFormat(imgFormat);
}

} // namespace protocol
} // namespace libspark
