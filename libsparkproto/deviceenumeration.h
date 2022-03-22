// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <libsparkproto/common.h>
#include <libsparkproto/deviceinfo.h>

#include <memory>
#include <vector>

namespace libspark {
namespace protocol {

class DeviceEnumerationImpl;
class SPARK_API DeviceEnumeration {
public:
    DeviceEnumeration();
    virtual ~DeviceEnumeration();

    /**
     * @brief Discovers new devices and returns the list of all devices
     * that have been found
     * @return DeviceList
     */
    DeviceList discoverDevices();

private:
    std::unique_ptr<DeviceEnumerationImpl> _pImpl;
};

}  // namespace protocol
}  // namespace libspark
