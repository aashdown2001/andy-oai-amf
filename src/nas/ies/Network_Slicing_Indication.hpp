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

#ifndef __Network_Slicing_Indication_H_
#define __Network_Slicing_Indication_H_

#include <stdint.h>

namespace nas {

class Network_Slicing_Indication {
 public:
  Network_Slicing_Indication();
  Network_Slicing_Indication(uint8_t iei);
  Network_Slicing_Indication(const uint8_t iei, bool dcni, bool nssci);
  ~Network_Slicing_Indication();
  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);
  void setDCNI(bool value);
  void setNSSCI(bool value);
  bool getDCNI();
  bool getNSSCI();

 private:
  uint8_t _iei;
  bool NSSCI;
  bool DCNI;
};
}  // namespace nas

#endif
