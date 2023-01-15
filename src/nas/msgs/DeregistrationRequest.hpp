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
 \author
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _DEREGISTRATION_REQUEST_H_
#define _DEREGISTRATION_REQUEST_H_

#include "NasIeHeader.hpp"

using namespace std;
namespace nas {

class DeregistrationRequest {
 public:
  DeregistrationRequest();
  ~DeregistrationRequest();
  int Encode(uint8_t* buf, int len);
  int Decode(NasMmPlainHeader* header, uint8_t* buf, int len);
  void SetHeader(uint8_t security_header_type);
  void setDeregistrationType(uint8_t dereg_type);
  void setDeregistrationType(_5gs_deregistration_type_t type);
  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);

  void setMobilityIdentityType(uint8_t type);
  void SetSuciSupiFormatImsi(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, const string msin);
  void SetSuciSupiFormatImsi(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, uint8_t hnpki, const string msin);
  void Set5gGuti();
  void SetImeiImeisv();
  void Set5gSTmsi();

  void getDeregistrationType(uint8_t& dereg_type);
  void getDeregistrationType(_5gs_deregistration_type_t& type);
  bool getngKSI(uint8_t& ng_ksi);
  void getMobilityIdentityType(uint8_t& type);
  std::string get_5g_guti();
  bool getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi);

 public:
  NasMmPlainHeader* plain_header;
  _5GSDeregistrationType* ie_deregistrationtype;
  NasKeySetIdentifier* ie_ngKSI;
  _5GSMobileIdentity* ie_5gs_mobility_id;
};

}  // namespace nas

#endif
