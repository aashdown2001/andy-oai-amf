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

#include "_5GS_Update_Type.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GS_Update_Type::_5GS_Update_Type(uint8_t iei) {
  _iei          = iei;
  EPS_PNB_CIoT  = 0;
  _5GS_PNB_CIoT = 0;
  NG_RAN        = false;
  SMS           = false;
  length        = 0;
}

//------------------------------------------------------------------------------
_5GS_Update_Type::_5GS_Update_Type() {
  _iei          = 0;
  EPS_PNB_CIoT  = 0;
  _5GS_PNB_CIoT = 0;
  NG_RAN        = false;
  SMS           = false;
  length        = 0;
}

//------------------------------------------------------------------------------
_5GS_Update_Type::~_5GS_Update_Type() {}

//------------------------------------------------------------------------------
_5GS_Update_Type::_5GS_Update_Type(
    const uint8_t iei, uint8_t eps_PNB_CIoT, uint8_t _5gs_PNB_CIoT, bool ng_RAN,
    bool sms) {
  _iei          = iei;
  EPS_PNB_CIoT  = eps_PNB_CIoT;
  _5GS_PNB_CIoT = _5gs_PNB_CIoT;
  NG_RAN        = ng_RAN;
  SMS           = sms;
  length        = k5gsUpdateTypeLength;
}

//------------------------------------------------------------------------------
void _5GS_Update_Type::setEPS_PNB_CIoT(uint8_t value) {
  EPS_PNB_CIoT = value & 0x03;  // 2 bits
}

//------------------------------------------------------------------------------
void _5GS_Update_Type::set_5GS_PNB_CIoT(uint8_t value) {
  _5GS_PNB_CIoT = value & 0x03;  // 2 bits
}

//------------------------------------------------------------------------------
void _5GS_Update_Type::setNG_RAN(uint8_t value) {
  NG_RAN = value & 0x01;  // 1 bit
}

//------------------------------------------------------------------------------
void _5GS_Update_Type::setSMS(uint8_t value) {
  SMS = value & 0x01;  // 1 bit
}

//------------------------------------------------------------------------------
uint8_t _5GS_Update_Type::getEPS_PNB_CIoT() {
  return EPS_PNB_CIoT;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Update_Type::get_5GS_PNB_CIoT() {
  return _5GS_PNB_CIoT;
}

//------------------------------------------------------------------------------
bool _5GS_Update_Type::getNG_RAN() {
  return NG_RAN;
}

//------------------------------------------------------------------------------
bool _5GS_Update_Type::getSMS() {
  return SMS;
}

//------------------------------------------------------------------------------
int _5GS_Update_Type::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5GS_Update_Type");
  if (len < k5gsUpdateTypeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsUpdateTypeLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  ENCODE_U8(buf + encoded_size, length, encoded_size);

  uint8_t octet = 0;
  octet = (EPS_PNB_CIoT << 4) | (_5GS_PNB_CIoT << 2) | (NG_RAN << 1) | SMS;
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug("encoded _5GS_Update_Type (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_Update_Type::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding _5GS_Update_Type");

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  EPS_PNB_CIoT  = (octet & 0x30) >> 4;
  _5GS_PNB_CIoT = (octet & 0x0c) >> 2;
  NG_RAN        = (octet & 0x02) >> 1;
  SMS           = (octet & 0x01);
  Logger::nas_mm().debug(
      "decoded _5GS_Update_Type, "
      "EPS_PNB_CIoT 0x%x, _5GS_PNB_CIoT 0x%x, NG_RAN 0x%x, SMS 0x%x",
      EPS_PNB_CIoT, _5GS_PNB_CIoT, NG_RAN, SMS);
  Logger::nas_mm().debug("Decoded _5GS_Update_Type (len %d)", decoded_size);
  return decoded_size;
}
