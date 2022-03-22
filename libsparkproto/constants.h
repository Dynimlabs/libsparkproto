// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <string>
#include <vector>

namespace libspark {
namespace protocol {

static const std::vector<uint32_t> PROTOCOL_VERSION {2, 0, 0};

static const std::string DISCOVERY_BROADCAST_MSG = "SPARK-DISCOVER";
static constexpr int DISCOVERY_BROADCAST_PORT = 2002;
static constexpr int IMAGEDATA_PORT = 2003;
static constexpr int PARAMETERS_PORT = 2004;

static constexpr int MAX_DISCOVERY_MSG_SIZE = 1024;

static const std::string CAMERA_DEVICE_0 = "/dev/video0";
static const std::string CAMERA_DEVICE_1 = "/dev/video1";
static constexpr uint32_t CAMERA_DEFAULT_WIDTH = 1440;
static constexpr uint32_t CAMERA_DEFAULT_HEIGHT = 1080;
static constexpr int CAMERA_BUFFER_COUNT = 3;
static constexpr int CAMERA_IOCTL_RETRY = 5;

}  // namespace protocol
}  // namespace libspark
