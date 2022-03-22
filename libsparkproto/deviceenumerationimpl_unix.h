// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <libsparkproto/common.h>
#include <libsparkproto/deviceinfo.h>
#include <libsparkproto/exception.h>

#include <memory>
#include <netinet/in.h>
#include <ifaddrs.h>

namespace libspark {
namespace protocol {

class DeviceEnumerationImpl {
public:
    DeviceEnumerationImpl();
    virtual ~DeviceEnumerationImpl();

    DeviceList discoverDevices();

private:
    int _sock;
    /**
     * @brief Send discover broadcast over available interfaces
     */
    void sendDiscoverBroadcast();

    /**
     * @brief Find all available broadcast addresses on the host machine
     * 
     * @return List of socket addresses
     */
    std::vector<sockaddr_in> findBroadcastAddresses();

    /**
     * @brief Collect responses after broadcasting a discover message
     * 
     * @return DeviceList 
     */
    DeviceList collectDiscoverResponses();
};

}  // namespace protocol
}  // namespace libspark
