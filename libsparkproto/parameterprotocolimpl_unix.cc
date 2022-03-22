// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


#include <libsparkproto/parameterprotocolimpl_unix.h>
#include <libsparkproto/parameterids.pb.h>
#include <libsparkproto/parameters.pb.h>
#include <libsparkproto/exception.h>
#include <libsparkproto/network.h>

namespace libspark {

namespace protocol {

ParameterProtocolImpl::ParameterProtocolImpl(const std::string &address, const std::string &service) {
    _socket = network::connectTcpSocket(address, service);
}

ParameterProtocolImpl::~ParameterProtocolImpl() {
    network::closeConnection(_socket);
}

bool ParameterProtocolImpl::readBoolParameter(int32_t id) {

    throwErrorIfInvalid(id);

    ParameterInfoBool paramInfo;
    paramInfo.set_id(ParameterID(id));

    return readParameter<bool>(ParameterType::PARAMETER_READ_BOOLEAN, paramInfo);
}

void ParameterProtocolImpl::writeBoolParameter(int32_t id, bool value) {
   
    throwErrorIfInvalid(id);

    ParameterInfoBool paramInfo;
    paramInfo.set_id(ParameterID(id));
    paramInfo.set_value(value);

    writeParameter(ParameterType::PARAMETER_WRITE_BOOLEAN, paramInfo);
}

int32_t ParameterProtocolImpl::readIntParameter(int32_t id) {

    throwErrorIfInvalid(id);

    ParameterInfoInt paramInfo;
    paramInfo.set_id(ParameterID(id));

    return readParameter<int32_t>(ParameterType::PARAMETER_READ_INT, paramInfo);
}

void ParameterProtocolImpl::writeIntParameter(int32_t id, int32_t value) {
    
    throwErrorIfInvalid(id);

    ParameterInfoInt paramInfo;
    paramInfo.set_id(ParameterID(id));
    paramInfo.set_value(value);

    writeParameter(ParameterType::PARAMETER_WRITE_INT, paramInfo);
}

double ParameterProtocolImpl::readDoubleParameter(int32_t id) {
    
    throwErrorIfInvalid(id);

    ParameterInfoDouble paramInfo;
    paramInfo.set_id(ParameterID(id));

    return readParameter<double>(ParameterType::PARAMETER_READ_DOUBLE, paramInfo);
}

void ParameterProtocolImpl::writeDoubleParameter(int32_t id, double value) {

    throwErrorIfInvalid(id);

    ParameterInfoDouble paramInfo;
    paramInfo.set_id(ParameterID(id));
    paramInfo.set_value(value);

    writeParameter(ParameterType::PARAMETER_WRITE_DOUBLE, paramInfo);
}

std::string ParameterProtocolImpl::readStringParameter(int32_t id) {

    throwErrorIfInvalid(id);

    ParameterInfoString paramInfo;
    paramInfo.set_id(ParameterID(id));

    return readParameter<std::string>(ParameterType::PARAMETER_READ_STRING, paramInfo);
}

void ParameterProtocolImpl::writeStringParameter(int32_t id, const std::string& value) {

    throwErrorIfInvalid(id);

    ParameterInfoString paramInfo;
    paramInfo.set_id(ParameterID(id));
    paramInfo.set_value(value);

    writeParameter(ParameterType::PARAMETER_WRITE_STRING, paramInfo);
}


void ParameterProtocolImpl::throwErrorIfInvalid(int32_t id) {
    if(!ParameterID_IsValid(id))
        throw SparkError("passed a invalid ParameterID");
}

void ParameterProtocolImpl::callParameterRequest(const ParameterRequest &requestMsg, ParameterResponse &responseMsg) {
    
    // just one request is called at the same time
    std::lock_guard<std::mutex> lock(_requestLock);

    // serialize request message to buffer to send to socket
    int requestSize = requestMsg.ByteSize();
    int fullRequestSize = requestSize + 4;
    std::unique_ptr<char[]> pRqBuff = std::make_unique<char[]>(fullRequestSize);
    memcpy(pRqBuff.get(), &requestSize, 4);
    requestMsg.SerializeToArray(pRqBuff.get() + 4, requestSize);

    // send request via socket
    network::sendFixedTo(_socket, pRqBuff.get(), fullRequestSize);

    // receive 4 bytes for header, is size of response message
    int resSize;
    u_char hBuff[4];
    network::recvFixedFrom(_socket, hBuff, 4);
    memcpy(&resSize, hBuff, 4);
    if(resSize <= 0) {
        throw SparkError("nothing replied from device");
    }

    // receive sequence of size bytes in header
    std::unique_ptr<char[]> pResBuff = std::make_unique<char[]>(resSize);
    network::recvFixedFrom(_socket, pResBuff.get(), resSize);

    // parse response
    responseMsg.ParseFromArray(pResBuff.get(), resSize);
}

template<typename TRet, typename TInfo>
TRet ParameterProtocolImpl::readParameter(ParameterType paramType, TInfo paramInfo) {
    
    // make ParameterRequest object
    ParameterRequest requestMsg;
    ParameterResponse responseMsg;
    requestMsg.set_paramtype(paramType);
    requestMsg.set_paramid(paramInfo.id());
    requestMsg.set_paramsize(paramInfo.ByteSize());
    requestMsg.set_paraminfo(paramInfo.SerializeAsString());

    // communicate to network
    callParameterRequest(requestMsg, responseMsg);
    
    if(responseMsg.code() != ParameterResponse::RESPONSE_OK) {
        throw SparkError(responseMsg.message());
    }

    // parse msg
    TInfo infoMsg;
    if(!infoMsg.ParseFromString(responseMsg.paraminfo()))
        throw SparkError("response message format is wrong, please check if protocol version is match to device");

    return infoMsg.value();
}

template<typename TInfo>
void ParameterProtocolImpl::writeParameter(ParameterType paramType, TInfo paramInfo) {
    
    // make ParameterRequest object
    ParameterRequest requestMsg;
    ParameterResponse responseMsg;
    requestMsg.set_paramtype(paramType);
    requestMsg.set_paramid(paramInfo.id());
    requestMsg.set_paramsize(paramInfo.ByteSize());
    requestMsg.set_paraminfo(paramInfo.SerializeAsString());

    callParameterRequest(requestMsg, responseMsg);

    if(responseMsg.code() != ParameterResponse::RESPONSE_OK) {
        throw SparkError(responseMsg.message());
    }
}

void ParameterProtocolImpl::readDeviceInfoMsg(DeviceInfoMessage &deviceInfoMsg) {
    // make ParameterRequest object
    ParameterRequest requestMsg;
    ParameterResponse responseMsg;
    requestMsg.set_paramtype(ParameterType::PARAMETER_READ_DEVICEINFO);
    requestMsg.set_paramid(ParameterID::DEVICE_INFORMATION);
    
    callParameterRequest(requestMsg, responseMsg);

    if(responseMsg.code() != ParameterResponse::RESPONSE_OK) {
        throw SparkError(responseMsg.message());
    }

    // parse msg
    if(!deviceInfoMsg.ParseFromString(responseMsg.paraminfo()))
        throw SparkError("response message format is wrong, please check if protocol version is match to device");

}

} // namespace protocol
} // namespace libspark
