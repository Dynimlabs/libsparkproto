// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <iostream>
#include <libsparkproto/imageset.h>

namespace libspark {

namespace protocol {

ImageSet::ImageSet() : _meta(new ImageSetMeta()){
    _bufferSet.resize((uint32_t)ImageSet::BufferID::BUFFER_ID_MAX + 1);
}

ImageSet::ImageSet(std::unique_ptr<ImageSetMeta> meta) : _meta(std::move(meta)){
    _bufferSet.resize((uint32_t)ImageSet::BufferID::BUFFER_ID_MAX + 1);
}

ImageSet::ImageSet(const ImageSet& rhs) {
    _bufferSet = rhs._bufferSet;
    _meta = std::make_unique<ImageSetMeta>();
    *_meta.get() = *rhs._meta.get();
}

ImageSet::ImageSet(ImageSet&& rhs) {
    _bufferSet = std::move(rhs._bufferSet);
    _meta = std::move(rhs._meta);
}

ImageSet::ImageSet(const ImageSet *rhs) {
    _bufferSet = rhs->_bufferSet;
    _meta = std::make_unique<ImageSetMeta>();
    *_meta.get() = *rhs->_meta.get();
}

const ImageSet& ImageSet::operator=(const ImageSet& rhs) {
    _bufferSet = rhs._bufferSet;
    _meta = std::make_unique<ImageSetMeta>();
    *_meta.get() = *rhs._meta.get();

    return *this;
}

ImageSet::~ImageSet() {

}

const ImageSet::Buffer& ImageSet::getBuffer(BufferID id) const {
    if(_bufferSet.size() <= (uint32_t)id){
        return _nullBuffer;
    }

    return _bufferSet[(uint32_t)id];
}

ImageSet::Buffer& ImageSet::getMutableBuffer(BufferID id) {
    if(_bufferSet.size() <= (uint32_t)id){
        return _nullBuffer;
    }

    return _bufferSet[(uint32_t)id];
}

ImageSet::Buffer ImageSet::getMovedBuffer(BufferID id) {
    if(_bufferSet.size() <= (uint32_t)id){
        return _nullBuffer;
    }

    return std::move(_bufferSet[(uint32_t)id]);
}

uint32_t ImageSet::imageWidth() const {
    if(hasLeft()){
        return meta().left().width();
    }
    if(hasRight()){
        return meta().right().width();
    }
    return 0; // no image
}

uint32_t ImageSet::imageHeight() const {
    if(hasLeft()){
        return meta().left().height();
    }
    if(hasRight()){
        return meta().right().height();
    }
    return 0; // no image
}

ImageFormat ImageSet::imageFormat() const {
    if(hasLeft()){
        return meta().left().format();
    }
    if(hasRight()){
        return meta().right().format();
    }
    return ImageFormat::FORMAT_UNKNOWN; // no image
}

uint64_t ImageSet::imageTimestamp() const {
    if(hasLeft()){
        return meta().left().timestamp();
    }
    if(hasRight()){
        return meta().right().timestamp();
    }
    return 0; // no image
}

void ImageSet::setAllocatedMeta(std::unique_ptr<ImageSetMeta> meta) {
    _meta.swap(meta);
}

void ImageSet::printDebugString() const {
    _meta->PrintDebugString();
}

} // namespace protocol
} // namespace libspark
