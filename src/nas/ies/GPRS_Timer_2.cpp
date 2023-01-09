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

#include "GPRS_Timer_2.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
GPRS_Timer_2::GPRS_Timer_2(uint8_t iei) {
  _iei   = iei;
  length = 1;
  _value = 0;
}

//------------------------------------------------------------------------------
GPRS_Timer_2::GPRS_Timer_2(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
  length = 1;
}

//------------------------------------------------------------------------------
GPRS_Timer_2::GPRS_Timer_2() : _iei(), _value(), length(1) {}

//------------------------------------------------------------------------------
GPRS_Timer_2::~GPRS_Timer_2() {}

//------------------------------------------------------------------------------
void GPRS_Timer_2::setIEI(uint8_t iei) {
  _iei = iei;
}

//------------------------------------------------------------------------------
void GPRS_Timer_2::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t GPRS_Timer_2::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int GPRS_Timer_2::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding GPRS_Timer_2");
  if (len < kGprsTimer2Length) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)",
        kGprsTimer2Length);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  // Length
  length = 1;
  ENCODE_U8(buf + encoded_size, length, encoded_size);
  // Value
  ENCODE_U8(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug("encoded GPRS_Timer_2, value 0x%x", _value);

  Logger::nas_mm().debug("encoded GPRS_Timer_2 ( len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int GPRS_Timer_2::Decode(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding GPRS_Timer_2");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);
  DECODE_U8(buf + decoded_size, _value, decoded_size);
  Logger::nas_mm().debug(
      "Decoded GPRS_Timer_2, value 0x%x, IEI 0x%x", _value, _iei);
  Logger::nas_mm().debug("decoded GPRS_Timer_2 (len %d)", decoded_size);
  return decoded_size;
}
