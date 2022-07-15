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

#include "S_NSSAI.hpp"
#include "Ie_Const.hpp"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
S_NSSAI::S_NSSAI(uint8_t iei) {
  _iei   = iei;
  length = 0;
  SNSSAI = {};
}

//------------------------------------------------------------------------------
S_NSSAI::S_NSSAI(const uint8_t iei, SNSSAI_s snssai) {
  _iei   = iei;
  length = 1;
  SNSSAI = snssai;
  length += snssai.Length();
}

//------------------------------------------------------------------------------
S_NSSAI::S_NSSAI() : _iei(), length(), SNSSAI{} {}

//------------------------------------------------------------------------------
S_NSSAI::~S_NSSAI() {}

//------------------------------------------------------------------------------
void S_NSSAI::SetSnssai(SNSSAI_s snssai) {
  SNSSAI = snssai;
}

//------------------------------------------------------------------------------
void S_NSSAI::GetValue(SNSSAI_s& snssai) {
  snssai = SNSSAI;
}

//------------------------------------------------------------------------------
int S_NSSAI::Encode2Buffer(
    uint8_t* buf, int len, const bool is_option = true) const {
  Logger::nas_mm().debug("Encoding S_NSSAI IEI (0x%x)", kIeiSNssai);
  // 2 = size of IE encoded + length encoded
  int encoded_size = 0;
  if (is_option) {
    if (len < (SNSSAI.Length() + 1)) {
      Logger::nas_mm().error(
          "Out of buffer space for encoding len %d is less than %u", len,
          SNSSAI.Length() + 1);
      return 0;
    }
    if (_iei) {
      *(buf + encoded_size) = _iei;
      encoded_size++;
      len -= 1;
    } else {
      Logger::nas_mm().error("Encoding S_NSSAI IEI is 0!");
      return 0;
    }
  } else if (len < SNSSAI.Length()) {
    Logger::nas_mm().error(
        "Out of buffer space for encoding len %d is less than %u", len,
        SNSSAI.Length());
    return 0;
  }
  encoded_size += SNSSAI.Encode2Buffer(buf, len, is_option);
  Logger::nas_mm().debug("encoded S_NSSAI len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int S_NSSAI::DecodeFromBuffer(uint8_t* buf, int len, const bool is_option) {
  Logger::nas_mm().debug("decoding S_NSSAI IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (len >= SNSSAI_t::kSNSSAIMinSize) {
    if (is_option) {
      _iei = *(buf + decoded_size);
      decoded_size++;
      len -= 1;
    }
    decoded_size += SNSSAI.DecodeFromBuffer(buf + decoded_size, len, is_option);

    Logger::nas_mm().debug(
        "Decoded S_NSSAI SST (0x%x) SD (0x%x) hplmnSST (0x%x) hplmnSD (0x%x)",
        SNSSAI);
    Logger::nas_mm().debug("Decoded S_NSSAI len (%d)", decoded_size);
  }
  return decoded_size;
}
