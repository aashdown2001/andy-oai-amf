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

#include "UENetworkCapability.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UENetworkCapability::UENetworkCapability() : Type4NasIe() {
  eea_ = 0;
  eia_ = 0;
  SetLengthIndicator(2);
  SetIeName(kUeNetworkCapabilityIeName);
}

//------------------------------------------------------------------------------
UENetworkCapability::UENetworkCapability(uint8_t iei) : Type4NasIe(iei) {
  eea_ = 0;
  eia_ = 0;
  SetLengthIndicator(2);
  SetIeName(kUeNetworkCapabilityIeName);
}

//------------------------------------------------------------------------------
UENetworkCapability::~UENetworkCapability() {}

//------------------------------------------------------------------------------
UENetworkCapability::UENetworkCapability(
    const uint8_t iei, uint8_t eea, uint8_t eia)
    : Type4NasIe(iei) {
  eea_ = eea;
  eia_ = eia;
  SetLengthIndicator(2);
  SetIeName(kUeNetworkCapabilityIeName);
  Logger::nas_mm().debug(
      "Initialized %s EEA 0x%x, EIA 0x%x", GetIeName().c_str(), eea_, eia_);
}

//------------------------------------------------------------------------------
void UENetworkCapability::SetEea(uint8_t value) {
  eea_ = value;
}

//------------------------------------------------------------------------------
void UENetworkCapability::SetEia(uint8_t value) {
  eia_ = value;
}

//------------------------------------------------------------------------------
uint8_t UENetworkCapability::GetEea() const {
  return eea_;
}

//------------------------------------------------------------------------------
uint8_t UENetworkCapability::GetEia() const {
  return eia_;
}

//------------------------------------------------------------------------------
int UENetworkCapability::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;

  // IEI and Length
  encoded_size += Type4NasIe::Encode(buf + encoded_size, len);
  // EEA
  ENCODE_U8(buf + encoded_size, eea_, encoded_size);
  // EIA
  ENCODE_U8(buf + encoded_size, eia_, encoded_size);

  // TODO: Encode the rest as spare for now
  uint8_t spare = 0;
  for (int i = 0; i < (ie_len - encoded_size); i++) {
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UENetworkCapability::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kUeNetworkCapabilityMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUeNetworkCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);

  DECODE_U8(buf + decoded_size, eea_, decoded_size);
  DECODE_U8(buf + decoded_size, eia_, decoded_size);

  int ie_len = GetIeLength();

  // TODO: decode the rest as spare for now
  uint8_t spare = 0;
  for (int i = 0; i < (ie_len - decoded_size); i++) {
    ENCODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  Logger::nas_mm().debug("EEA 0x%x, EIA 0x%x", eea_, eia_);
  return decoded_size;
}
