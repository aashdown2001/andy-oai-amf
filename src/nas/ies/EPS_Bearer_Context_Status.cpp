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

#include "EPS_Bearer_Context_Status.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
EPS_Bearer_Context_Status::EPS_Bearer_Context_Status(uint8_t iei) {
  _iei   = iei;
  length = 0;
  _value = 0;
}

//------------------------------------------------------------------------------
EPS_Bearer_Context_Status::EPS_Bearer_Context_Status(
    const uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
  length = kEpsBearerContextStatusLength;
}

//------------------------------------------------------------------------------
EPS_Bearer_Context_Status::EPS_Bearer_Context_Status()
    : _iei(), length(), _value() {}

//------------------------------------------------------------------------------
EPS_Bearer_Context_Status::~EPS_Bearer_Context_Status() {}

//------------------------------------------------------------------------------
void EPS_Bearer_Context_Status::setValue(uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
uint16_t EPS_Bearer_Context_Status::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int EPS_Bearer_Context_Status::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding EPS_Bearer_Context_Status");
  if (len < kEpsBearerContextStatusLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kEpsBearerContextStatusLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  // Length
  ENCODE_U8(buf + encoded_size, length, encoded_size);
  // Value
  ENCODE_U16(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug(
      "encoded EPS_Bearer_Context_Status (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EPS_Bearer_Context_Status::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding EPS_Bearer_Context_Status");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);  // for IE
  }
  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);  // for IE
  // Value
  DECODE_U16(buf + decoded_size, _value, decoded_size);  // for IE

  Logger::nas_mm().debug(
      "decoded EPS_Bearer_Context_Status, value 0x%4x", _value);
  Logger::nas_mm().debug(
      "decoded EPS_Bearer_Context_Status (len %d)", decoded_size);
  return decoded_size;
}
