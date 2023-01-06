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

#include "NSSAI.hpp"

#include "3gpp_24.501.hpp"
#include "amf.hpp"
#include "logger.hpp"

#include <vector>

using namespace nas;

//------------------------------------------------------------------------------
NSSAI::NSSAI(uint8_t iei) : Type4NasIe(iei) {
  SetLengthIndicator(0);
  SetIeName(kNssaiIeName);
}

//------------------------------------------------------------------------------
NSSAI::NSSAI(uint8_t iei, const std::vector<struct SNSSAI_s>& nssai)
    : Type4NasIe(iei) {
  int length = 0;
  S_NSSAIs.assign(nssai.begin(), nssai.end());
  for (int i = 0; i < nssai.size(); i++) {
    length += (1 + nssai[i].length);  // 1 for length of NSSAI[i]
  }
  SetLengthIndicator(length);
  SetIeName(kNssaiIeName);
}

//------------------------------------------------------------------------------
NSSAI::NSSAI() : Type4NasIe(), S_NSSAIs() {
  SetIeName(kNssaiIeName);
}

//------------------------------------------------------------------------------
NSSAI::~NSSAI() {}

//------------------------------------------------------------------------------
void NSSAI::getValue(std::vector<struct SNSSAI_s>& nssai) {
  nssai.assign(S_NSSAIs.begin(), S_NSSAIs.end());
}

//------------------------------------------------------------------------------
int NSSAI::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  encoded_size += Type4NasIe::Encode(buf + encoded_size, len);

  for (int i = 0; i < S_NSSAIs.size(); i++) {
    // TODO: Define encode for SNSSAI_s
    int len_s_nssai = SST_LENGTH;
    encoded_size++;  // Store the length of S-NSSAI contents later

    ENCODE_U8(buf + encoded_size, S_NSSAIs.at(i).sst, encoded_size);

    if (S_NSSAIs.at(i).sd != SD_NO_VALUE) {
      len_s_nssai += SD_LENGTH;
      ENCODE_U24(buf + encoded_size, S_NSSAIs.at(i).sd, encoded_size);
      Logger::nas_mm().debug("Encoded NSSAI SD (0x%x)", S_NSSAIs.at(i).sd);
    }
    if (S_NSSAIs.at(i).length > (SST_LENGTH + SD_LENGTH)) {
      if (S_NSSAIs.at(i).mHplmnSst != -1) {
        len_s_nssai += SST_LENGTH;
        *(buf + encoded_size) = S_NSSAIs.at(i).mHplmnSst;
        encoded_size++;
      }
      if (S_NSSAIs.at(i).mHplmnSd != SD_NO_VALUE) {
        len_s_nssai += SD_LENGTH;
        ENCODE_U24(buf + encoded_size, S_NSSAIs.at(i).mHplmnSd, encoded_size);
      }
    }

    int encoded_size_tmp = 0;
    ENCODE_U8(
        buf + encoded_size - len_s_nssai - 1, len_s_nssai, encoded_size_tmp);
  }

  Logger::nas_mm().debug("Encoded NSSAI len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NSSAI::decodeFromBuffer(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());
  if (len < kNssaiMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNssaiMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  SNSSAI_s a       = {0, 0, 0, 0};

  // IEI and Length
  decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);

  int length_tmp = GetLengthIndicator();

  while (length_tmp) {
    // Decode length of SNSSAI
    uint8_t len_snssai = 0;
    DECODE_U8(buf + decoded_size, len_snssai, decoded_size);
    length_tmp--;

    switch (len_snssai) {
      case 1: {  // Only SST
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        a.sd        = SD_NO_VALUE;
        a.mHplmnSst = 0;
        a.mHplmnSd  = 0;
      } break;
      case 4: {  // SST and SD
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        a.mHplmnSst = 0;
        a.mHplmnSd  = 0;
      } break;
      case 5: {  // SST, SD and HPLMN SST
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        // HPLMN SST
        DECODE_U8(buf + decoded_size, a.mHplmnSst, decoded_size);
        length_tmp--;
        a.mHplmnSd = SD_NO_VALUE;
      } break;
      case 8: {
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        // HPLMN SST
        DECODE_U8(buf + decoded_size, a.mHplmnSst, decoded_size);
        length_tmp--;
        // HPLMN SD
        DECODE_U24(buf + decoded_size, a.mHplmnSd, decoded_size);
        length_tmp -= 3;
      } break;
    }

    S_NSSAIs.insert(S_NSSAIs.end(), a);
    a = {0, 0, 0, 0};
  }

  for (int i = 0; i < S_NSSAIs.size(); i++) {
    Logger::nas_mm().debug(
        "Decoded NSSAI %s", S_NSSAIs.at(i).ToString().c_str());
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
