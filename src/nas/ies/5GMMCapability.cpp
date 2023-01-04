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

#include "5GMMCapability.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GMMCapability::_5GMMCapability(const uint8_t iei, uint8_t octet3) {
  iei_    = iei;
  octet3_ = octet3;
  octet4_ = std::nullopt;
  octet5_ = std::nullopt;
  length  = 1;
}

//------------------------------------------------------------------------------
_5GMMCapability::_5GMMCapability() {}

//------------------------------------------------------------------------------
_5GMMCapability::~_5GMMCapability() {}

//------------------------------------------------------------------------------
void _5GMMCapability::setOctet3(const uint8_t iei, uint8_t octet3) {
  iei_    = iei;
  octet3_ = octet3;
}

//------------------------------------------------------------------------------
uint8_t _5GMMCapability::getOctet3() const {
  return octet3_;
}

//------------------------------------------------------------------------------
int _5GMMCapability::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5GMMCapability IEI");
  int ie_len = iei_ ? (length + 2) : (length + 1);  // 1 for IEI, 1 for Length

  if (len < ie_len) {
    Logger::nas_mm().error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI
  if (iei_) {
    ENCODE_U8(buf + encoded_size, iei_, encoded_size);
  }
  // Length
  ENCODE_U8(buf + encoded_size, length, encoded_size);
  // Octet 3
  ENCODE_U8(buf + encoded_size, octet3_, encoded_size);
  // TODO: Encode spare for the rest
  uint8_t spare = 0;
  int spare_len = ie_len - encoded_size;
  for (int i = 0; i < spare_len; i++) {
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  Logger::nas_mm().debug("Encoded _5GMMCapability len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GMMCapability::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  if (len < k5gmmCapabilityMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gmmCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;

  Logger::nas_mm().debug("Decoding _5GMMCapability IE");
  if (is_option) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    iei_ = std::optional<uint8_t>(octet);
  }

  DECODE_U8(buf + decoded_size, length, decoded_size);

  DECODE_U8(buf + decoded_size, octet3_, decoded_size);
  // TODO: decode the rest as spare for now
  uint8_t spare = 0;
  for (int i = 0; i < (length - 1); i++) {
    ENCODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug(
      "Decoded _5GMMCapability Octet3 value (0x%x)", octet3_);
  Logger::nas_mm().debug("Decoded _5GMMCapability len (%d)", decoded_size);
  return decoded_size;
}
