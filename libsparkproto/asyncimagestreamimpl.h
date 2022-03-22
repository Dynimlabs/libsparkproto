// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <libsparkproto/image.pb.h>
#include <libsparkproto/imagestreamprotocol.h>

namespace libspark
{
namespace protocol
{

class IImageEvent;
class AsyncImageStreamImpl
{
public:
    AsyncImageStreamImpl(std::shared_ptr<DeviceInfo> pDevice);

    virtual ~AsyncImageStreamImpl();

    /**
     * @brief Start stream ImageSet from spark,
     * default just left image is streamed if client don't call setStreamType function
     * default ImageFormat is FORMAT_MONO_8_BIT if client don't call setImageFormat function.
     * If stream can not start, a exception is thrown, see more exception libspark::protocol::SparkException
     */
    void start();

    /**
     * @brief Stop stream
     * 
     */
    void stop();

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
    
    /**
     * @brief 
     * 
     * @param ImgEvent 
     */
    void registerEvent(const std::shared_ptr<IImageEvent> &imgEvent);

    void unregisterEvent(const std::shared_ptr<IImageEvent> &imgEvent);

private:
    std::unique_ptr<ImageStreamProtocol> _pImgStream;
    std::shared_ptr<IImageEvent> _pImageEvent;

    std::thread _streamThreading;
    std::mutex _streamLock;
    std::mutex _streamStopLock;
    bool _isStreaming;
};

}
}
