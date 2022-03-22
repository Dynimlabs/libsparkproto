// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <memory>
#include <libsparkproto/common.h>
#include <libsparkproto/deviceinfo.h>
#include <libsparkproto/image.pb.h>

namespace libspark
{
namespace protocol
{
class AsyncImageStreamImpl;
class IImageEvent;
class SPARK_API AsyncImageStream
{
public:

    using Ptr = std::unique_ptr<AsyncImageStream>;
    
    /**
     * @brief Construct a new AsyncImageStream object
     * 
     * @param pDevice 
     */
    AsyncImageStream(std::shared_ptr<DeviceInfo> pDevice);

    /**
     * @brief Destroy the AsyncImageStream object
     * 
     */
    virtual ~AsyncImageStream();

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

    /**
     * @brief register a ImageEvent overided from IImageEvent interface
     * ImageEvent will called when ImageSet is ready
     * 
     * @param ImgEvent 
     */
    void registerEvent(const std::shared_ptr<IImageEvent> &imgEvent);

    /**
     * @brief remove the ImageEvent
     * 
     * @param imgEvent 
     */
    void unregisterEvent(const std::shared_ptr<IImageEvent> &imgEvent);

private:
    std::unique_ptr<AsyncImageStreamImpl> _pImpl;
};

}
}
