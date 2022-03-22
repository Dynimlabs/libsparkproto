// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <string>

namespace libspark {

namespace protocol {

namespace network {

#define SOCKET int
#define INVALID_SOCKET -1

int connectTcpSocket(const std::string &address, const std::string &service);

void closeConnection(SOCKET &socket);

/**
 * @brief send fixedSize bytes to socket,
 * if the sent buffer size is not equal fixedSized, a exception will be thrown
 * 
 * @param socket 
 * @param buff 
 * @param fixedSize
 */
void sendFixedTo(SOCKET socket, void *buff, uint32_t fixedSize);

/**
 * @brief receive fixedSize bytes from socket, 
 * if the received buffer size is not equal fixedSize, a exception will be thrown
 * 
 * @param socket 
 * @param buff 
 * @param fixedSize 
 */
void recvFixedFrom(SOCKET socket, void *buff, uint32_t fixedSize);


} // namespace network
} // namespace sparkprot
} // namespace libspark
