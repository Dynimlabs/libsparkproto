// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once
#include <libsparkproto/imagestreamprotocol.h>

namespace libspark {

namespace protocol {

/**
 * @brief ImageStream is a class used to stream ImageSet from Spark in synchronous
 * For simpling, it used as alias of ImageStreamProtocol
 * Client can use direct ImageStreamProtocol for streaming ImageSet in synchronous
 * 
 */
using ImageStream = ImageStreamProtocol;

} // namespace protocol
} // namespace libspark
