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

#ifndef _SECURITY_MODE_COMMAND_H_
#define _SECURITY_MODE_COMMAND_H_

#include "NasIeHeader.hpp"

namespace nas {

class SecurityModeCommand : public NasMmPlainHeader {
 public:
  SecurityModeCommand();
  ~SecurityModeCommand();

  void setHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void setNAS_Security_Algorithms(uint8_t ciphering, uint8_t integrity);
  // TODO: Get

  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  // TODO: Get

  void setUE_Security_Capability(uint8_t g_EASel, uint8_t g_IASel);
  // TODO: Get
  void setUE_Security_Capability(
      uint8_t g_EASel, uint8_t g_IASel, uint8_t eea, uint8_t eia);
  // TODO: Get

  void setIMEISV_Request(uint8_t value);
  // TODO: Get

  void setEPS_NAS_Security_Algorithms(uint8_t ciphering, uint8_t integrity);
  // TODO: Get

  void setAdditional_5G_Security_Information(bool rinmr, bool hdp);
  // TODO: Get

  void SetEapMessage(bstring eap);
  // TODO: Get

  void setABBA(uint8_t length, uint8_t* value);
  // TODO: Get

  void setS1_UE_Security_Capability(uint8_t g_EEASel, uint8_t g_EIASel);
  // TODO: Get

 public:
  NasSecurityAlgorithms ie_selected_nas_security_algorithms;  // Mandatory
  NasKeySetIdentifier ie_ngKSI;                               // Mandatory
  UESecurityCapability ie_ue_security_capability;             // Mandatory

  std::optional<IMEISV_Request> ie_imeisv_request;  // Optional
  std::optional<EPS_NAS_Security_Algorithms>
      ie_eps_nas_security_algorithms;  // Optional
  std::optional<Additional_5G_Security_Information>
      ie_additional_5G_security_information;  // Optional
  std::optional<EapMessage> ie_eap_message;   // Optional
  std::optional<ABBA> ie_abba;                // Optional
  std::optional<S1_UE_Security_Capability>
      ie_s1_ue_security_capability;  // Optional
};

}  // namespace nas

#endif
