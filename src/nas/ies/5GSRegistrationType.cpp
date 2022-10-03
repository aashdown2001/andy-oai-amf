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

#include "5GSRegistrationType.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType()
    : iei_(0), follow_on_req_(false), reg_type_(0) {}

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType(bool follow_on_req, uint8_t type)
    : iei_(0) {
  follow_on_req_ = follow_on_req;
  reg_type_      = 0x07 & type;
}

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType(
    uint8_t iei, bool follow_on_req, uint8_t type) {
  iei_           = 0x0f & iei;
  follow_on_req_ = follow_on_req;
  reg_type_      = 0x07 & type;
}

//------------------------------------------------------------------------------
_5GSRegistrationType::~_5GSRegistrationType() {}

//------------------------------------------------------------------------------
int _5GSRegistrationType::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().error("Encoding 5gsregistrationtype IE");
  if (len < kType1IeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType1IeSize);
    return KEncodeDecodeError;
  }

  uint8_t octet        = 0;
  uint8_t encoded_size = 0;

  if (follow_on_req_) octet = 0b1000;
  octet |= reg_type_;

  if (!(iei_ & 0x0f)) {
    ENCODE_U8(buf, 0x0f & octet, encoded_size);
  } else {
    ENCODE_U8(buf, (iei_ << 4) | octet, encoded_size);
  }
  Logger::nas_mm().debug(
      "Encoded 5GSRegistrationType IE (%d octet))", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSRegistrationType::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  if (len < kType1IeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType1IeSize);
    return KEncodeDecodeError;
  }

  if (is_option) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug("Decoding 5GSRegistrationType");
  uint8_t decoded_size = 0;
  uint8_t octet        = 0;

  DECODE_U8(buf, octet, decoded_size);

  if (octet & 0x08)
    follow_on_req_ = FOLLOW_ON_REQ_PENDING;
  else
    follow_on_req_ = NO_FOLLOW_ON_REQ_PENDING;

  reg_type_ = 0x07 & octet;

  Logger::nas_mm().debug(
      "Decoded 5GSRegistrationType IE (%d octet)", decoded_size);
  return 0;  // to read NAS Key Set Identifier (1/2 octet)
}

//------------------------------------------------------------------------------
void _5GSRegistrationType::set(
    const bool& follow_on_req, const uint8_t& type, const uint8_t& iei) {
  follow_on_req_ = follow_on_req;
  reg_type_      = 0x07 & type;
  iei_           = 0x0f & iei;
}
//------------------------------------------------------------------------------
void _5GSRegistrationType::setFollowOnReq(const bool follow_on_req) {
  follow_on_req_ = follow_on_req;
}

//------------------------------------------------------------------------------
bool _5GSRegistrationType::isFollowOnReq() {
  return follow_on_req_;
}

//------------------------------------------------------------------------------
void _5GSRegistrationType::setRegType(const uint8_t type) {
  reg_type_ = 0x07 & type;
}

//------------------------------------------------------------------------------
uint8_t _5GSRegistrationType::getRegType() {
  return reg_type_;
}
