// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <libsparkproto/common.h>
#include <libsparkproto/deviceinfo.h>

namespace libspark {

namespace protocol {

class ImageStreamProtocolImpl;
class ImageSet;
class SPARK_API ImageStreamProtocol {

public:

    using Ptr = std::unique_ptr<ImageStreamProtocol>;
    
    /**
     * @brief Construct a new ImageStreamProtocol object
     * 
     * @param device 
     */
    ImageStreamProtocol(std::shared_ptr<DeviceInfo> device);

    /**
     * @brief Destroy the ImageStreamProtocol object
     * 
     */
    virtual ~ImageStreamProtocol();

    /**
     * @brief Start stream ImageSet from spark,
     * default just left image is streamed if client don't call setStreamType function
     * default ImageFormat is FORMAT_RGB if client don't call setImageFormat function.
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
     * @brief Set the StreamType. 
     * See list of stream type at libspark::protocol::StreamType.
     * StreamType can be passed as a bitwise value, for example: setStreamType(STREAM_LEFT|STREAM_RIGHT|STREAM_DISPARITY);
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
    std::unique_ptr<ImageStreamProtocolImpl> _pImpl;
};

} // namespace protocol
} // namespace libspark
