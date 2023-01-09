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

#include "_5GSTrackingAreaIdList.hpp"

#include "String2Value.hpp"
#include "NasUtils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSTrackingAreaIdList::_5GSTrackingAreaIdList(
    uint8_t iei, std::vector<p_tai_t> tai_list) {
  m_tai_list = tai_list;
  m_iei      = iei;
  length     = 0;  // TODO:
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::Encode(uint8_t* buf, int len) {
  int encoded_size = 0;
  int len_pos      = 0;
  if (m_iei) {
    ENCODE_U8(buf + encoded_size, m_iei, encoded_size);  // IEI
  }

  len_pos = encoded_size;  // position to encode Length
  encoded_size++;          // for length octet later on

  for (int i = 0; i < m_tai_list.size(); i++) {
    int item_len = 0;
    switch (m_tai_list[i].type) {
      case 0x00: {
        item_len += encode_00_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
      case 0x01: {
        item_len += encode_01_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
      case 0x10: {
        item_len += encode_10_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
    }
    encoded_size += item_len;
    length += item_len;
  }
  //*(buf + encoded_size - length - 1) = length;
  uint8_t encoded_size_ie = 0;
  ENCODE_U8(buf + len_pos, length, encoded_size_ie);

  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_00_type(
    p_tai_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  uint8_t octet    = 0x00 | ((item.tac_list.size() - 1) &
                          0x1f);  // see Table 9.11.3.9.1@3GPP TS 24.501 V16.1.0
  ENCODE_U8(buf + encoded_size, octet, encoded_size);  // IEI

  // Encode PLMN
  encoded_size += NasUtils::encodeMccMnc2Buffer(
      item.plmn_list[0].mcc, item.plmn_list[0].mnc, buf + encoded_size,
      len - encoded_size);

  // Encode TAC list
  for (int i = 0; i < item.tac_list.size(); i++) {
    // TODO: use ENCODE_U24
    ENCODE_U24(buf + encoded_size, item.tac_list[i], encoded_size);
    /*
octet = (item.tac_list[i] & 0x00ff0000) >> 16;
ENCODE_U8(buf + encoded_size, octet, encoded_size);
octet = (item.tac_list[i] & 0x0000ff00) >> 8;
ENCODE_U8(buf + encoded_size, octet, encoded_size);
octet = (item.tac_list[i] & 0x000000ff) >> 0;
ENCODE_U8(buf + encoded_size, octet, encoded_size);*/
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_01_type(
    p_tai_t item, uint8_t* buf, int len) {
  // TODO:
  return 1;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_10_type(
    p_tai_t item, uint8_t* buf, int len) {
  // TODO:
  return 1;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_mcc_mnc(
    nas_plmn_t plmn, uint8_t* buf, int len) {
  int encoded_size = 0;
  int mcc          = fromString<int>(plmn.mcc);
  int mnc          = fromString<int>(plmn.mnc);
  *(buf + encoded_size) =
      (0x0f & (mcc / 100)) | ((0x0f & ((mcc % 100) / 10)) << 4);
  encoded_size += 1;
  if (!(mnc / 100)) {
    *(buf + encoded_size) = (0x0f & (mcc % 10)) | 0xf0;
    encoded_size += 1;
    *(buf + encoded_size) =
        (0x0f & ((mnc % 100) / 10)) | ((0x0f & (mnc % 10)) << 4);
    encoded_size += 1;
  } else {
    *(buf + encoded_size) = (0x0f & (mcc % 10)) | ((0x0f & (mnc % 10)) << 4);
    encoded_size += 1;
    *(buf + encoded_size) =
        ((0x0f & ((mnc % 100) / 10)) << 4) | (0x0f & (mnc / 100));
    encoded_size += 1;
  }
  return encoded_size;
}
