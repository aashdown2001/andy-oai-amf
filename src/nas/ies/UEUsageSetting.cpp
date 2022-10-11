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

#include "UEUsageSetting.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UEUsageSetting::UEUsageSetting(uint8_t iei) {
  _iei               = iei;
  _ues_usage_setting = false;
}

//------------------------------------------------------------------------------
UEUsageSetting::UEUsageSetting(const uint8_t iei, bool ues_usage_setting) {
  _iei               = iei;
  _ues_usage_setting = ues_usage_setting;
}

//------------------------------------------------------------------------------
UEUsageSetting::UEUsageSetting() {
  _iei               = 0;
  _ues_usage_setting = false;
}

//------------------------------------------------------------------------------
UEUsageSetting::~UEUsageSetting() {}

//------------------------------------------------------------------------------
void UEUsageSetting::setValue(bool value) {
  _ues_usage_setting = value;
}

//------------------------------------------------------------------------------
bool UEUsageSetting::getValue() {
  return _ues_usage_setting;
}

//------------------------------------------------------------------------------
int UEUsageSetting::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding UEUsageSetting (iei 0x%x)", _iei);

  if ((len < kUEUsageSettingLength) or (len < length + 2)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUEUsageSettingLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);
  }

  ENCODE_U8(buf + encoded_size, length, encoded_size);
  ENCODE_U8(buf + encoded_size, 0x01 & _ues_usage_setting, encoded_size);

  Logger::nas_mm().debug("Encoded UEUsageSetting (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UEUsageSetting::decodeFromBuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding UEUsageSetting");

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);
  }
  DECODE_U8(buf + decoded_size, length, decoded_size);
  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  _ues_usage_setting = octet & 0x01;

  Logger::nas_mm().debug(
      "Decoded UE's Usage Setting, UE's Usage Setting 0x%x, IEI 0x%x, decoded "
      "len %d",
      _ues_usage_setting, _iei, decoded_size);

  return decoded_size;
}
