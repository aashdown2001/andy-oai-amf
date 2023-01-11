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
#include "Authentication_Parameter_RAND.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND()
    : Type3NasIe(), _value() {
  SetIeName(kAuthenticationParameterRandIeName);
}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND(uint8_t iei)
    : Type3NasIe(iei), _value() {
  SetIeName(kAuthenticationParameterRandIeName);
}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND(
    uint8_t iei, uint8_t* value)
    : Type3NasIe(iei) {
  for (int i = 0; i < 16; i++) {
    this->_value[i] = value[i];
  }
  SetIeName(kAuthenticationParameterRandIeName);
}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::~Authentication_Parameter_RAND() {}

/*
//------------------------------------------------------------------------------
uint8_t* Authentication_Parameter_RAND::getValue() {
  return _value;
}
*/

//------------------------------------------------------------------------------
int Authentication_Parameter_RAND::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < kAuthenticationParameterRandLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAuthenticationParameterRandLength);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;

  // IEI
  encoded_size += Type3NasIe::Encode(buf + encoded_size, len);
  for (int i = 0; i < kAuthenticationParameterRandLength - 1; i++) {
    ENCODE_U8(buf + encoded_size, _value[i], encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Authentication_Parameter_RAND::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kAuthenticationParameterRandLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAuthenticationParameterRandLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  decoded_size += Type3NasIe::Decode(buf + decoded_size, len, is_iei);

  for (int i = 0; i < kAuthenticationParameterRandLength - 1; i++) {
    DECODE_U8(buf + decoded_size, _value[i], decoded_size);
  }

  for (int j = 0; j < kAuthenticationParameterRandLength - 1; j++) {
    Logger::nas_mm().debug(
        "Decoded Authentication_Parameter_RAND value (0x%2x)", _value[j]);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
