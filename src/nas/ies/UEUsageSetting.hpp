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

#ifndef _UES_USAGE_SETTING_H_
#define _UES_USAGE_SETTING_H_

#include <stdint.h>

constexpr uint8_t kUEUsageSettingLength = 3;

namespace nas {

class UEUsageSetting {
 public:
  UEUsageSetting();
  UEUsageSetting(uint8_t iei);
  UEUsageSetting(const uint8_t iei, bool ues_usage_setting);
  ~UEUsageSetting();
  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);
  void setValue(bool value);
  bool getValue();

 private:
  uint8_t _iei;
  uint8_t length;
  bool _ues_usage_setting;
};
}  // namespace nas

#endif
