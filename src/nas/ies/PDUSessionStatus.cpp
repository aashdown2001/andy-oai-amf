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

#include "PDUSessionStatus.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
PDUSessionStatus::PDUSessionStatus(uint8_t iei) {
  _iei   = iei;
  _value = 0;
  length = 2;
}

//------------------------------------------------------------------------------
PDUSessionStatus::PDUSessionStatus(const uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
  length = 2;  // Lengh of IE contents
}

//------------------------------------------------------------------------------
PDUSessionStatus::PDUSessionStatus() {
  _iei   = 0;
  _value = 0;
  length = 2;
}

//------------------------------------------------------------------------------
PDUSessionStatus::~PDUSessionStatus() {}

//------------------------------------------------------------------------------
void PDUSessionStatus::setValue(uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
uint16_t PDUSessionStatus::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int PDUSessionStatus::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding PDUSessionStatus");

  if ((len < kPduSessionStatusMinimumLength) or (len < length + 2)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPduSessionStatusMinimumLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U8(buf + encoded_size, length, encoded_size);
  ENCODE_U16(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug("Encoded PDUSessionStatus, len %d", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PDUSessionStatus::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding PDUSessionStatus");

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);
  DECODE_U16(buf + decoded_size, _value, decoded_size);

  Logger::nas_mm().debug(
      "Decoded PDUSessionStatus value 0x%x, len %d", _value, decoded_size);
  return decoded_size;
}
