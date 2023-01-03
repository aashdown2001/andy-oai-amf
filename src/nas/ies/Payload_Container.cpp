/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "Payload_Container.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(uint8_t iei) {
  _iei    = iei;
  length  = 0;
  content = std::nullopt;
  CONTENT = std::nullopt;
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(uint8_t iei, bstring b) {
  _iei    = iei;
  content = std::optional<bstring>(b);
  CONTENT = std::nullopt;
  length  = blength(b);
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(
    const uint8_t iei, std::vector<PayloadContainerEntry> contents) {
  _iei    = iei;
  content = std::nullopt;

  length = 1;  // for number of entries
  // CONTENT.assign(content.begin(), content.end());
  CONTENT = std::optional<std::vector<PayloadContainerEntry>>(contents);
  for (int i = 0; i < contents.size(); i++) {
    length = length + 2 +
             contents.at(i).length;  // 2 for Length of Payload container entry
  }
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container() {
  _iei    = 0;
  length  = 0;
  content = std::nullopt;
  CONTENT = std::nullopt;
}

//------------------------------------------------------------------------------
Payload_Container::~Payload_Container() {}

//------------------------------------------------------------------------------
void Payload_Container::setValue(uint8_t iei, uint8_t value) {
  _iei = iei;
}

//------------------------------------------------------------------------------
bool Payload_Container::getValue(std::vector<PayloadContainerEntry>& content) {
  if (CONTENT.has_value()) {
    content.assign(CONTENT.value().begin(), CONTENT.value().end());
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool Payload_Container::getValue(bstring& cnt) {
  if (content.has_value()) {
    cnt = content.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
int Payload_Container::encode2Buffer(uint8_t* buf, int len, uint8_t type) {
  Logger::nas_mm().debug("Encoding Payload_Container");

  if ((len < kPayloadContainerMinimumLength) or (len < length + 3)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPayloadContainerMinimumLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U16(buf + encoded_size, length, encoded_size);

  if (content.has_value()) {
    uint8_t* buf_tmp = (uint8_t*) bdata(content.value());
    if (buf_tmp != nullptr)
      memcpy(buf + encoded_size, buf_tmp, blength(content.value()));
    encoded_size += blength(content.value());
    return encoded_size;
  }

  if (CONTENT.has_value()) {
    // Number of entries
    ENCODE_U8(buf + encoded_size, CONTENT.value().size(), encoded_size);

    for (int i = 0; i < CONTENT.value().size(); i++) {
      // Length of Payload container entry
      ENCODE_U8(buf + encoded_size, CONTENT.value().at(i).length, encoded_size);
      // Number of optional IEs and Payload container type
      uint8_t octet = 0;
      octet         = ((CONTENT.value().at(i).optionalIE.size() & 0x0f) << 4) |
              CONTENT.value().at(i).payloadContainerType;
      ENCODE_U8(buf + encoded_size, octet, encoded_size);
      // Optional IEs
      for (int j = 0; j < CONTENT.value().at(i).optionalIE.size(); j++) {
        // Type
        ENCODE_U8(
            buf + encoded_size, CONTENT.value().at(i).optionalIE.at(j).ie_type,
            encoded_size);
        // Length
        ENCODE_U8(
            buf + encoded_size, CONTENT.value().at(i).optionalIE.at(j).ie_len,
            encoded_size);
        // Value
        int size = encode_bstring(
            CONTENT.value().at(i).optionalIE.at(j).ie_value,
            (buf + encoded_size), len - encoded_size);
        encoded_size += size;
      }
    }
  }

  Logger::nas_mm().debug("Encoded Payload_Container len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Payload_Container::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option, uint8_t type) {
  Logger::nas_mm().debug("Decoding Payload_Container");

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }

  DECODE_U16(buf + decoded_size, length, decoded_size);

  if (type != MULTIPLE_PAYLOADS) {  // not multiple payloads
    uint8_t octet       = 0;
    bstring content_tmp = {};
    content_tmp         = blk2bstr(buf + decoded_size, length);
    content             = std::optional<bstring>(content_tmp);
    decoded_size += length;
    return decoded_size;
  }

  // Multiple payloads
  uint8_t num_entries;
  uint8_t num_optional;
  std::vector<PayloadContainerEntry> CONTENT_tmp;

  PayloadContainerEntry payloadcontainerentry = {};
  DECODE_U8(buf + decoded_size, num_entries, decoded_size);

  while (num_entries) {
    uint8_t length_entry = 0;
    DECODE_U8(buf + decoded_size, length_entry, decoded_size);
    uint8_t octet = 0;
    // Number of optional IEs and Payload container type
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    payloadcontainerentry.payloadContainerType = octet & 0x0f;
    num_optional                               = (octet & 0xf0) >> 4;

    IE_t value;
    while (num_optional) {
      DECODE_U8(buf + decoded_size, value.ie_type, decoded_size);
      DECODE_U8(buf + decoded_size, value.ie_len, decoded_size);

      decode_bstring(
          &value.ie_value, value.ie_len, (buf + decoded_size),
          len - decoded_size);
      decoded_size += value.ie_len;
      payloadcontainerentry.optionalIE.insert(
          payloadcontainerentry.optionalIE.end(), value);
      num_optional--;
    }
    CONTENT_tmp.insert(CONTENT_tmp.end(), payloadcontainerentry);
    num_entries--;
  }
  CONTENT = std::optional<std::vector<PayloadContainerEntry>>(CONTENT);
  Logger::nas_mm().debug("Decoded Payload_Container (len %d)", decoded_size);
  return decoded_size;
}
