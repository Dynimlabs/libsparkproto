// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <libsparkproto/deviceenumeration.h>
#include <libsparkproto/deviceenumerationimpl.h>

namespace libspark {
namespace protocol {

DeviceEnumeration::DeviceEnumeration() {
    _pImpl = std::make_unique<DeviceEnumerationImpl>();
}

DeviceEnumeration::~DeviceEnumeration() {}

DeviceList DeviceEnumeration::discoverDevices() {
    return _pImpl->discoverDevices();
}

}  // namespace protocol
}  // namespace libspark
