// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <memory>

#include <libsparkproto/common.h>
#include <libsparkproto/deviceinfo.h>
#include <libsparkproto/device.pb.h>

namespace libspark {

namespace protocol {

class DeviceParamConfigureImpl;
class SPARK_API DeviceParamConfigure
{
public:
    using Ptr = std::unique_ptr<DeviceParamConfigure>;
    
    /**
     * @brief Construct a new Device Parameters object
     * 
     * @param pDevice 
     */
    DeviceParamConfigure(std::shared_ptr<DeviceInfo> pDevice);

    /**
     * @brief Destroy the Device Parameters object
     * 
     */
    virtual ~DeviceParamConfigure();

    /**
     * @brief Set the Auto Exposure (ON/OFF)
     * 
     * @param enable 
     */
    void setAutoExposure(bool enable);

    /**
     * @brief Get the Auto Exposure
     * 
     * @return true 
     * @return false 
     */
    bool getAutoExposure();

    /**
     * @brief Set the Auto Exposure Mode
     * 
     * @param mode 
     */
    void setAutoExposureMode(int32_t mode);

    /**
     * @brief Get the Auto Exposure Mode
     * 
     * @return int32_t 
     */
    int32_t getAutoExposureMode();


    /**
     * @brief Set the Exposure Value
     * Exposure value ranging from 6 to 36200 (0.06ms -> 362ms)
     * 
     * @param value exposure value
     * 
     */
    void setExposureValue(int32_t value);

    /**
     * @brief Get the Exposure Value
     * 
     * @return int32_t 
     */
    int32_t getExposureValue();

    /**
     * @brief Set the Resolution
     * See list of resolution supported at libspark::protocol::SupportedResolution
     * @param index 
     */
    void setResolution(int32_t index);

    /**
     * @brief Get the Resolution
     * 
     * @return int32_t 
     */
    int32_t getResolution();

    /**
     * @brief Set the Gain Value
     * Gain value ranging from 0 to 480 (0 -> 48 dB)
     * @param value gain value
     */
    void setGainValue(int32_t value);

    /**
     * @brief Get the Gain Value
     * 
     * @return int32_t 
     */
    int32_t getGainValue();

    /**
     * @brief Set the Auto White Balance
     * 
     * @param enable 
     */
    void setAutoWhiteBalance(bool enable);

    /**
     * @brief Get the Auto White Balance
     * 
     * @return true 
     * @return false 
     */
    bool getAutoWhiteBalance();

    /**
     * @brief Set the Auto White Balance Mode
     * 
     * @param value 
     */
    void setAutoWhiteBalanceMode(int32_t value);

    /**
     * @brief Get the Auto White Balance Mode
     * 
     * @return int32_t 
     */
    int32_t getAutoWhiteBalanceMode();

    /**
     * @brief Set the White Balance Value
     * 
     * @param value 
     */
    void setWhiteBalanceValue(int32_t value);

    /**
     * @brief Get the White Balance Value
     * 
     * @return int32_t 
     */
    int32_t getWhiteBalanceValue();

    /**
     * @brief Set the Led Mode
     * 
     * @param enable 
     */
    void setLedMode(bool enable);

    /**
     * @brief Get the Led Mode
     * 
     * @return true 
     * @return false 
     */
    bool getLedMode();

    /**
     * @brief Set the Led Brightness Level
     * 
     * @param value 
     */
    void setLedBrightnessLevel(int32_t value);

    /**
     * @brief Get the Led BrightnessLevel
     * 
     * @return int32_t 
     */
    int32_t getLedBrightnessLevel();

    /**
     * @brief export calibration data to file
     * filename could be a path to file, or just filename only.
     * in case filename only, the file will created at currently working folder
     * 
     * @param filename 
     */
    void exportCalibrationData(std::string filename);

    /**
     * @brief read detail informations of device. The information is defined as a protobuf message, DeviceInfoMessage.
     * 
     * @param deviceInfoMsg 
     */
    void readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg);

private:
    std::unique_ptr<DeviceParamConfigureImpl> _pImpl;
};
} // namespace protocol
} // namespace libspark
