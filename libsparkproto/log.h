// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <string.h>

namespace libspark
{
namespace protocol
{

#ifdef SPARK_LOGGING_ENABLE

#define LOG_ERROR(format, ...) libspark::protocol::log(__FILE__, __LINE__, libspark::protocol::LogLevel::ERROR, format, ##__VA_ARGS__)
#define LOG_PERROR(format, ...) libspark::protocol::log(__FILE__, __LINE__, libspark::protocol::LogLevel::ERROR, format, ##__VA_ARGS__, strerror(errno))
#define LOG_WARNING(format, ...) libspark::protocol::log(__FILE__, __LINE__, libspark::protocol::LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) libspark::protocol::log(__FILE__, __LINE__, libspark::protocol::LogLevel::INFO, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) libspark::protocol::log(__FILE__, __LINE__, libspark::protocol::LogLevel::DEBUG, format, ##__VA_ARGS__)

#else

#define LOG_ERROR(format, ...) do {} while(0)
#define LOG_PERROR(format, ...) do {} while(0)
#define LOG_WARNING(format, ...) do {} while(0)
#define LOG_INFO(format, ...) do {} while(0)
#define LOG_DEBUG(format, ...) do {} while(0)

#endif

        enum class LogLevel
        {
            ERROR = 0,
            WARNING,
            INFO,
            DEBUG
        };

        void log(const char* filename, int line, LogLevel level, const char* format, ...);

} // namespace proto
} // namespace libspark
