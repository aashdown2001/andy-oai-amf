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
#include "MICOIndication.hpp"

using namespace nas;

//------------------------------------------------------------------------------
MICOIndication::MICOIndication(const uint8_t _iei, bool sprti, bool raai) {
  iei   = _iei;
  RAAI  = raai;
  SPRTI = sprti;
}

//------------------------------------------------------------------------------
MICOIndication::MICOIndication(bool sprti, bool raai) {
  this->iei   = 0;
  this->RAAI  = raai;
  this->SPRTI = sprti;
}

//------------------------------------------------------------------------------
MICOIndication::MICOIndication() {
  iei   = 0;
  RAAI  = false;
  SPRTI = false;
}

//------------------------------------------------------------------------------
MICOIndication::~MICOIndication(){};

//------------------------------------------------------------------------------
int MICOIndication::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding MICOIndication IE ( IEI 0x%x)", iei);

  if (len < kMICOIndicationIELength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kMICOIndicationIELength);
    return KEncodeDecodeError;
  }

  uint8_t octet    = 0;
  int encoded_size = 0;

  octet = (iei << 4) | (SPRTI << 1) | RAAI;
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded MICOIndication IE (len: %d octet)", encoded_size);

  return encoded_size;
}

//------------------------------------------------------------------------------
int MICOIndication::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding MICOIndication IE");
  if (len < kMICOIndicationIELength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kMICOIndicationIELength);
    return KEncodeDecodeError;
  }

  uint8_t octet    = 0;
  int decoded_size = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  if (is_option) {
    iei = (octet & 0xf0) >> 4;
  } else {
    iei = 0;
  }

  SPRTI = octet & 0x02;
  RAAI  = octet & 0x01;
  Logger::nas_mm().debug(
      "Decoded MICOIndication IEI 0x%x, SPRTI 0x%x, RAAI 0x%x", iei, SPRTI,
      RAAI);
  return decoded_size;
}

//------------------------------------------------------------------------------
void MICOIndication::setSPRTI(bool value) {
  SPRTI = value;
}

//------------------------------------------------------------------------------
void MICOIndication::setRAAI(bool value) {
  RAAI = value;
}

//------------------------------------------------------------------------------
bool MICOIndication::getSPRTI() {
  return SPRTI;
}

//------------------------------------------------------------------------------
bool MICOIndication::getRAAI() {
  return RAAI;
}
