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

#include "_5GSTrackingAreaIdentity.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "NasUtils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSTrackingAreaIdentity::_5GSTrackingAreaIdentity(uint8_t iei) {
  _iei = iei;
  mcc_ = {};
  mnc_ = {};
  tac_ = 0x0000;
}

//------------------------------------------------------------------------------
_5GSTrackingAreaIdentity::_5GSTrackingAreaIdentity(
    const uint8_t iei, const std::string& mcc, const std::string& mnc,
    const uint32_t& tac) {
  _iei = iei;
  mcc_ = mcc;
  mnc_ = mnc;
  tac_ = tac & 0x0fff;
}

//------------------------------------------------------------------------------
_5GSTrackingAreaIdentity::_5GSTrackingAreaIdentity() {
  _iei = 0;
  mcc_ = {};
  mnc_ = {};
  tac_ = 0;
}

//------------------------------------------------------------------------------
_5GSTrackingAreaIdentity::~_5GSTrackingAreaIdentity() {}

//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::setTAC(const uint32_t& value) {
  tac_ = value & 0x0fff;
}

//------------------------------------------------------------------------------
uint32_t _5GSTrackingAreaIdentity::getTAC() const {
  return tac_;
}
//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::getTAC(uint32_t& value) const {
  value = tac_;
}

//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::setMcc(const std::string& mcc) {
  mcc_ = mcc;
}

//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::getMcc(std::string& mcc) const {
  mcc = mcc_;
}

//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::setMnc(const std::string& mnc) {
  mnc_ = mnc;
}

//------------------------------------------------------------------------------
void _5GSTrackingAreaIdentity::getMnc(std::string& mnc) const {
  mnc = mnc_;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdentity::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5GSTrackingAreaIdentity (IEI 0x%x)", _iei);
  if (len < k5gsTrackingAreaIdentityLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsTrackingAreaIdentityLength);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;

  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  encoded_size += NasUtils::encodeMccMnc2Buffer(
      mcc_, mnc_, buf + encoded_size, len - encoded_size);

  ENCODE_U24(buf + encoded_size, tac_, encoded_size);

  Logger::nas_mm().debug(
      "Encoded _5GSTrackingAreaIdentity len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdentity::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding _5GSTrackingAreaIdentity");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
    Logger::nas_mm().debug(
        "Decoding _5GSTrackingAreaIdentity IEI (0x%x)", _iei);
  }
  decoded_size += NasUtils::decodeMccMncFromBuffer(
      mcc_, mnc_, buf + decoded_size, len - decoded_size);

  DECODE_U24(buf + decoded_size, tac_, decoded_size);

  Logger::nas_mm().debug("Decoded TAC 0x%x", tac_);
  Logger::nas_mm().debug(
      "Decoded _5GSTrackingAreaIdentity len(%d)", decoded_size);
  return decoded_size;
}
