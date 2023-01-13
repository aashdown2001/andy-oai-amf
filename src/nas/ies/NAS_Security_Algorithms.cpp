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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "NAS_Security_Algorithms.hpp"

#include <iostream>

#include "logger.hpp"
using namespace nas;
using namespace std;

//------------------------------------------------------------------------------
NAS_Security_Algorithms::NAS_Security_Algorithms()
    : Type3NasIe(),
      type_of_ciphering_algorithm_(),
      type_of_integrity_protection_algorithm_() {
  SetIeName(kNasSecurityAlgorithmsIeName);
}

//------------------------------------------------------------------------------
NAS_Security_Algorithms::NAS_Security_Algorithms(uint8_t iei)
    : Type3NasIe(iei),
      type_of_ciphering_algorithm_(),
      type_of_integrity_protection_algorithm_() {
  SetIeName(kNasSecurityAlgorithmsIeName);
}

//------------------------------------------------------------------------------
NAS_Security_Algorithms::~NAS_Security_Algorithms() {}

//------------------------------------------------------------------------------
NAS_Security_Algorithms::NAS_Security_Algorithms(
    uint8_t ciphering, uint8_t integrity_protection)
    : Type3NasIe() {
  type_of_ciphering_algorithm_            = ciphering & 0x0f;
  type_of_integrity_protection_algorithm_ = integrity_protection & 0x0f;
  SetIeName(kNasSecurityAlgorithmsIeName);
}

//------------------------------------------------------------------------------
void NAS_Security_Algorithms::SetTypeOfCipheringAlgorithm(uint8_t value) {
  type_of_ciphering_algorithm_ = value & 0x0f;
}

//------------------------------------------------------------------------------
void NAS_Security_Algorithms::SetTypeOfIntegrityProtectionAlgorithm(
    uint8_t value) {
  type_of_integrity_protection_algorithm_ = value & 0x0f;
}

//------------------------------------------------------------------------------
uint8_t NAS_Security_Algorithms::GetTypeOfCipheringAlgorithm() const {
  return type_of_ciphering_algorithm_;
}

//------------------------------------------------------------------------------
uint8_t NAS_Security_Algorithms::GetTypeOfIntegrityProtectionAlgorithm() const {
  return type_of_integrity_protection_algorithm_;
}

//------------------------------------------------------------------------------
void NAS_Security_Algorithms::Set(
    uint8_t ciphering, uint8_t integrity_protection) {
  type_of_ciphering_algorithm_            = ciphering & 0x0f;
  type_of_integrity_protection_algorithm_ = integrity_protection & 0x0f;
}
//------------------------------------------------------------------------------
void NAS_Security_Algorithms::Get(
    uint8_t& ciphering, uint8_t& integrity_protection) const {
  ciphering            = type_of_ciphering_algorithm_;
  integrity_protection = type_of_integrity_protection_algorithm_;
}

//------------------------------------------------------------------------------
int NAS_Security_Algorithms::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < kNasSecurityAlgorithmsLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNasSecurityAlgorithmsLength);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;

  // IEI
  encoded_size += Type3NasIe::Encode(buf + encoded_size, len);

  uint8_t octet = 0;
  octet         = ((type_of_ciphering_algorithm_ & 0x0f) << 4) |
          (type_of_integrity_protection_algorithm_ & 0x0f);

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NAS_Security_Algorithms::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kNasSecurityAlgorithmsLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNasSecurityAlgorithmsLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  decoded_size += Type3NasIe::Decode(buf + decoded_size, len, is_iei);

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  type_of_ciphering_algorithm_            = (octet & 0xf0) >> 4;
  type_of_integrity_protection_algorithm_ = octet & 0x0f;

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
