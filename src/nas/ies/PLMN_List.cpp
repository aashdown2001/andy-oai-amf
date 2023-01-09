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

#include "PLMN_List.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "NasUtils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
PLMN_List::PLMN_List(uint8_t iei) {
  _iei   = iei;
  length = 2;
}

//------------------------------------------------------------------------------
PLMN_List::PLMN_List() {
  _iei   = 0;
  length = 2;
}

//------------------------------------------------------------------------------
PLMN_List::~PLMN_List() {}

//------------------------------------------------------------------------------
void PLMN_List::set(uint8_t iei, const std::vector<nas_plmn_t>& list) {
  _iei      = iei;
  plmn_list = list;
  if (list.size() > 0)
    length =
        kPlmnListMinimumLength +
        (list.size() - 1) *
            3;  // 3 - size of each PLMN
                // size of the first PLMN is included in kPlmnListMinimumLength
}

//------------------------------------------------------------------------------
void PLMN_List::getPLMNList(std::vector<nas_plmn_t>& list) {
  list = plmn_list;
}

//------------------------------------------------------------------------------
int PLMN_List::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding PLMN_List");

  if (len < length) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)", length);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);  // IEI
  }
  // Length
  ENCODE_U8(buf + encoded_size, length, encoded_size);

  for (auto it : plmn_list)
    encoded_size += NasUtils::encodeMccMnc2Buffer(
        it.mcc, it.mnc, buf + encoded_size, len - encoded_size);

  Logger::nas_mm().debug("Encoded PLMN_List (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PLMN_List::Decode(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding PLMN_List");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);  // IEI
  }
  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);
  uint8_t len_ie = length;
  while (len_ie > 0) {
    nas_plmn_t nas_plmn = {};
    uint8_t size        = NasUtils::decodeMccMncFromBuffer(
        nas_plmn.mcc, nas_plmn.mnc, buf + decoded_size, len - decoded_size);
    if (size > 0) {
      len_ie -= size;
      plmn_list.push_back(nas_plmn);
    } else {
      break;
    }
  }
  Logger::nas_mm().debug("Decoded PLMN_List (len %d)", decoded_size);
  return decoded_size;
}
