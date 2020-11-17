//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2020, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------

#include "tsAbstractVVCAccessUnit.h"
#include "tsMemory.h"
TSDUCK_SOURCE;


//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

ts::AbstractVVCAccessUnit::AbstractVVCAccessUnit() :
    SuperClass(),
    forbidden_zero_bit(0),
    nuh_reserved_zero_bit(0),
    nuh_layer_id(0),
    nal_unit_type(0),
    nuh_temporal_id_plus1(0)
{
}


//----------------------------------------------------------------------------
// Clear all values
//----------------------------------------------------------------------------

void ts::AbstractVVCAccessUnit::clear()
{
    SuperClass::clear();
    forbidden_zero_bit = 0;
    nuh_reserved_zero_bit = 0;
    nuh_layer_id = 0;
    nal_unit_type = 0;
    nuh_temporal_id_plus1 = 0;
}


//----------------------------------------------------------------------------
// Parse the VVC access unit header.
//----------------------------------------------------------------------------

bool ts::AbstractVVCAccessUnit::parseHeader(const uint8_t*& data, size_t& size)
{
    if (data == nullptr || size < 2) {
        return false;
    }
    else {
        forbidden_zero_bit = (data[0] >> 7) & 0x01;
        nuh_reserved_zero_bit= (data[0] >> 6) & 0x01;
        nuh_layer_id = data[0] & 0x3F;
        nal_unit_type = (data[1] >> 3) & 0x1F;
        nuh_temporal_id_plus1 = data[1] & 0x07;
        data += 2;
        size -= 2;
        return true;
    }
}
