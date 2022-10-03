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
NSSAI::NSSAI(uint8_t iei) {
  _iei   = iei;
  length = 0;
}

//------------------------------------------------------------------------------
NSSAI::NSSAI(const uint8_t iei, const std::vector<struct SNSSAI_s>& nssai) {
  _iei   = iei;
  length = 0;
  S_NSSAIs.assign(nssai.begin(), nssai.end());
  for (int i = 0; i < nssai.size(); i++) {
    // TODO: do the calculation based on length of nssai
    length += 2;  // 1 for IEI and 1 for sst
    if (nssai[i].sd != SD_NO_VALUE) length += SD_LENGTH;
    if (nssai[i].mHplmnSst != -1) length += SST_LENGTH;
    if (nssai[i].mHplmnSd != SD_NO_VALUE) length += SD_LENGTH;
  }
}

//------------------------------------------------------------------------------
NSSAI::NSSAI() : _iei(), length(), S_NSSAIs() {}

//------------------------------------------------------------------------------
NSSAI::~NSSAI() {}

//------------------------------------------------------------------------------
void NSSAI::getValue(std::vector<struct SNSSAI_s>& nssai) {
  nssai.assign(S_NSSAIs.begin(), S_NSSAIs.end());
}

//------------------------------------------------------------------------------
int NSSAI::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NSSAI IEI (0x%x)", _iei);

  int total_length = (_iei > 0) ? (length + 2) : (length + 1);
  if (len < total_length) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)",
        total_length);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U8(buf + encoded_size, length, encoded_size);

  for (int i = 0; i < S_NSSAIs.size(); i++) {
    // TODO: Define encode for SNSSAI_s
    int len_s_nssai = SST_LENGTH;
    encoded_size++;  // Store the length of S-NSSAI contents later

    ENCODE_U8(buf + encoded_size, S_NSSAIs.at(i).sst, encoded_size);

    if (S_NSSAIs.at(i).sd != SD_NO_VALUE) {
      len_s_nssai += SD_LENGTH;
      ENCODE_U8(
          buf + encoded_size, (S_NSSAIs.at(i).sd & 0x00ff0000) >> 16,
          encoded_size);
      Logger::nas_mm().debug(
          "Encoded NSSAI SD first octet (0x%x)",
          (S_NSSAIs.at(i).sd & 0x00ff0000) >> 16);

      ENCODE_U8(
          buf + encoded_size, (S_NSSAIs.at(i).sd & 0x0000ff00) >> 8,
          encoded_size);
      Logger::nas_mm().debug(
          "Encoded NSSAI SD second octet (%x)", *(buf + encoded_size - 1));

      ENCODE_U8(
          buf + encoded_size, S_NSSAIs.at(i).sd & 0x000000ff, encoded_size);
      Logger::nas_mm().debug(
          "Encoded NSSAI SD third octet (%x)", *(buf + encoded_size - 1));
    }
    if (S_NSSAIs.at(i).mHplmnSst != -1) {
      len_s_nssai += SST_LENGTH;
      ENCODE_U8(buf + encoded_size, S_NSSAIs.at(i).mHplmnSst, encoded_size);
    }
    if (S_NSSAIs.at(i).mHplmnSd != SD_NO_VALUE) {
      len_s_nssai += SD_LENGTH;
      ENCODE_U8(
          buf + encoded_size, (S_NSSAIs.at(i).mHplmnSd & 0x00ff0000) >> 16,
          encoded_size);
      ENCODE_U8(
          buf + encoded_size, (S_NSSAIs.at(i).mHplmnSd & 0x0000ff00) >> 8,
          encoded_size);
      ENCODE_U8(
          buf + encoded_size, S_NSSAIs.at(i).mHplmnSd & 0x000000ff,
          encoded_size);
    }

    // Length of S-NSSAI contents
    int encoded_len_ie = 0;
    ENCODE_U8(
        buf + encoded_size - len_s_nssai - 1, len_s_nssai, encoded_len_ie);
  }

  Logger::nas_mm().debug("Encoded NSSAI len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NSSAI::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding NSSAI IEI (0x%x)", *buf);
  int decoded_size = 0;
  SNSSAI_s a       = {0, 0, 0, 0};
  if (is_option) {
    // IEI
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }

  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);

  int length_tmp = length;

  while (length_tmp) {
    switch (*(buf + decoded_size)) {  // Length of SNSSAI
      case 1: {                       // Only SST
        // Length of SNSSAI
        decoded_size++;
        length_tmp--;
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        a.sd        = SD_NO_VALUE;
        a.mHplmnSst = 0;
        a.mHplmnSd  = 0;
      } break;
      case 4: {  // SST and SD
        // Length of SNSSAI
        decoded_size++;
        length_tmp--;
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
        // Length of SNSSAI
        decoded_size++;
        length_tmp--;
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
        // Length of SNSSAI
        decoded_size++;
        length_tmp--;
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
    Logger::nas_mm().debug("Decoded NSSAI %s", S_NSSAIs.at(i).ToString());
  }
  Logger::nas_mm().debug("Decoded NSSAI len (%d)", decoded_size);
  return decoded_size;
}
