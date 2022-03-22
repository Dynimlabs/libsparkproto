// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


/**
 * Example of streaming images asynchronously
 * 
 */

#include <iostream>
#include <thread>
#include <chrono>

#include <libsparkproto/sparkproto.h>

using namespace std::chrono_literals;
using namespace libspark::protocol;

// use this interface with method `onImageEvent` to implement a callback method
// for image handling
class ImageEvent: public IImageEvent {

public:
    ImageEvent() = default;
    ~ImageEvent() = default;

    void onImageEvent(ImageSet &imgSet) override
    {
        imgSet.printDebugString();
    }
};

int main() {
    std::unique_ptr<DeviceEnumeration> deviceEnum = std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();

    if(deviceList.empty()) {
        std::cout<<"No device was found"<<std::endl;
        return -1;
    }

    // create default stream with just left image, with FORMAT_RGB format
    AsyncImageStream::Ptr imgStream = std::make_unique<AsyncImageStream>(deviceList[0]);
    std::shared_ptr<ImageEvent> imgEvent = std::make_shared<ImageEvent>();

    imgStream->registerEvent(imgEvent);
    imgStream->start();
    // Do something during streaming
    std::this_thread::sleep_for(30s); // wait 30s to stop stream
    imgStream->stop();
    imgStream->unregisterEvent(imgEvent);
}
