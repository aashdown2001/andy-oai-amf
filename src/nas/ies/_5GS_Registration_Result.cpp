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

#include "_5GS_Registration_Result.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GS_Registration_Result::_5GS_Registration_Result(uint8_t iei) {
  _iei                 = iei;
  length               = k5gsRegistrationResultLength;
  emergency_registered = false;
  NSSAA_performed      = false;
  SMS_allowed          = false;
  _value               = 0;
}

//------------------------------------------------------------------------------
_5GS_Registration_Result::_5GS_Registration_Result(
    const uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value) {
  _iei                 = iei;
  length               = k5gsRegistrationResultLength;
  emergency_registered = emergency;
  NSSAA_performed      = nssaa;
  SMS_allowed          = sms;
  _value               = value;
}

//------------------------------------------------------------------------------
_5GS_Registration_Result::_5GS_Registration_Result() {
  _iei                 = 0;
  length               = 0;
  emergency_registered = false;
  NSSAA_performed      = false;
  SMS_allowed          = false;
  _value               = 0;
}
_5GS_Registration_Result::~_5GS_Registration_Result() {}

//------------------------------------------------------------------------------
void _5GS_Registration_Result::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Registration_Result::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
void _5GS_Registration_Result::set(
    const uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value) {
  _iei                 = iei;
  length               = k5gsRegistrationResultLength;
  emergency_registered = emergency;
  NSSAA_performed      = nssaa;
  SMS_allowed          = sms;
  _value               = value;
}

//------------------------------------------------------------------------------
void _5GS_Registration_Result::set(
    bool emergency, bool nssaa, bool sms, uint8_t value) {
  _iei                 = 0;
  length               = k5gsRegistrationResultLength - 1;  // without IEI
  emergency_registered = emergency;
  NSSAA_performed      = nssaa;
  SMS_allowed          = sms;
  _value               = value;
}

//------------------------------------------------------------------------------
int _5GS_Registration_Result::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5GS_Registration_Result");
  uint8_t ie_len = 0;
  if (_iei) {
    ie_len = k5gsRegistrationResultLength;
  } else {
    ie_len = k5gsRegistrationResultLength - 1;
  }

  if (len < ie_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;

  if (_iei) {
    ENCODE_U8(buf + encoded_size, _iei, encoded_size);  // IEI
  }
  // Length
  ENCODE_U8(buf + encoded_size, length, encoded_size);

  // Octet 3
  uint8_t octet = 0;
  octet         = 0x00 | (emergency_registered << 5) | (NSSAA_performed << 4) |
          (SMS_allowed << 3) | (_value & 0x07);
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded _5GS_Registration_Result, value 0x%x, IEI 0x%x", _value, _iei);

  Logger::nas_mm().debug(
      "Encoded _5GS_Registration_Result (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_Registration_Result::Decode(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding _5GS_Registration_Result");

  if (len < k5gsRegistrationResultLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)",
        k5gsRegistrationResultLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, _iei, decoded_size);  // IEI
  }
  // Length
  DECODE_U8(buf + decoded_size, length, decoded_size);

  // Octet 3
  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  emergency_registered = (octet & 0x20) >> 5;
  NSSAA_performed      = (octet & 0x10) >> 4;
  SMS_allowed          = (octet & 0x08) >> 3;
  _value               = octet & 0x07;

  Logger::nas_mm().debug(
      "Decoded _5GS_Registration_Result, Emergency Registered 0x%x, NSSAA "
      "Performed 0x%x, SMS Allowed 0x%x, "
      "Value 0x%x",
      emergency_registered, NSSAA_performed, SMS_allowed, _value);
  Logger::nas_mm().debug(
      "Decoded _5GS_Registration_Result (len %d)", decoded_size);
  return decoded_size;
}
