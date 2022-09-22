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

#include "NasKeySetIdentifier.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier() {}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(
    const uint8_t& iei, const uint8_t& tsc, const uint8_t& key_id) {
  iei_    = iei;
  tsc_    = 0x01 & tsc;
  key_id_ = 0x07 & key_id;
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(
    const uint8_t& tsc, const uint8_t& key_id) {
  iei_    = 0;
  tsc_    = 0x01 & tsc;
  key_id_ = 0x07 & key_id;
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::~NasKeySetIdentifier(){};

//------------------------------------------------------------------------------
int NasKeySetIdentifier::encode2Buffer(uint8_t* buf, const int& len) {
  Logger::nas_mm().debug("Encoding NasKeySetIdentifier IE (IEI 0x%x)", iei_);
  if (len < kType1IeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%s octet)",
        kType1IeSize);
    return KEncodeDecodeError;
  }

  uint8_t octet         = 0;
  uint32_t encoded_size = 0;

  if (!(iei_ & 0x0f)) {
    octet = (0x0f) & ((tsc_ << 3) | key_id_);
    ENCODE_U8(buf, octet, encoded_size);
  } else {
    octet = (iei_ << 4) | (tsc_ << 3) | key_id_;
    ENCODE_U8(buf, octet, encoded_size);
  }
  Logger::nas_mm().debug(
      "Encoded NasKeySetIdentifier IE (TSC 0x%x, Key_id 0x%x)", tsc_, key_id_);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NasKeySetIdentifier::decodeFromBuffer(
    uint8_t* buf, const int& len, bool is_option, bool is_high) {
  Logger::nas_mm().debug("Decoding NasKeySetIdentifier IE");

  if (len < kType1IeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%s octet)",
        kType1IeSize);
    return KEncodeDecodeError;
  }
  uint32_t decoded_size = 0;
  uint8_t octet         = 0;
  DECODE_U8(buf, octet, decoded_size);

  if (is_option) {
    iei_ = (octet & 0xf0) >> 4;
  } else {
    iei_ = 0;
  }

  if (!is_high) {
    tsc_    = octet & 0x08;
    key_id_ = octet & 0x07;
  } else {
    tsc_    = (octet & 0x80) >> 4;
    key_id_ = (octet & 0x70) >> 4;
  }

  Logger::nas_mm().debug(
      "Decoded NasKeySetIdentifier IE (TSC 0x%x, Key_id 0x%x)", tsc_, key_id_);
  if (iei_)
    return 1;
  else
    return 0;  // 1/2 octet
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::setTypeOfSecurityContext(const uint8_t& type) {
  tsc_ = 0x01 & type;
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::setNasKeyIdentifier(const uint8_t& id) {
  key_id_ = 0x07 & id;
}

//------------------------------------------------------------------------------
uint8_t NasKeySetIdentifier::getTypeOfSecurityContext() const {
  return tsc_;
}

//------------------------------------------------------------------------------
uint8_t NasKeySetIdentifier::getNasKeyIdentifier() const {
  return key_id_;
}
