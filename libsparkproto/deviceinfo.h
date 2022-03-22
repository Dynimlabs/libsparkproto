// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <memory>
#include <vector>
#include <libsparkproto/common.h>

namespace libspark {
namespace protocol {

class DeviceInfo;
using DeviceList = std::vector<std::shared_ptr<DeviceInfo>>;

class SPARK_API DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(std::string deviceName, std::string model, 
                std::string ipAddress, std::string firmwareVersion,
                std::vector<uint32_t> protocolVersion,
                int status);
    virtual ~DeviceInfo();

    /**
     * @brief Print the device info information to stdout
     * 
     */
    void printDebugString();

    /**
     * @brief Check if libsproto is compatible with Spark device
     * 
     * @return true 
     * @return false 
     */
    bool isCompatible();

    const std::string& getDeviceName();
    const std::string& getModel();
    const std::string& getIpAdress();
    int getStatus();

    void setDeviceName(const std::string& deviceName);
    void setModel(const std::string& model);
    void setIpAdress(const std::string& ipAddress);
    void setStatus(int status);

private:
    std::string _deviceName;
    std::string _model;
    std::string _ipAddress;
    std::string _firmwareVersion;
    std::vector<uint32_t> _protocolVersion;
    int _status;
};

}  // namespace protocol
}  // namespace libspark
