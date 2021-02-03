/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#ifndef _PlmnId_H
#define _PlmnId_H

extern "C" {
#include "Ngap_PLMNIdentity.h"
}

#include <string>

namespace ngap {

class PlmnId {
 public:
  PlmnId();
  virtual ~PlmnId();

  bool encode2octetstring(Ngap_PLMNIdentity_t&);
  bool decodefromoctetstring(Ngap_PLMNIdentity_t&);
  void setMccMnc(const std::string mcc, const std::string mnc);
  void getMcc(std::string& mcc);
  void getMnc(std::string& mnc);

 private:
  uint8_t mcc_digit1;
  uint8_t mcc_digit2;
  uint8_t mcc_digit3;
  uint8_t mnc_digit1;  // in case of 2 digit MNC, it should be 0xf
  uint8_t mnc_digit2;
  uint8_t mnc_digit3;
};

}  // namespace ngap

#endif
