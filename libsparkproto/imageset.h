// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <vector>
#include <libsparkproto/common.h>
#include <libsparkproto/image.pb.h>

namespace libspark {

namespace protocol {

class SPARK_API ImageSet {

public:
    using Buffer = std::vector<u_char>;
    
    enum BufferID {
        BUFFER_LEFT = 0,
        BUFFER_RIGHT = 1,
        BUFFER_DEPTH = 2,
        BUFFER_DISPARITY = 3,
        BUFFER_ID_MAX = BUFFER_DISPARITY
    };

    /**
     * @brief Construct a new ImageSet object
     * 
     */
    ImageSet();

    /**
     * @brief Construct a new ImageSet object
     * 
     * @param meta 
     */
    explicit ImageSet(std::unique_ptr<ImageSetMeta> meta);

    /**
     * @brief Copy constructor 
     * 
     * @param rhs 
     */
    ImageSet(const ImageSet& rhs);

    /**
     * @brief Move constructor
     * 
     * @param rhs 
     */
    ImageSet(ImageSet&& rhs);

    /**
     * @brief Copy constructor from a pointer to ImageSet instance
     * 
     * @param rhs 
     */
    ImageSet(const ImageSet *rhs);

    /**
     * @brief Assign operator
     * 
     * @param rhs 
     * @return const ImageSet& 
     */
    const ImageSet& operator=(const ImageSet& rhs);

    /**
     * @brief Destroy the ImageSet object
     * 
     */
    ~ImageSet();

    /**
     * @brief Check if left image exist
     * 
     * @return true 
     * @return false 
     */
    bool hasLeft() const;

    /**
     * @brief Check if right image exist
     * 
     * @return true 
     * @return false 
     */
    bool hasRight() const;

    /**
     * @brief Check if depthmap exist
     * 
     * @return true 
     * @return false 
     */
    bool hasDepth() const;

    /**
     * @brief Check if disparity image exist
     * 
     * @return true 
     * @return false 
     */
    bool hasDisparity() const;

    /**
     * @brief Return readonly buffer of an image with id
     * 
     * @param id 
     * @return const Buffer& 
     */
    const Buffer& getBuffer(BufferID id) const;

    /**
     * @brief Return modifiable buffer of an image with id
     * 
     * @param id 
     * @return Buffer& 
     */
    Buffer& getMutableBuffer(BufferID id);
    
    /**
     * @brief Returns a releaseable buffer of an image with id. User will handle buffer after the function is called.
     * 
     * @param id 
     * @return Buffer 
     */
    Buffer getMovedBuffer(BufferID id);

    /**
     * @brief Get width of image, it's same for both left and right image when they are streamed at same time.
     * 
     * @return uint32_t 
     */
    uint32_t imageWidth() const ;

    /**
     * @brief Get Height of image, it's same for both left and right image when they are streamed at same time.
     * 
     * @return uint32_t 
     */
    uint32_t imageHeight() const ;

    /**
     * @brief Get imageformat of image, it's same for both left and right image when they are streamed at same time.
     * 
     * @return ImageFormat 
     */
    ImageFormat imageFormat() const;

    /**
     * @brief Get timestamp of captured imageset. It is timestamp of left image if it's captured
     * 
     * @return uint64_t 
     */
    uint64_t imageTimestamp() const;

    /**
     * @brief Metadata of ImageSet. Can get full information of image in meta object. See more ImageSetMeta
     * 
     * @return const ImageSetMeta& 
     */
    const ImageSetMeta& meta() const;

    /**
     * @brief Internal using
     * 
     * @param meta 
     */
    void setAllocatedMeta(std::unique_ptr<ImageSetMeta> meta);

    /**
     * @brief Print metadata of ImageSet to standard output
     * 
     */
    void printDebugString() const;

private:
    std::unique_ptr<ImageSetMeta> _meta;
    std::vector<Buffer> _bufferSet;
    Buffer _nullBuffer;
};

inline bool ImageSet::hasLeft() const {
    return _meta->has_left();
}

inline bool ImageSet::hasRight() const {
    return _meta->has_right();
}

inline bool ImageSet::hasDepth() const {
    return _meta->has_depth();
}

inline bool ImageSet::hasDisparity() const {
    return _meta->has_disparity();
}

inline const ImageSetMeta& ImageSet::meta() const {
    return *_meta.get();
}

} // namespace protocol
} // namespace libspark
