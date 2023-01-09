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

#ifndef _LADN_Indication_H_
#define _LADN_Indication_H_

#include <stdint.h>
#include <iostream>
#include <vector>

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}
using namespace std;

constexpr uint8_t kLadnIndicationMinimumLength  = 3;
constexpr uint16_t kLadnIndicationMaximumLength = 1715;

namespace nas {

class LADN_Indication {
 public:
  LADN_Indication();
  LADN_Indication(uint8_t iei);
  LADN_Indication(const uint8_t iei, std::vector<bstring> ladn);
  ~LADN_Indication();
  void setValue(uint8_t iei);
  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);
  void getValue(std::vector<bstring>& ladn);

 private:
  uint8_t _iei;
  uint16_t length;
  std::vector<bstring> LADN;
};

}  // namespace nas

#endif
