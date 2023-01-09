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

#ifndef _5GS_REGISTRATION_RESULT_H_
#define _5GS_REGISTRATION_RESULT_H_

#include <stdint.h>

constexpr uint8_t k5gsRegistrationResultLength = 3;

namespace nas {

class _5GS_Registration_Result {
 public:
  _5GS_Registration_Result();
  _5GS_Registration_Result(uint8_t iei);
  _5GS_Registration_Result(
      const uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value);
  ~_5GS_Registration_Result();
  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);
  void setValue(uint8_t value);
  uint8_t getValue();
  void set(
      const uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value);
  void set(bool emergency, bool nssaa, bool sms, uint8_t value);

 private:
  uint8_t _iei;
  uint8_t length;
  bool emergency_registered;
  bool NSSAA_performed;
  bool SMS_allowed;
  uint8_t _value;
};
}  // namespace nas

#endif
