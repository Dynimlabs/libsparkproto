// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


#include <fstream>

#include <libsparkproto/deviceparamconfigureimpl.h>
#include <libsparkproto/parameterids.pb.h>
#include <libsparkproto/exception.h>
#include <libsparkproto/constants.h>

namespace libspark
{
namespace protocol
{

DeviceParamConfigureImpl::DeviceParamConfigureImpl(std::shared_ptr<DeviceInfo> pDevice) {
    if(!pDevice->isCompatible())
        throw SparkError("The library is not compatible with Spark firmware, please upgrade new version of libsparkpro");
    
    _pProtocol = std::make_unique<ParameterProtocol>(pDevice->getIpAdress(), std::to_string(PARAMETERS_PORT));
}

DeviceParamConfigureImpl::~DeviceParamConfigureImpl() {
}

void DeviceParamConfigureImpl::setAutoExposure(bool enable) {
    _pProtocol->writeBoolParameter(ParameterID::AUTO_EXPOSURE, enable);
}

bool DeviceParamConfigureImpl::getAutoExposure() {
    return _pProtocol->readBoolParameter(ParameterID::AUTO_EXPOSURE);
}

void DeviceParamConfigureImpl::setAutoExposureMode(int32_t mode) {
    _pProtocol->writeIntParameter(ParameterID::AUTO_EXPOSURE_MODE, mode);
}

int32_t DeviceParamConfigureImpl::getAutoExposureMode() {
    return _pProtocol->readIntParameter(ParameterID::AUTO_EXPOSURE_MODE);
}

void DeviceParamConfigureImpl::setExposureValue(int32_t value) {
    _pProtocol->writeIntParameter(ParameterID::MANUAL_EXPOSURE, value);
}

int32_t DeviceParamConfigureImpl::getExposureValue() {
    return _pProtocol->readIntParameter(ParameterID::MANUAL_EXPOSURE);
}

void DeviceParamConfigureImpl::setResolution(int32_t index) {
    _pProtocol->writeIntParameter(ParameterID::RESOLUTION, index);
}

int32_t DeviceParamConfigureImpl::getResolution() {
    return _pProtocol->readIntParameter(ParameterID::RESOLUTION);
}

void DeviceParamConfigureImpl::setGainValue(int32_t value) {
    _pProtocol->writeIntParameter(ParameterID::MANUAL_GAIN, value);
}

int32_t DeviceParamConfigureImpl::getGainValue() {
    return _pProtocol->readIntParameter(ParameterID::MANUAL_GAIN);
}

void DeviceParamConfigureImpl::setAutoWhiteBalance(bool enable) {
    _pProtocol->writeBoolParameter(ParameterID::AUTO_WB, enable);
}

bool DeviceParamConfigureImpl::getAutoWhiteBalance() {
    return _pProtocol->readBoolParameter(ParameterID::AUTO_WB);
}

void DeviceParamConfigureImpl::setAutoWhiteBalanceMode(int32_t value) {
    _pProtocol->writeIntParameter(ParameterID::AUTO_WB_MODE, value);
}

int32_t DeviceParamConfigureImpl::getAutoWhiteBalanceMode() {
    return _pProtocol->readIntParameter(ParameterID::AUTO_WB_MODE);
}

void DeviceParamConfigureImpl::setWhiteBalanceValue(int32_t value) {
    _pProtocol->writeIntParameter(ParameterID::MANUAL_WB, value);
}

int32_t DeviceParamConfigureImpl::getWhiteBalanceValue() {
    return _pProtocol->readIntParameter(ParameterID::MANUAL_WB);
}

void DeviceParamConfigureImpl::setLedMode(bool enable) {
    _pProtocol->writeBoolParameter(ParameterID::LED_MODE, enable);
}

bool DeviceParamConfigureImpl::getLedMode() {
    return _pProtocol->readBoolParameter(ParameterID::LED_MODE);
}

void DeviceParamConfigureImpl::setLedBrightnessLevel(int32_t value) {
    _pProtocol->writeIntParameter(ParameterID::LED_BRIGHTNESS_LEVEL, value);
}

int32_t DeviceParamConfigureImpl::getLedBrightnessLevel() {
    return _pProtocol->readIntParameter(ParameterID::LED_BRIGHTNESS_LEVEL);
}

void DeviceParamConfigureImpl::exportCalibrationData(std::string filename) {
    std::string buff = _pProtocol->readStringParameter(ParameterID::CALIBRATION_DATA);

    std::ofstream calFile(filename);
    if(calFile.is_open()) {
        calFile << buff;
    }
    else {
        throw SparkError("can not open file");
    }
}

void DeviceParamConfigureImpl::readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg) {
    _pProtocol->readDeviceInfoMsg(deviceInfoMsg);
}

} // namespace protocol
} // namespace libspark
