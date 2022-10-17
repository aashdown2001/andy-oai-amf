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

#include "NetworkSlicingIndication.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication(uint8_t iei) {
  _iei  = iei;
  DCNI  = false;
  NSSCI = false;
}

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication(
    const uint8_t iei, bool dcni, bool nssci) {
  _iei  = iei;
  DCNI  = dcni;
  NSSCI = nssci;
}

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication()
    : _iei(), DCNI(), NSSCI() {}

//------------------------------------------------------------------------------
NetworkSlicingIndication::~NetworkSlicingIndication() {}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::setDCNI(bool value) {
  DCNI = value;
}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::setNSSCI(bool value) {
  NSSCI = value;
}

//------------------------------------------------------------------------------
bool NetworkSlicingIndication::getDCNI() {
  return DCNI;
}

//------------------------------------------------------------------------------
bool NetworkSlicingIndication::getNSSCI() {
  return NSSCI;
}

//------------------------------------------------------------------------------
int NetworkSlicingIndication::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Network Slicing Indication");

  if (len < kNetworkSlicingIndicationLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNetworkSlicingIndicationLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (_iei) {
    octet = (_iei << 4) | (DCNI << 1) | NSSCI;
  } else {
    octet = 0x0f & ((DCNI << 1) | NSSCI);
  }

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded NetworkSlicingIndication DCNI (0x%x) NSSCI (0x%x)", DCNI, NSSCI);

  Logger::nas_mm().debug(
      "Encoded NetworkSlicingIndication IE (len, %d octet)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NetworkSlicingIndication::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding Network Slicing Indication");

  if (len < kNetworkSlicingIndicationLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNetworkSlicingIndicationLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  uint8_t octet    = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  if (is_option) {
    _iei = (octet & 0xf0) >> 4;
  }
  DCNI  = octet & 0x02;
  NSSCI = octet & 0x01;

  Logger::nas_mm().debug(
      "Decoded NetworkSlicingIndication DCNI (0x%x) NSSCI (0x%x)", DCNI, NSSCI);
  return decoded_size;
}
