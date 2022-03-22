// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/deviceenumerationimpl_unix.h>
#include <libsparkproto/constants.h>
#include <libsparkproto/device.pb.h>
#include <libsparkproto/log.h>
#include <libsparkproto/exception.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <unistd.h>

namespace libspark {
namespace protocol {

DeviceEnumerationImpl::DeviceEnumerationImpl() {
    // create and register for the device discorvery socket
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sock < 0) {
        throw SparkError("failed to create a socket for device discovery. Error: " + std::string(strerror(errno)));
    }

    // set permission flag for socket broadcast
    int broadcastPermission = 1;
    setsockopt(_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&broadcastPermission),
            sizeof(broadcastPermission));

    // set timeout for broadcast message listening
    // Note: this could affect the response time of incoming requests
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1e3;
    setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
}

DeviceEnumerationImpl::~DeviceEnumerationImpl() {
    ::close(_sock);
}

DeviceList DeviceEnumerationImpl::discoverDevices() {
    sendDiscoverBroadcast();
    return collectDiscoverResponses();
}

void DeviceEnumerationImpl::sendDiscoverBroadcast() {
    std::vector<sockaddr_in> addresses = findBroadcastAddresses();
    for(sockaddr_in addr: addresses) {
        addr.sin_port = htons(DISCOVERY_BROADCAST_PORT);

        int ret = sendto(_sock, DISCOVERY_BROADCAST_MSG.c_str(),
                DISCOVERY_BROADCAST_MSG.length(), 0,
                (struct sockaddr *) &addr, sizeof(addr));

        if (ret < 0) {
            throw SparkError("error sending broadcast message. Error: " + std::string(strerror(errno)));
        }
    }
}

DeviceList DeviceEnumerationImpl::collectDiscoverResponses() {
    DeviceList deviceList;
    while (true) {
        sockaddr_in senderAddress;
        socklen_t senderLength = sizeof(senderAddress);

        // UDP messages cannot be splitted to receive multiple times,
        // thus it should be receive once with max size
        std::unique_ptr<char[]> packageBuff = std::make_unique<char[]>(MAX_DISCOVERY_MSG_SIZE);
        int ret = recvfrom(_sock, packageBuff.get(), MAX_DISCOVERY_MSG_SIZE, 0,
                            (sockaddr *)&senderAddress, &senderLength);
        if (ret < 4)
            break;

        // get package size from header
        uint32_t headerSize = 4;
        uint32_t payloadSize;
        memcpy(&payloadSize, packageBuff.get(), headerSize);
        // validate payload size
        if( payloadSize != ret - headerSize) {
            LOG_ERROR("payload size is invalid");
            continue;
        }

        DiscoveryMessage msg;
        ret = msg.ParseFromArray(packageBuff.get() + headerSize, payloadSize);
        if (!ret) {
            LOG_ERROR("failed to parse the message");
            continue;
        }

        // get IP address of the sender
        std::unique_ptr<char[]> ipStrBuff = std::make_unique<char[]>(INET6_ADDRSTRLEN);
        inet_ntop(AF_INET, &senderAddress.sin_addr, ipStrBuff.get(), INET_ADDRSTRLEN);
        std::string ipAddrStr(ipStrBuff.get(), INET_ADDRSTRLEN);

        std::vector<uint32_t> protocolVersion { msg.protocolversion().major_(),
            msg.protocolversion().minor_(), 
            msg.protocolversion().patch_()
            };
        std::shared_ptr<DeviceInfo> deviceInfo = std::make_shared<DeviceInfo>(
            msg.devicename(), msg.model(), ipAddrStr, msg.firmwareversion(),
            protocolVersion,
            msg.status()
        );
        deviceList.push_back(deviceInfo);
    }

    return deviceList;
}

std::vector<sockaddr_in> DeviceEnumerationImpl::findBroadcastAddresses() {
    std::vector<sockaddr_in> ret;
    // BSD-style implementation
    struct ifaddrs * ifap;
    if (getifaddrs(&ifap) == 0) {
        struct ifaddrs * p = ifap;
        while(p) {
            if(p->ifa_dstaddr != nullptr && p->ifa_dstaddr->sa_family == AF_INET) {
                ret.push_back(*reinterpret_cast<sockaddr_in*>(p->ifa_dstaddr));
            }
            p = p->ifa_next;
        }
        freeifaddrs(ifap);
    }

    return ret;
}

}  // namespace protocol
}  // namespace libspark
