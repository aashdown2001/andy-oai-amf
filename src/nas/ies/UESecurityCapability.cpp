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

#include "UESecurityCapability.hpp"

#include "logger.hpp"
#include "3gpp_24.501.hpp"
#include "common_defs.h"

using namespace nas;

//------------------------------------------------------------------------------
UESecurityCapability::UESecurityCapability(uint8_t iei) {
  _iei      = iei;
  _5g_EASel = 0;
  _5g_IASel = 0;
  EEASel    = 0;
  EIASel    = 0;
  length    = 0;
}

//------------------------------------------------------------------------------
UESecurityCapability::UESecurityCapability() {
  _iei      = 0;
  _5g_EASel = 0;
  _5g_IASel = 0;
  EEASel    = 0;
  EIASel    = 0;
  length    = 0;
}

//------------------------------------------------------------------------------
UESecurityCapability::~UESecurityCapability() {}

//------------------------------------------------------------------------------
UESecurityCapability::UESecurityCapability(
    const uint8_t iei, uint8_t _5gg_EASel, uint8_t _5gg_IASel) {
  _iei      = iei;
  _5g_EASel = _5gg_EASel;
  _5g_IASel = _5gg_IASel;
  EEASel    = 0;
  EIASel    = 0;
  length    = kUESecurityCapabilityMinimumLength;
}

//------------------------------------------------------------------------------
UESecurityCapability::UESecurityCapability(
    const uint8_t iei, uint8_t _5gg_EASel, uint8_t _5gg_IASel, uint8_t _EEASel,
    uint8_t _EIASel) {
  _iei      = iei;
  _5g_EASel = _5gg_EASel;
  _5g_IASel = _5gg_IASel;
  EEASel    = _EEASel;
  EIASel    = _EIASel;
  length    = kUESecurityCapabilityMaximumLength;
}

//------------------------------------------------------------------------------
void UESecurityCapability::setEASel(uint8_t sel) {
  _5g_EASel = sel;
}

//------------------------------------------------------------------------------
void UESecurityCapability::setIASel(uint8_t sel) {
  _5g_IASel = sel;
}

//------------------------------------------------------------------------------
void UESecurityCapability::setEEASel(uint8_t sel) {
  EEASel = sel;
}

//------------------------------------------------------------------------------
void UESecurityCapability::setEIASel(uint8_t sel) {
  EIASel = sel;
}

//------------------------------------------------------------------------------
uint8_t UESecurityCapability::getEASel() {
  return _5g_EASel;
}

//------------------------------------------------------------------------------
uint8_t UESecurityCapability::getIASel() {
  return _5g_IASel;
}

//------------------------------------------------------------------------------
uint8_t UESecurityCapability::getEEASel() {
  return EEASel;
}

//------------------------------------------------------------------------------
uint8_t UESecurityCapability::getEIASel() {
  return EIASel;
}

//------------------------------------------------------------------------------
void UESecurityCapability::setLength(uint8_t len) {
  if ((len >= kUESecurityCapabilityMinimumLength) &&
      (len <= kUESecurityCapabilityMaximumLength)) {
    length = len;
  } else {
    Logger::nas_mm().debug("Set UESecurityCapability Length fail %d", len);
    Logger::nas_mm().debug(
        "UESecurityCapability Length is set to the default value %d", length);
  }
}

//------------------------------------------------------------------------------
uint8_t UESecurityCapability::getLength() {
  return length;
}

//------------------------------------------------------------------------------
int UESecurityCapability::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding UESecurityCapability IEI 0x%x", _iei);
  if (len < (length + 2)) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        length + 2);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  // Length
  ENCODE_U8(buf + encoded_size, length, encoded_size);
  // EA
  ENCODE_U8(buf + encoded_size, _5g_EASel, encoded_size);
  // IA
  ENCODE_U8(buf + encoded_size, _5g_IASel, encoded_size);

  if (length == 4) {
    // EEA
    ENCODE_U8(buf + encoded_size, EEASel, encoded_size);
    // EIA
    ENCODE_U8(buf + encoded_size, EIASel, encoded_size);
  }

  Logger::nas_mm().debug("Encoded UESecurityCapability (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UESecurityCapability::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding UESecurityCapability");

  if (len < kUESecurityCapabilityMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUESecurityCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }

  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);
  // EA
  DECODE_U8(buf + decoded_size, _5g_EASel, decoded_size);
  // IA
  DECODE_U8(buf + decoded_size, _5g_IASel, decoded_size);

  if (length >= 4) {
    // EEA
    DECODE_U8(buf + decoded_size, EEASel, decoded_size);
    // EIA
    DECODE_U8(buf + decoded_size, EIASel, decoded_size);
    decoded_size += (length - 4);  // TODO: Spare
  }
  Logger::nas_mm().debug(
      "UESecurityCapability (length %d) EA 0x%x,IA 0x%x, EEA 0x%x, EIA 0x%x,",
      length, _5g_EASel, _5g_IASel, EEASel, EIASel);
  return decoded_size;
}
