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
#include "_5GMM_Cause.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GMM_Cause::_5GMM_Cause(uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
void _5GMM_Cause::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t _5GMM_Cause::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
_5GMM_Cause::_5GMM_Cause() {
  _iei   = 0;
  _value = 0;
}

//------------------------------------------------------------------------------
_5GMM_Cause::~_5GMM_Cause(){};

//------------------------------------------------------------------------------
void _5GMM_Cause::set(uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}
//------------------------------------------------------------------------------
int _5GMM_Cause::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5GMM_Cause IE ");

  int encoded_size = 0;

  if (_iei) {
    if (len < k5gmmCauseMaximumLength) {
      Logger::nas_mm().error(
          "Buffer length is less than the length of this IE (%d octet)",
          k5gmmCauseMaximumLength);
      return KEncodeDecodeError;
    }
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }
  ENCODE_U8(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug("Encoded _5GMM_Cause IE (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GMM_Cause::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding _5GMM_Cause IE");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);  // for IE
    if (len < k5gmmCauseMaximumLength) {
      Logger::nas_mm().error(
          "Buffer length is less than the length of this IE (%d octet)",
          k5gmmCauseMaximumLength);
      return KEncodeDecodeError;
    }
  } else {
    if (len < k5gmmCauseMinimumLength) {
      Logger::nas_mm().error(
          "Buffer length is less than the length of this IE (%d octet)",
          k5gmmCauseMinimumLength);
      return KEncodeDecodeError;
    }
  }

  DECODE_U8(buf + decoded_size, _value, decoded_size);

  Logger::nas_mm().debug(
      "Decoded _5GMM_Cause (len %d octet), 5G Cause 0x%x", decoded_size,
      _value);
  return decoded_size;
}
