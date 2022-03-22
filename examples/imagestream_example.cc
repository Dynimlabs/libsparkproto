// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


/**
 * Example of streaming images synchronously
 * 
 */

#include <iostream>
#include <libsparkproto/sparkproto.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace libspark::protocol;

int main() {
    using std::chrono::high_resolution_clock;
    // search for available device in the network
    std::unique_ptr<DeviceEnumeration> deviceEnum = std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();

    if(deviceList.empty()) {
        std::cout<<"No device was found"<<std::endl;
        return -1;
    }

    // Create a stream of images from both camera in RGB format
    ImageStream::Ptr imgStream = std::make_unique<ImageStream>(deviceList[0]);
    imgStream->setStreamType(StreamType::STREAM_LEFT | StreamType::STREAM_RIGHT);
    imgStream->setImageFormat(ImageFormat::FORMAT_RGB);

    try {
        imgStream->start();
    } catch (SparkException & e) {
        std::cout << "Failed to start the stream. Error: " << e.what() << std::endl;
        return 1;
    }

    // measure the capture time
    auto startTime = high_resolution_clock::now();
    // capture 100 images from the stream
    int nFrame = 100;
    for(int i = 0; i < nFrame; i++){
        ImageSet imgSet;
        imgStream->recvImageSet(imgSet);

        imgSet.printDebugString();
    }

    auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                        high_resolution_clock::now() - startTime);
    imgStream->stop();

    std::cout.precision(3);
    std::cout << "Avg. FPS: " << nFrame / (durationMs.count() / 1000.0) << std::endl;

    return 0;
}
