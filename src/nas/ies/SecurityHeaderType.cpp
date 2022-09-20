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

#include "SecurityHeaderType.hpp"

#include "3gpp_ts24501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityHeaderType::SecurityHeaderType() {}

//------------------------------------------------------------------------------
SecurityHeaderType::~SecurityHeaderType() {}

//------------------------------------------------------------------------------
void SecurityHeaderType::Set(
    const uint8_t& secu_header_type, const uint8_t& spare) {
  secu_header_type_ = 0x0f & secu_header_type;
  spare_            = spare & 0xf0;
}

//------------------------------------------------------------------------------
void SecurityHeaderType::Get(uint8_t& secu_header_type) const {
  secu_header_type = secu_header_type_ & 0x0f;
}

//------------------------------------------------------------------------------
uint8_t SecurityHeaderType::Get() const {
  return (secu_header_type_ & 0x0f);
}

//------------------------------------------------------------------------------
uint32_t SecurityHeaderType::Encode(uint8_t* buf, uint32_t len) {
  if (len < kSecurityHeaderIeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than %d octet", kSecurityHeaderIeSize);
    return KEncodeDecodeError;
  }
  uint8_t value = (secu_header_type_ & 0x0f) | (spare_ & 0xf0);

  uint32_t encoded_size = 0;
  ENCODE_U8(buf, value, encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
uint32_t SecurityHeaderType::Decode(const uint8_t* const buf, uint32_t len) {
  if (len < kSecurityHeaderIeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than %s octet", kSecurityHeaderIeSize);
    return KEncodeDecodeError;
  }
  uint8_t value;
  uint32_t decoded_size = 0;
  DECODE_U8(buf, value, decoded_size);

  secu_header_type_ = 0x0f & value;
  spare_            = value & 0xf0;

  return decoded_size;
}
