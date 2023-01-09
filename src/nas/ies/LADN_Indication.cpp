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

#include "LADN_Indication.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
LADN_Indication::LADN_Indication(uint8_t iei) {
  _iei   = iei;
  length = 0;
  LADN   = {};
}

//------------------------------------------------------------------------------
LADN_Indication::LADN_Indication(const uint8_t iei, std::vector<bstring> ladn) {
  _iei   = iei;
  length = 3;
  LADN.assign(ladn.begin(), ladn.end());
  for (int i = 0; i < ladn.size(); i++) {
    length = length + blength(ladn.at(i));
  }
}

//------------------------------------------------------------------------------
LADN_Indication::LADN_Indication() : _iei(), length(), LADN() {}

//------------------------------------------------------------------------------
LADN_Indication::~LADN_Indication() {}

//------------------------------------------------------------------------------
void LADN_Indication::setValue(uint8_t iei) {
  _iei = iei;
}

//------------------------------------------------------------------------------
void LADN_Indication::getValue(std::vector<bstring>& ladn) {
  ladn.assign(LADN.begin(), LADN.end());
}

//------------------------------------------------------------------------------
int LADN_Indication::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding LADN_Indication (IEI 0x%x)", _iei);

  if ((len < length + 3) or (len < kLadnIndicationMinimumLength)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE");
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U16(buf + encoded_size, length, encoded_size);
  for (int i = 0; i < LADN.size(); i++) {
    ENCODE_U8(buf + encoded_size, blength(LADN.at(i)), encoded_size);
    encoded_size +=
        encode_bstring(LADN.at(i), (buf + encoded_size), len - encoded_size);
  }

  Logger::nas_mm().debug("Encoded LADN_Indication (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int LADN_Indication::Decode(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding LADN_Indication");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  length = 0;
  DECODE_U16(buf + decoded_size, length, decoded_size);
  Logger::nas_mm().debug("Decoded LADN_Indication (len %d)", length);
  int len_ie      = length;
  uint8_t len_dnn = 0;
  bstring dnn;
  while (len_ie) {
    DECODE_U8(buf + decoded_size, len_dnn, decoded_size);
    len_ie--;
    decode_bstring(&dnn, len_dnn, (buf + decoded_size), len - decoded_size);
    decoded_size += len_dnn;
    len_ie -= len_dnn;
    LADN.insert(LADN.end(), dnn);
  }

  for (int i = 0; i < LADN.size(); i++) {
    for (int j = 0; j < blength(LADN.at(i)); j++) {
      Logger::nas_mm().debug(
          "Decoded LADN_Indication value (0x%x)",
          (uint8_t) LADN.at(i)->data[j]);
    }
  }

  Logger::nas_mm().debug("Decoded LADN_Indication (len %d)", decoded_size);
  return decoded_size;
}
