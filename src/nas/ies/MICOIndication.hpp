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

#ifndef _MICO_Indication_H
#define _MICO_Indication_H

#include <stdint.h>

constexpr uint8_t kMICOIndicationIELength = 1;

namespace nas {

class MICOIndication {
 public:
  MICOIndication();
  MICOIndication(const uint8_t _iei, bool sprti, bool raai);
  MICOIndication(bool sprti, bool raai);
  ~MICOIndication();

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);

  void setSPRTI(bool value);
  void setRAAI(bool value);
  bool getRAAI();
  bool getSPRTI();

 private:
  uint8_t iei;
  bool SPRTI;
  bool RAAI;
};

}  // namespace nas

#endif
