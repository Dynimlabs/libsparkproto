// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include "libsparkproto/deviceinfo.h"
#include "libsparkproto/device.pb.h"
#include "libsparkproto/constants.h"
#include "libsparkproto/configure.h"

namespace libspark {
namespace protocol {

DeviceInfo::DeviceInfo(): _deviceName(""), _model(""), _ipAddress(""),
                _firmwareVersion(""), _status(0) {
    _protocolVersion.resize(3);
}

DeviceInfo::~DeviceInfo() {}

DeviceInfo::DeviceInfo(std::string deviceName, std::string model, 
            std::string ipAddress, std::string firmwareVersion,
            std::vector<uint32_t> protocolVersion,
            int status): _deviceName(deviceName), _model(model),
            _ipAddress(ipAddress), _firmwareVersion(firmwareVersion),
            _protocolVersion(protocolVersion),
            _status(status) {
}

void DeviceInfo::printDebugString() {
    printf("\n");
    printf("Device name: %s\n", _deviceName.c_str());
    printf("Model: %s\n", _model.c_str());
    printf("IP Address: %s\n", _ipAddress.c_str());
    printf("Firmware version: %s\n", _firmwareVersion.c_str());
    printf("Protocol version: %d.%d.%d\n", _protocolVersion[0], _protocolVersion[1], _protocolVersion[2]);
    printf("Compatible: %d\n", isCompatible());
    printf("Status: %s\n", DeviceStatus_Name(DeviceStatus(_status)).c_str());
}

bool DeviceInfo::isCompatible() {
    // check client version
    // server: a.b.c
    // client: x.y.z
    // accept client if a=x, b=y, c>=z

    if (PROTOCOL_VERSION[0] != _protocolVersion[0] ||
        PROTOCOL_VERSION[1] != _protocolVersion[1] ||
        PROTOCOL_VERSION[2] > _protocolVersion[2] ) {
        return false;
    }
    else {
        return true;
    }


}

const std::string& DeviceInfo::getDeviceName() {
    return _deviceName;
}

const std::string& DeviceInfo::getModel() {
    return _model;
}

const std::string& DeviceInfo::getIpAdress() {
    return _ipAddress;
}

int DeviceInfo::getStatus() {
    return _status;
}

void DeviceInfo::setDeviceName(const std::string& deviceName) {
    _deviceName = deviceName;
}

void DeviceInfo::setModel(const std::string& model) {
    _model = model;
}

void DeviceInfo::setIpAdress(const std::string& ipAddress) {
    _ipAddress = ipAddress;
}

void DeviceInfo::setStatus(int status) {
    _status = status;
}

}  // namespace protocol
}  // namespace libspark
