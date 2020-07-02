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

#include "tsShortNodeInformationDescriptor.h"
#include "tsDescriptor.h"
#include "tsTablesDisplay.h"
#include "tsPSIRepository.h"
#include "tsDuckContext.h"
#include "tsxmlElement.h"
TSDUCK_SOURCE;

#define MY_XML_NAME u"short_node_information_descriptor"
#define MY_CLASS ts::ShortNodeInformationDescriptor
#define MY_DID ts::DID_ISDB_SHORT_NODE_INF
#define MY_PDS ts::PDS_ISDB
#define MY_STD ts::Standards::ISDB

TS_REGISTER_DESCRIPTOR(MY_CLASS, ts::EDID::Private(MY_DID, MY_PDS), MY_XML_NAME, MY_CLASS::DisplayDescriptor);


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::ShortNodeInformationDescriptor::ShortNodeInformationDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    ISO_639_language_code(),
    node_name(),
    text()
{
}

ts::ShortNodeInformationDescriptor::ShortNodeInformationDescriptor(DuckContext& duck, const Descriptor& desc) :
    ShortNodeInformationDescriptor()
{
    deserialize(duck, desc);
}

void ts::ShortNodeInformationDescriptor::clearContent()
{
    ISO_639_language_code.clear();
    node_name.clear();
    text.clear();
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ShortNodeInformationDescriptor::serialize(DuckContext& duck, Descriptor& desc) const
{
    ByteBlockPtr bbp(serializeStart());
    if (!SerializeLanguageCode(*bbp, ISO_639_language_code)) {
        desc.invalidate();
        return;
    }
    bbp->append(duck.encodedWithByteLength(node_name));
    bbp->append(duck.encodedWithByteLength(text));
    serializeEnd(desc, bbp);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::ShortNodeInformationDescriptor::deserialize(DuckContext& duck, const Descriptor& desc)
{
    const uint8_t* data = desc.payload();
    size_t size = desc.payloadSize();

    ISO_639_language_code.clear();
    node_name.clear();
    text.clear();

    _is_valid = desc.isValid() && desc.tag() == _tag &&
        deserializeLanguageCode(ISO_639_language_code, data, size) &&
        duck.decodeWithByteLength(node_name, data, size) &&
        duck.decodeWithByteLength(text, data, size);
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ShortNodeInformationDescriptor::DisplayDescriptor(TablesDisplay& display, DID did, const uint8_t* data, size_t size, int indent, TID tid, PDS pds)
{
    DuckContext& duck(display.duck());
    std::ostream& strm(duck.out());
    const std::string margin(indent, ' ');

    if (size >= 3) {
        strm << margin << "Language: \"" << DeserializeLanguageCode(data) << "\"" << std::endl;
        data += 3; size -= 3;
        strm << margin << "Node name: \"" << duck.decodedWithByteLength(data, size) << "\"" << std::endl;
        strm << margin << "Text: \"" << duck.decodedWithByteLength(data, size) << "\"" << std::endl;
    }
    display.displayExtraData(data, size, indent);
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ShortNodeInformationDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    root->setAttribute(u"ISO_639_language_code", ISO_639_language_code);
    root->setAttribute(u"node_name", node_name, true);
    root->setAttribute(u"text", text, true);
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

bool ts::ShortNodeInformationDescriptor::analyzeXML(DuckContext& duck, const xml::Element* element)
{
    return element->getAttribute(ISO_639_language_code, u"ISO_639_language_code", true, UString(), 3, 3) &&
           element->getAttribute(node_name, u"node_name", false) &&
           element->getAttribute(text, u"text", false);
}
