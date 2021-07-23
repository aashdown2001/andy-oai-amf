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

/*! \file pdu_session_context.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _PDU_SESSION_CONTEXT_H_
#define _PDU_SESSION_CONTEXT_H_

#include <string>
#include "bstrlib.h"
#include "amf.hpp"
#include <nlohmann/json.hpp>
class pdu_session_context {
 public:
  pdu_session_context();
  ~pdu_session_context();
  void pdu_session_context_n1sm_from_json(nlohmann::json j);
  void pdu_session_context_n2sm_from_json(nlohmann::json j);
  
  bool pdu_session_context_from_json(nlohmann::json j);
  uint32_t pdu_session_context_ran_ue_ngap_id_from_json(nlohmann::json j);
  long pdu_session_context_amf_ue_ngap_id_from_json(nlohmann::json j);
  uint8_t pdu_session_context_req_type_from_json(nlohmann::json j);
  uint8_t pdu_session_context_pdu_session_id_from_json(nlohmann::json j);
  bool pdu_session_context_isn2sm_avaliable_from_json(nlohmann::json j);
  bool pdu_session_context_isn1sm_avaliable_from_json(nlohmann::json j);
  std::string pdu_session_context_dnn_from_json(nlohmann::json j);
  std::string pdu_session_context_supi_from_json(nlohmann::json j);
  std::string pdu_session_context_remote_smf_addr_from_json(nlohmann::json j);
  bool pdu_session_context_smf_available_from_json(nlohmann::json j);
  std::string pdu_session_context_location_from_json(nlohmann::json j);

  void pdu_session_context_snssai_from_json(nlohmann::json j,snssai_t &snssai_json);
  void pdu_session_context_plmn_from_json(nlohmann::json j,plmn_t &plmn_json);


  std::string pdu_session_context_smf_context_location_from_json(nlohmann::json j);


  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  uint8_t req_type;
  uint8_t pdu_session_id;
  bstring n2sm;
  bool isn2sm_avaliable;
  bstring n1sm;
  bool isn1sm_avaliable;
  std::string dnn;
  std::string smf_addr;
  std::string smf_api_version;
  std::string remote_smf_addr[0];  //"192.168.12.10:8080"
  bool smf_available;
  std::string location;
  snssai_t snssai;
  plmn_t plmn;
  std::string smf_context_location;

  std::string supi;
  //15
};
#endif
