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
#ifndef __EPS_NAS_Message_Container_H_
#define __EPS_NAS_Message_Container_H_
#include <stdint.h>

#include <iostream>
extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}
namespace nas {

class EPS_NAS_Message_Container {
 public:
  EPS_NAS_Message_Container();
  EPS_NAS_Message_Container(uint8_t iei);
  EPS_NAS_Message_Container(const uint8_t iei, bstring value);
  ~EPS_NAS_Message_Container();
  void setValue(uint8_t iei, uint8_t value);
  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);
  void getValue(bstring& value);

 private:
  uint8_t _iei;
  uint16_t length;
  bstring _value;
};

}  // namespace nas

#endif
