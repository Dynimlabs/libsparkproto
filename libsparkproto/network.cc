// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ifaddrs.h>
#include <poll.h>
#include <string.h>
#include <memory>

#include <libsparkproto/network.h>
#include <libsparkproto/exception.h>

namespace libspark {

namespace protocol {

namespace network {

using smart_addrinfo = std::unique_ptr<addrinfo, void (*)(addrinfo*)>;

static smart_addrinfo resolveAddress(const char* address, const char* service) {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Use IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    addrinfo* addressInfo = nullptr;

    if(getaddrinfo(address, service, &hints, &addressInfo) != 0 || addressInfo == nullptr) {
        throw SparkError("error resolving address: " + std::string(strerror(errno)));
    }

    if(addressInfo->ai_addrlen != sizeof(sockaddr_in)) {
        freeaddrinfo(addressInfo);
        throw SparkError("illegal address length");
    }

    // wrap addrinfo raw pointer to auto release when a exception occured
    smart_addrinfo addressInfoWrap(addressInfo, freeaddrinfo);
    
    return std::move(addressInfoWrap);
}

int connectTcpSocket(const std::string &address, const std::string &service) {
    
    smart_addrinfo addressInfo = resolveAddress(address.c_str(), service.c_str());

    SOCKET sock = ::socket(addressInfo->ai_family, addressInfo->ai_socktype,
        addressInfo->ai_protocol);
    if(sock == INVALID_SOCKET) {
        throw SparkError("error creating socket: " + std::string(strerror(errno)));
    }

    if(connect(sock, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen)) < 0) {
        throw SparkError("error connection to destination address: " + std::string(strerror(errno)));
    }

    return sock;
}

void closeConnection(SOCKET &socket) {
    if(socket > 0) {
        ::close(socket);
        socket = -1;
    }
}

void sendFixedTo(SOCKET socket, void *buff, uint32_t fixedSize) {

    ssize_t sentSize = ::send(socket, buff, fixedSize, MSG_NOSIGNAL);
    if(sentSize < fixedSize) {
        char errorBuff[1024];
        sprintf(errorBuff, "just sent %ld bytes of buffer, require minimum of %d bytes, error: %s", sentSize, fixedSize, strerror(errno));
        throw SparkError(std::string(errorBuff));
    }
}

void recvFixedFrom(SOCKET socket, void *buff, uint32_t fixedSize) {
    
    char* buffer = (char*) buff;

    for(ssize_t offset = 0; offset < fixedSize;) {
        ssize_t recvSize = ::recv(socket, buffer + offset, fixedSize - offset, 0);
        if(recvSize > 0) {
            offset += recvSize;
            continue;
        }
        else {
            char errorBuff[1024];
            sprintf(errorBuff, "just received %ld bytes of buffer, require minimum of %d bytes, error: %s", offset, fixedSize, strerror(errno));
            throw SparkError(std::string(errorBuff));
        }
    }
}

} // namespace network
} // namespace protocol
} // namespace libspark
