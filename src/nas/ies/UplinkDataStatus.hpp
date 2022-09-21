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

#ifndef __UplinkDataStatus_H_
#define __UplinkDataStatus_H_

#include <stdint.h>

namespace nas {

class UplinkDataStatus {
 public:
  UplinkDataStatus();
  UplinkDataStatus(uint8_t);
  UplinkDataStatus(const uint8_t iei, uint16_t value);
  ~UplinkDataStatus();
  void setValue(uint8_t iei, uint16_t value);
  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);
  uint16_t getValue();

 private:
  uint8_t _iei;
  uint8_t length;
  uint16_t _value;
};

}  // namespace nas

#endif
