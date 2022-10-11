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

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "UEStatus.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UEStatus::UEStatus(uint8_t iei) {
  _iei   = iei;
  length = 1;
  S1     = false;
  N1     = false;
}

//------------------------------------------------------------------------------
UEStatus::UEStatus(const uint8_t iei, bool n1, bool s1) {
  _iei   = iei;
  length = 1;
  S1     = s1;
  N1     = n1;
}

//------------------------------------------------------------------------------
UEStatus::UEStatus() {
  _iei   = 0;
  length = 1;
  S1     = false;
  N1     = false;
}

//------------------------------------------------------------------------------
UEStatus::~UEStatus() {}

//------------------------------------------------------------------------------
void UEStatus::setS1(bool value) {
  S1 = value;
}

//------------------------------------------------------------------------------
void UEStatus::setN1(bool value) {
  N1 = value;
}

//------------------------------------------------------------------------------
bool UEStatus::getS1() {
  return S1;
}

//------------------------------------------------------------------------------
bool UEStatus::getN1() {
  return N1;
}

//------------------------------------------------------------------------------
int UEStatus::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding UE Status (IEI 0x%x)", _iei);

  if ((len < kUEStatusIELength) or (len < length + 2)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUEStatusIELength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U8(buf + encoded_size, length, encoded_size);

  uint8_t octet = 0x03 & (S1 | (N1 << 1));
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug("Encoded UE Status ( len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UEStatus::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding UE Status");

  if ((len < kUEStatusIELength) or (len < length + 2)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUEStatusIELength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  N1 = octet & 0x02;
  S1 = octet & 0x01;
  Logger::nas_mm().debug(
      "Decoded UE Status, N1 0x%x, S1 0x%x, len %d", N1, S1, decoded_size);
  return decoded_size;
}
