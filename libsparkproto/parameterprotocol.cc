// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/parameterprotocol.h>
#include <libsparkproto/parameterprotocolimpl.h>

namespace libspark {

namespace protocol {

ParameterProtocol::ParameterProtocol(const std::string &address, const std::string &service) 
    : _pImpl(new ParameterProtocolImpl(address, service)){

}

ParameterProtocol::~ParameterProtocol() {

}

bool ParameterProtocol::readBoolParameter(int32_t id) {
    return _pImpl->readBoolParameter(id);
}

void ParameterProtocol::writeBoolParameter(int32_t id, bool value) {
    _pImpl->writeBoolParameter(id, value);
}

int32_t ParameterProtocol::readIntParameter(int32_t id) {
    return _pImpl->readIntParameter(id);
}

void ParameterProtocol::writeIntParameter(int32_t id, int32_t value) {
    _pImpl->writeIntParameter(id, value);
}

double ParameterProtocol::readDoubleParameter(int32_t id) {
    return _pImpl->readDoubleParameter(id);
}

void ParameterProtocol::writeDoubleParameter(int32_t id, double value) {
    _pImpl->writeDoubleParameter(id, value);
}

std::string ParameterProtocol::readStringParameter(int32_t id) {
    return _pImpl->readStringParameter(id);
}

void ParameterProtocol::writeStringParameter(int32_t id, const std::string &value) {
    _pImpl->writeStringParameter(id, value);
}

void ParameterProtocol::readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg) {
    _pImpl->readDeviceInfoMsg(deviceInfoMsg);
}

}
}
