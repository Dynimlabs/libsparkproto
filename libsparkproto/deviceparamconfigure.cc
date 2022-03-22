// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


#include <libsparkproto/deviceparamconfigure.h>
#include <libsparkproto/deviceparamconfigureimpl.h>

namespace libspark {
namespace protocol {

DeviceParamConfigure::DeviceParamConfigure(std::shared_ptr<DeviceInfo> pDevice)
    : _pImpl(new DeviceParamConfigureImpl(pDevice)) {
}

DeviceParamConfigure::~DeviceParamConfigure() {
}

void DeviceParamConfigure::setAutoExposure(bool enable) {
    _pImpl->setAutoExposure(enable);
}

bool DeviceParamConfigure::getAutoExposure() {
    return _pImpl->getAutoExposure();
}

void DeviceParamConfigure::setAutoExposureMode(int32_t mode) {
    _pImpl->setAutoExposureMode(mode);
}

int32_t DeviceParamConfigure::getAutoExposureMode() {
    return _pImpl->getAutoExposureMode();
}

void DeviceParamConfigure::setExposureValue(int32_t value) {
    _pImpl->setExposureValue(value);
}

int32_t DeviceParamConfigure::getExposureValue() {
    return _pImpl->getExposureValue();
}

void DeviceParamConfigure::setResolution(int32_t index) {
    _pImpl->setResolution(index);
}

int32_t DeviceParamConfigure::getResolution() {
    return _pImpl->getResolution();
}

void DeviceParamConfigure::setGainValue(int32_t value) {
    _pImpl->setGainValue(value);
}

int32_t DeviceParamConfigure::getGainValue() {
    return _pImpl->getGainValue();
}

void DeviceParamConfigure::setAutoWhiteBalance(bool enable) {
    _pImpl->setAutoWhiteBalance(enable);
}

bool DeviceParamConfigure::getAutoWhiteBalance() {
    return _pImpl->getAutoWhiteBalance();
}

void DeviceParamConfigure::setAutoWhiteBalanceMode(int32_t value) {
    _pImpl->setAutoWhiteBalanceMode(value);
}

int32_t DeviceParamConfigure::getAutoWhiteBalanceMode() {
    return _pImpl->getAutoWhiteBalanceMode();
}

void DeviceParamConfigure::setWhiteBalanceValue(int32_t value) {
    return _pImpl->setWhiteBalanceValue(value);
}

int32_t DeviceParamConfigure::getWhiteBalanceValue() {
    return _pImpl->getWhiteBalanceValue();
}

void DeviceParamConfigure::setLedMode(bool enable) {
    _pImpl->setLedMode(enable);
}

bool DeviceParamConfigure::getLedMode() {
    return _pImpl->getLedMode();
}

void DeviceParamConfigure::setLedBrightnessLevel(int32_t value) {
    return _pImpl->setLedBrightnessLevel(value);
}

int32_t DeviceParamConfigure::getLedBrightnessLevel() {
    return _pImpl->getLedBrightnessLevel();
}

void DeviceParamConfigure::exportCalibrationData(std::string filename) {
    _pImpl->exportCalibrationData(filename);
}

void DeviceParamConfigure::readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg) {
    _pImpl->readDeviceInfoMsg(deviceInfoMsg);
}

} // namespace protocol
} // namespace libspark
