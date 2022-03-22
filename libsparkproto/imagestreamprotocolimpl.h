// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#if (defined(_WIN32))
    #include <libsparkproto/imagestreamprotocolimpl_win32.h>
#else
    #include <libsparkproto/imagestreamprotocolimpl_unix.h>
#endif
