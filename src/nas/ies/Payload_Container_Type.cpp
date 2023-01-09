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

#include "Payload_Container_Type.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
PayloadContainerType::PayloadContainerType(const uint8_t iei, uint8_t value)
    : Type1NasIeFormatTv(iei) {
  SetValue(value & 0x0f);
  SetIeName(kPayloadContainerTypeIeName);
}

//------------------------------------------------------------------------------
PayloadContainerType::PayloadContainerType() : Type1NasIeFormatTv() {
  SetIeName(kPayloadContainerTypeIeName);
}

//------------------------------------------------------------------------------
PayloadContainerType::~PayloadContainerType(){};

/*
//------------------------------------------------------------------------------
void PayloadContainerType::SetValue(const uint8_t value) {
  _value = value & 0x0f;
}

//------------------------------------------------------------------------------
uint8_t PayloadContainerType::GetValue() const {
  return _value;
}

//------------------------------------------------------------------------------
int PayloadContainerType::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Payload_Container_Type IE");

  if (len < kPayloadContainerTypeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPayloadContainerTypeLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (_iei) {
    octet = (_iei << 4) | (_value & 0x0f);
  } else {
    octet = (_value & 0x0f);
  }
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug("Encoded Payload_Container_Type IE");
  return encoded_size;
}

//------------------------------------------------------------------------------
int PayloadContainerType::Decode(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding Payload_Container_Type IE");

  if (len < kPayloadContainerTypeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPayloadContainerTypeLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  uint8_t octet    = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  if (is_option) {
    _iei = (octet & 0xf0) >> 4;
  }
  _value = octet & 0x0f;

  Logger::nas_mm().debug(
      "Decoded Payload_Container_Type (IEI 0x%x, value 0x%x)", _iei, _value);
  return decoded_size;
}
*/
