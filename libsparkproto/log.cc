// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <string.h>
#include <stdarg.h>
#include <string>
#include <memory>

#include <libsparkproto/log.h>

namespace libspark
{
    namespace protocol
    {
#define STD_DEV stdout

        static constexpr int32_t BUFFER_SIZE = 1024;

        void log(const char* filename, int line, LogLevel level, const char* format, ... )
        {
            va_list format_args;
            va_start(format_args, format);

            auto buffer = std::make_unique<char[]>(BUFFER_SIZE);
            memset(buffer.get(), 0x00, BUFFER_SIZE);
            // If we would have static buffer, then the trouble is with multithread access
            ssize_t ret = vsnprintf(buffer.get(), BUFFER_SIZE, format, format_args);
            va_end(format_args);

            if (ret <= 0)
            {
                fprintf(STD_DEV, "Log call failed from '%s':%d\n", filename, line);
                return;
            }

            if (ret >= BUFFER_SIZE)
            {
                fprintf(STD_DEV, "Log too long at '%s':%d\n", filename, line);
                return;
            }

            char level_key = 'E';
            switch (level)
            {
            case LogLevel::DEBUG:
                level_key = 'D';
                break;

            case LogLevel::INFO:
                level_key = 'I';
                break;

            case LogLevel::ERROR:
                level_key = 'E';
                break;

            case LogLevel::WARNING:
                level_key = 'W';
                break;

            default:
                level_key = 'E';
                break;
            }

            fprintf(STD_DEV, "[%c] %s:%d: %s\n", level_key, basename(filename), line, buffer.get());
        }
    }
}
