// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


/**
 * Example of creating a stream and displaying images using OpenCV
 * 
 */

#include <iostream>
#include <chrono>
#include <libsparkproto/sparkproto.h>
#include "opencv2/opencv.hpp"

using namespace std::chrono_literals;

using namespace libspark::protocol;

int main() {
    // search for available device in the network
    std::unique_ptr<DeviceEnumeration> deviceEnum = std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();

    if(deviceList.empty()) {
        std::cout << "No device was found" << std::endl;
        return -1;
    }

    // create a stream of images from Left camera in RGB format
    ImageStream::Ptr imgStream = std::make_unique<ImageStream>(deviceList[0]);
    imgStream->setStreamType(StreamType::STREAM_LEFT);
    imgStream->setImageFormat(ImageFormat::FORMAT_RGB);

    try {
        imgStream->start();
    } catch (SparkException & e) {
        std::cout << "Failed to start the stream. Error: " << e.what() << std::endl;
        return 1;
    }

    for(int i = 0; i < 1000; i++) {
        ImageSet imgSet;
        imgStream->recvImageSet(imgSet);

        cv::Mat image(cv::Size(imgSet.meta().left().width(), imgSet.meta().left().height()),
                    CV_8UC3, (void*)imgSet.getBuffer(ImageSet::BUFFER_LEFT).data(), cv::Mat::AUTO_STEP
                    );

        cv::Mat bgrImg;
        cv::cvtColor(image, bgrImg, cv::COLOR_RGB2BGR);
        cv::imshow("left camera", bgrImg);
        cv::waitKey(1);
    }
    imgStream->stop();

    cv::destroyAllWindows();

    return 0;
}
