// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

#include <libsparkproto/imageset.h>

namespace libspark {

namespace protocol {

class IImageEvent {

public:
    virtual ~IImageEvent() {}
    
    /**
     * @brief Callback function will called when ImageSet ready
     * 
     * @param imgSet 
     */
    virtual void onImageEvent(ImageSet &imgSet) = 0;
};


} // namespace protocol
} // namespace libspark
