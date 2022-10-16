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

#include "_5GS_DRX_Parameters.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GS_DRX_Parameters::_5GS_DRX_Parameters(uint8_t iei) {
  _iei   = iei;
  length = 1;
  _value = 0;
}

//------------------------------------------------------------------------------
_5GS_DRX_Parameters::_5GS_DRX_Parameters(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  length = 1;
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
_5GS_DRX_Parameters::_5GS_DRX_Parameters() {
  _iei   = 0;
  length = 1;
  _value = 0;
}
_5GS_DRX_Parameters::~_5GS_DRX_Parameters() {}

//------------------------------------------------------------------------------
void _5GS_DRX_Parameters::setValue(uint8_t value) {
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
uint8_t _5GS_DRX_Parameters::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int _5GS_DRX_Parameters::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding _5GS_DRX_Parameters iei(0x%x)", _iei);
  if (len < k5gsDrxParametersLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsDrxParametersLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  ENCODE_U8(buf + encoded_size, length, encoded_size);
  ENCODE_U8(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug("Encoded _5GS_DRX_Parameters (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_DRX_Parameters::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding _5GS_DRX_Parameters");

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);
  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _value = octet & 0x0f;

  Logger::nas_mm().debug(
      "Decoded _5GS_DRX_Parameters DRX (value 0x%x)", _value);
  Logger::nas_mm().debug("Decoded _5GS_DRX_Parameters (len %d)", decoded_size);
  return decoded_size;
}
