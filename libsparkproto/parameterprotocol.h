// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <memory>

namespace libspark {

namespace protocol {

class DeviceInfoMessage; // defined at libsparkproto/device.pb.h
class ParameterProtocolImpl;
class ParameterProtocol {

public:

    /**
     * @brief Construct a new ParameterProtocol object
     * 
     */
    ParameterProtocol(const std::string &address, const std::string &service);

    /**
     * @brief Destroy the ParameterProtocol object
     * 
     */
    virtual ~ParameterProtocol();

    /**
     * @brief read a bool parameter from device by id
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool readBoolParameter(int32_t id);

    /**
     * @brief write a bool value by id to device 
     * 
     * @param id 
     * @param value 
     */
    void writeBoolParameter(int32_t id, bool value);

    /**
     * @brief read a int value from device by id
     * 
     * @param id 
     * @return int32_t 
  
     */
    int32_t readIntParameter(int32_t id);

    /**
     * @brief write int value by id to device
     * 
     * @param id 
     * @param value
     */
    void writeIntParameter(int32_t id, int32_t value);

    /**
     * @brief read double value from device by id
     * 
     * @param id 
     * @return double 
     */
    double readDoubleParameter(int32_t id);

    /**
     * @brief write a double value by id to device
     * 
     * @param id 
     * @param value 
     */
    void writeDoubleParameter(int32_t id, double value);

    /**
     * @brief read a string value by id from device
     * 
     * @param id 
     * @return std::string 
     */
    std::string readStringParameter(int32_t id);

    /**
     * @brief write a string value by id to device
     * 
     * @param id 
     * @param value 
     */
    void writeStringParameter(int32_t id, const std::string &value);

    /**
     * @brief read detail informations of device. The information is defined as a protobuf message, DeviceInfoMessage.
     * 
     * @param deviceInfoMsg 
     */
    void readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg);

private:
    std::unique_ptr<ParameterProtocolImpl> _pImpl;

};

} // namespace protocol
} // namespace libspark
