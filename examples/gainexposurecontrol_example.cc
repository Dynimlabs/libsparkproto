// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

/**
 * Example of manually controlling gain and exposure using keyboard
 *
 * Press d/e and f/r on the displaying window for adjusting the gain and
 * exposure respectively
 *
 */

#include <iostream>
#include <libsparkproto/sparkproto.h>
#include "opencv2/opencv.hpp"

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

    // create a device configuration object
    DeviceParamConfigure::Ptr deviceConfigure =
        std::make_unique<DeviceParamConfigure>(deviceList[0]);
    // set the Auto Exposure (AE) to OFF (i.e. manual) by default
    deviceConfigure->setAutoExposure(false);

    try {
        imgStream->start();
    } catch (SparkException & e) {
        std::cout << "Failed to start the stream. Error: " << e.what() << std::endl;
        return 1;
    }

    bool isStreaming = true;
    int currExposure = deviceConfigure->getExposureValue();
    int currGain = deviceConfigure->getGainValue();
    int maxExposure = 36200, minExposure = 6,
    maxGain = 480, minGain = 0,
    exposureStep = 500, gainStep = 10;


    while (isStreaming) {
        ImageSet imgSet;
        imgStream->recvImageSet(imgSet);

        cv::Mat image(cv::Size(imgSet.meta().left().width(),
                               imgSet.meta().left().height()),
                      CV_8UC3,
                      (void*)imgSet.getBuffer(ImageSet::BUFFER_LEFT).data(),
                      cv::Mat::AUTO_STEP);

        cv::Mat bgrImg;
        cv::cvtColor(image, bgrImg, cv::COLOR_RGB2BGR);
        cv::imshow("left camera", bgrImg);

        int k = cv::waitKey(1);

        if (k == -1) continue;
        switch (k)
        {
        case 68: // D
        case 100: // d
            currExposure -= exposureStep;
            if (currExposure >= minExposure) {
                deviceConfigure->setExposureValue(currExposure);
                std::cout << "Decrease exposure value. New value: "
                          << currExposure << std::endl;
            } else {
                currExposure = minExposure;
                std::cout << "Min exposure reached" << std::endl;
            }
            break;
        case 69: // E
        case 101: // e
            currExposure += exposureStep;
            if (currExposure <= maxExposure) {
                deviceConfigure->setExposureValue(currExposure);
                std::cout << "Increase exposure value. New value: "
                          << currExposure << std::endl;
            } else {
                currExposure = maxExposure;
                std::cout << "Max exposure value reached" << std::endl;
            }
            break;
        case 70: // F
        case 102: // f
            currGain -= gainStep;
            if (currGain >= minGain) {
                deviceConfigure->setGainValue(currGain);
                std::cout << "Decrease gain value. New value: " << currGain
                          << std::endl;
            } else {
                currGain = minGain;
                std::cout << "Min gain reached" << std::endl;
            }
            break;
        case 82: // R
        case 114: // r
            currGain += gainStep;
            if (currGain <= maxGain) {
                deviceConfigure->setGainValue(currGain);
                std::cout << "Increase gain value. New value: "
                          << currGain << std::endl;
            } else {
                currGain = maxGain;
                std::cout << "Max gain value reached";
            }
            break;
        
        case 27: // ESC
            isStreaming = false;
            break;

        default:
            std::cout
                << "Press d/e for adjusting exposure; f/r for gain; ESC for quit"
                << std::endl;
            break;
        }
    }
    imgStream->stop();

    cv::destroyAllWindows();

    return 0;
}
