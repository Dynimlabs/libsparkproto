// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <stdexcept>

namespace libspark
{
    namespace protocol
    {

        class SparkException : public std::runtime_error
        {
        public:
            SparkException(std::string msg, const char *file, int32_t line);
            ~SparkException();

            std::string toString();
            const char* what() const throw() override;

        private:
            std::string _fullMsg;
            std::string _msg;
            const char *_throw_file;
            int32_t _throw_line;
        };

#define SparkError(msg) SparkException(msg, __FILE__, __LINE__)

    }
}
