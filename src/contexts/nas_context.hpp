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

/*! \file nas_context.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _AMF_NAS_CONTEXT_H_
#define _AMF_NAS_CONTEXT_H_

#include <stdint.h>
#include <string>
#include "struct.hpp"

#include "nas_security_context.hpp"
#include "security_def.hpp"
#include "authentication_algorithms_with_5gaka.hpp"
#include <nlohmann/json.hpp>
#include "NgapIEsStruct.hpp"

extern "C" {
#include "Ngap_RRCEstablishmentCause.h"
}

typedef enum {
  _5GMM_STATE_MIN     = 0,
  _5GMM_STATE_INVALID = _5GMM_STATE_MIN,
  _5GMM_DEREGISTERED,
  _5GMM_REGISTERED,
  _5GMM_DEREGISTERED_INITIATED,
  _5GMM_COMMON_PROCEDURE_INITIATED,
  _5GMM_STATE_MAX
} _5gmm_state_t;

static const std::vector<std::string> _5gmm_state_e2str = {
    "_5GMM_STATE_INVALID",
    "_5GMM_DEREGISTERED",
    "_5GMM_REGISTERED",
    "_5GMM_DEREGISTERED_INITIATED",
    "_5GMM_COMMON_PROCEDURE_INITIATED",
    "_5GMM_STATE_MAX"};

class nas_context {
 public:
  nas_context();
  ~nas_context();
  //
  //ctx_avaliability_ind = true;

  // nc.get()->is_imsi_present = true;
  // nc.get()->imsi = supi;
//nc.get()->imsi = supi;

  bool nas_context_from_json(nlohmann::json j);
  

  bool nas_context_ctx_avaliability_ind_from_json(nlohmann::json j);
  bool nas_context_is_stacs_available_from_json(nlohmann::json j);
  long nas_context_amf_ue_ngap_id_from_json(nlohmann::json j);
  uint32_t nas_context_ran_ue_ngap_id_from_json(nlohmann::json j);
  std::string nas_context_nas_status_from_json(nlohmann::json j);
  int nas_context__5gmm_state_from_json(nlohmann::json j);

  uint8_t nas_context_registration_type_from_json(nlohmann::json j);
  bool nas_context_follow_on_req_pending_ind_from_json(nlohmann::json j);
  uint8_t nas_context_ngKsi_from_json(nlohmann::json j);

  std::string nas_context_imsi_from_json(nlohmann::json j);  //ok
//  uint8_t mmCapability;
uint8_t nas_context_mmCapability_from_json(nlohmann::json j);
//   uint8_t ueSecurityCaplen;
uint8_t nas_context_ueSecurityCaplen_from_json(nlohmann::json j);
//   uint8_t ueSecurityCapEnc;
uint8_t nas_context_ueSecurityCapEnc_from_json(nlohmann::json j);
//   uint8_t ueSecurityCapInt;
uint8_t nas_context_ueSecurityCapInt_from_json(nlohmann::json j);
//   uint8_t ueSecurityCapEEnc;
uint8_t nas_context_ueSecurityCapEEnc_from_json(nlohmann::json j);
//   uint8_t ueSecurityCapEInt;
uint8_t nas_context_ueSecurityCapEInt_from_json(nlohmann::json j);

  //void nas_context_requestedNssai_from_json(nlohmann::json j);
  std::string nas_context_serving_network_from_json(nlohmann::json j);
  void nas_context_auts_from_json(nlohmann::json j);
  bool nas_context_is_specific_procedure_for_registration_running_from_json(nlohmann::json j);
  bool nas_context_is_specific_procedure_for_deregistration_running_from_json(nlohmann::json j);
  bool nas_context_is_specific_procedure_for_eCell_inactivity_running_from_json(nlohmann::json j);
  bool nas_context_is_common_procedure_for_authentication_running_from_json(nlohmann::json j);
  bool nas_context_is_common_procedure_for_identification_running_from_json(nlohmann::json j);
  bool nas_context_is_common_procedure_for_security_mode_control_running_from_json(nlohmann::json j);
  bool nas_context_is_common_procedure_for_nas_transport_running_from_json(nlohmann::json j);
  
  void nas_context__vector_from_json(nlohmann::json j);
  void nas_context__5g_he_av_from_json(nlohmann::json j);
  void nas_context__5g_av_from_json(nlohmann::json j);
  std::string nas_context_Href_from_json(nlohmann::json j);
  void nas_context_kamf_from_json(nlohmann::json j);
  void nas_context__security_from_json(nlohmann::json j);
  void nas_context_security_ctx_from_json(nlohmann::json j);
  bool nas_context_is_current_security_available_from_json(nlohmann::json j);
  int nas_context_registration_attempt_counter_from_json(nlohmann::json j);

  bool nas_context_is_imsi_present_from_json(nlohmann::json j);
  bool nas_context_is_5g_guti_present_from_json(nlohmann::json j);
  bool nas_context_is_auth_vectors_present_from_json(nlohmann::json j);
  bool nas_context_to_be_register_by_new_suci_from_json(nlohmann::json j);
  std::string nas_context_guti_from_json(nlohmann::json j);


  


  bool ctx_avaliability_ind;
  bool is_stacs_available;
  long amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
  std::string nas_status;
  _5gmm_state_t _5gmm_state;

  /************ parameters from Registration request *************/
  uint8_t registration_type : 3;
  bool follow_on_req_pending_ind;
  uint8_t ngKsi : 4;
  // mobility identity: imsi, supi, 5g-guti, etc
  std::string imsi;
  uint8_t mmCapability;
  uint8_t ueSecurityCaplen;
  uint8_t ueSecurityCapEnc;
  uint8_t ueSecurityCapInt;
  uint8_t ueSecurityCapEEnc;
  uint8_t ueSecurityCapEInt;
  std::vector<nas::SNSSAI_t> requestedNssai;
  std::string serving_network;
  bstring auts;
  /************ NAS EP(s) ****************/
  bool is_specific_procedure_for_registration_running;
  bool is_specific_procedure_for_deregistration_running;
  bool is_specific_procedure_for_eCell_inactivity_running;
  bool is_common_procedure_for_authentication_running;
  bool is_common_procedure_for_identification_running;
  bool is_common_procedure_for_security_mode_control_running;
  bool is_common_procedure_for_nas_transport_running;

  /************ security related ***********/
#define MAX_5GS_AUTH_VECTORS 1
  auc_vector_t _vector[MAX_5GS_AUTH_VECTORS];   /* 5GS authentication vector */
  _5G_HE_AV_t _5g_he_av[MAX_5GS_AUTH_VECTORS];  // generated by UDM
  _5G_AV_t _5g_av[MAX_5GS_AUTH_VECTORS];        // generated by ausf
  std::string Href;
  uint8_t kamf[MAX_5GS_AUTH_VECTORS][32];
  security_context_t _security;

  nas_secu_ctx* security_ctx;

  bool is_current_security_available;

  int registration_attempt_counter;  // used to limit the subsequently reject
                                     // registration
                                     // attempts(clause 5.5.1.2.7/5.5.1.3.7,
                                     // 3gpp ts24.501)
  /**************** parameters present? ****************/
  bool is_imsi_present;
  bool is_5g_guti_present;
  bool is_auth_vectors_present;
  bool to_be_register_by_new_suci;

  std::string guti ;

};

#endif
