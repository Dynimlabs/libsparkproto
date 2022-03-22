// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


/**
 * Example of discovering Spark cameras in the network
 * 
 */
#include <libsparkproto/sparkproto.h>

using namespace libspark::protocol;

int main(int argc, char const *argv[]) {
    
    // print library info
    printLibraryInfo();

    std::unique_ptr<DeviceEnumeration> deviceEnum = std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();

    // print all the information of available Spark devices in the network
    for (const auto& devInfo : deviceList) {
        devInfo->printDebugString();
    }

    return 0;
}
