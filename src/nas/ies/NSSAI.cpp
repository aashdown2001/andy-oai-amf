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

#include "NSSAI.hpp"

#include <vector>

#include "logger.hpp"
#include "amf.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NSSAI::NSSAI(uint8_t iei) {
  _iei    = iei;
  length  = 0;
  S_NSSAI = {};
}

//------------------------------------------------------------------------------
NSSAI::NSSAI(const uint8_t iei, std::vector<struct SNSSAI_s> nssai) {
  _iei   = iei;
  length = 0;
  S_NSSAI.assign(nssai.begin(), nssai.end());
  for (int i = 0; i < nssai.size(); i++) {
    length += nssai[i].Length() + 1 + 1;  // 1 for IEI, 1 for length
  }
}

//------------------------------------------------------------------------------
NSSAI::NSSAI() : _iei(), length(), S_NSSAI() {}

//------------------------------------------------------------------------------
NSSAI::~NSSAI() {}

//------------------------------------------------------------------------------
void NSSAI::setS_NAASI(uint8_t SST) {}

//------------------------------------------------------------------------------
void NSSAI::GetValue(std::vector<struct SNSSAI_s>& nssai) const {
  nssai.assign(S_NSSAI.begin(), S_NSSAI.end());
}

//------------------------------------------------------------------------------
int NSSAI::Encode2Buffer(
    uint8_t* buf, int len, const bool is_option = true) const {
  Logger::nas_mm().debug("Encoding NSSAI IEI (0x%x)", _iei);
  int encoded_size = 0;
  if (is_option) {
    if (len < length + 2) {
      Logger::nas_mm().error("len is less than %d", length + 2);
      return 0;
    }
    if (_iei) {
      *(buf + encoded_size) = _iei;
      encoded_size++;
    } else {
      return 0;
    }
  } else {
    if (len < length + 1) {
      Logger::nas_mm().error("len is less than %d", length + 1);
      return 0;
    }
  }

  *(buf + encoded_size) = length;
  encoded_size++;

  len -= encoded_size;
  for (int i = 0; i < S_NSSAI.size(); i++) {
    int encoded_size_ie =
        S_NSSAI.at(i).Encode2Buffer((buf + encoded_size), len, is_option);
    len -= encoded_size_ie;
  }
  Logger::nas_mm().debug("Encoded NSSAI len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NSSAI::DecodeFromBuffer(uint8_t* buf, int len, const bool is_option) {
  Logger::nas_mm().debug("Decoding NSSAI IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  int length_tmp = length;

  while (length_tmp > 0) {
    struct SNSSAI_s a;
    int decoded_size_ie =
        a.DecodeFromBuffer(buf + decoded_size, len, is_option);
    if (decoded_size_ie > 0) {  // if error
      length_tmp -= decoded_size_ie;
      len -= decoded_size_ie;
      S_NSSAI.insert(S_NSSAI.end(), a);
    } else {
      break;
    }
  }

  for (int i = 0; i < S_NSSAI.size(); i++) {
    Logger::nas_mm().debug("Decoded NSSAI %s", S_NSSAI.at(i).ToString());
  }
  Logger::nas_mm().debug("Decoded NSSAI len (%d)", decoded_size);
  return decoded_size;
}
