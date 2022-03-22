// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

/**
 * Example of querying device configuration parameters
 *
 */
#include <libsparkproto/sparkproto.h>

#include <iostream>

using namespace libspark::protocol;

int main() {
    // search for available device in the network
    std::unique_ptr<DeviceEnumeration> deviceEnum =
        std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();

    if (deviceList.empty()) {
        std::cout << "No device was found" << std::endl;
        return -1;
    }

    DeviceParamConfigure::Ptr deviceConfigure =
        std::make_unique<DeviceParamConfigure>(deviceList[0]);
    std::cout << "AutoExposure: " << deviceConfigure->getAutoExposure()
              << std::endl;
    std::cout << "AutoExposureMode: " << deviceConfigure->getAutoExposureMode()
              << std::endl;
    std::cout << "ExposureValue: " << deviceConfigure->getExposureValue()
              << std::endl;
    std::cout << "AutoWB: " << deviceConfigure->getAutoWhiteBalance()
              << std::endl;
    std::cout << "AutoWBMode: " << deviceConfigure->getAutoWhiteBalanceMode()
              << std::endl;
    std::cout << "WBValue: " << deviceConfigure->getWhiteBalanceValue()
              << std::endl;
    std::cout << "Resolution index: " << deviceConfigure->getResolution()
              << std::endl;
    std::cout << "GainValue: " << deviceConfigure->getGainValue() << std::endl;
    std::cout << "LedMode: " << deviceConfigure->getLedMode() << std::endl;
    std::cout << "LedBrightnessLevel: "
              << deviceConfigure->getLedBrightnessLevel() << std::endl;

    std::cout << "Export calibrationdata to calib.yaml " << std::endl;
    deviceConfigure->exportCalibrationData("calib.yaml");

    std::cout << "Device informations:" << std::endl;
    std::cout << "===================" << std::endl;
    DeviceInfoMessage deviceInfoMsg;
    deviceConfigure->readDeviceInfoMsg(deviceInfoMsg);
    deviceInfoMsg.PrintDebugString();

    return 0;
}
