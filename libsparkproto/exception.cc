// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#include <string.h>
#include <libsparkproto/exception.h>

namespace libspark
{
    namespace protocol
    {

        SparkException::SparkException(std::string msg, const char *file, int32_t line)
            : std::runtime_error(msg), _msg(msg), _throw_line(line)
        {
            char *file_mod = strdup(file);
            _throw_file = strdup(basename(file_mod));
            free(file_mod);

            _fullMsg = toString();
        } 

        SparkException::~SparkException()
        {
            free((void*)_throw_file);
        }

        std::string SparkException::toString()
        {
            static int32_t buffSize = 1024;

            if(_msg.size() > buffSize)
            {
                _msg.resize(buffSize/2);
            }
            char buff[buffSize];
            sprintf(buff, "SparkException thrown from %s:%d %s", _throw_file, _throw_line, _msg.c_str());

            return std::string{buff};
        }

        const char* SparkException::what() const throw() {
            return _fullMsg.c_str();
        }
    }
}
