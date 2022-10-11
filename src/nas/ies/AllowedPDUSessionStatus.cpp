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

#include "AllowedPDUSessionStatus.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AllowedPDUSessionStatus::AllowedPDUSessionStatus(uint8_t iei) {
  _iei   = iei;
  length = 0;
  _value = 0;
}

//------------------------------------------------------------------------------
AllowedPDUSessionStatus::AllowedPDUSessionStatus(
    const uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
  length = 4;
}

//------------------------------------------------------------------------------
AllowedPDUSessionStatus::AllowedPDUSessionStatus()
    : _iei(), length(), _value() {}

//------------------------------------------------------------------------------
AllowedPDUSessionStatus::~AllowedPDUSessionStatus() {}

//------------------------------------------------------------------------------
void AllowedPDUSessionStatus::setValue(uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
}
uint16_t AllowedPDUSessionStatus::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int AllowedPDUSessionStatus::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AllowedPDUSessionStatus (IEI 0x%x)", _iei);

  if ((len < kAllowedPDUSessionStatusMinimumLength) or (len < length + 2)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAllowedPDUSessionStatusMinimumLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U8(buf + encoded_size, length, encoded_size);
  ENCODE_U16(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug(
      "Encoded AllowedPDUSessionStatus (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AllowedPDUSessionStatus::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding AllowedPDUSessionStatus");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);
  DECODE_U16(buf + decoded_size, _value, decoded_size);

  Logger::nas_mm().debug(
      "Decoded AllowedPDUSessionStatus (value 0x%4x)", _value);
  Logger::nas_mm().debug(
      "Decoded AllowedPDUSessionStatus (len %d)", decoded_size);
  return decoded_size;
}
