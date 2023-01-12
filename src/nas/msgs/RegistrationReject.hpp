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

#ifndef _REGISTRATION_REJECT_H_
#define _REGISTRATION_REJECT_H_

#include "NasIeHeader.hpp"

namespace nas {

class RegistrationReject : public NasMmPlainHeader {
 public:
  RegistrationReject();
  ~RegistrationReject();

  void setHeader(uint8_t security_header_type);
  void getSecurityHeaderType(uint8_t security_header_type);
  bool verifyHeader();

  int Encode(uint8_t* buf, int len);
  int Decode(NasMmPlainHeader* header, uint8_t* buf, int len);

  void set_5GMM_Cause(uint8_t value);
  // TODO: Get

  void setGPRS_Timer_2_3346(uint8_t value);
  // TODO: Get

  void setGPRS_Timer_2_3502(uint8_t value);
  // TOGO: Get

  void setEAP_Message(bstring eap);
  void setRejected_NSSAI(const std::vector<Rejected_SNSSAI>& nssai);
  // TODO: Get

 public:
  _5GMM_Cause ie_5gmm_cause;                        // Mandatory
  std::optional<GprsTimer2> ie_T3346_value;         // Optional
  std::optional<GprsTimer2> ie_T3502_value;         // Optional
  std::optional<EapMessage> ie_eap_message;         // Optional
  std::optional<Rejected_NSSAI> ie_rejected_nssai;  // Release 16.4.1
};

}  // namespace nas

#endif
