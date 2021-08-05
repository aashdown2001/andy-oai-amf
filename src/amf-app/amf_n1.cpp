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

/*! \file amf_n1.cpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_n1.hpp"

#include "AuthenticationFailure.hpp"
#include "amf_n2.hpp"
#include "amf_n11.hpp"
#include "amf_app.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "RegistrationRequest.hpp"
#include "RegistrationReject.hpp"
#include "AuthenticationRequest.hpp"
#include "AuthenticationResponse.hpp"
#include "DeregistrationAccept.hpp"
#include "DeregistrationRequest.hpp"
#include "IdentityRequest.hpp"
#include "IdentityResponse.hpp"
#include "RegistrationAccept.hpp"
#include "RegistrationReject.hpp"
#include "RegistrationRequest.hpp"
#include "SecurityModeCommand.hpp"
#include "ServiceAccept.hpp"
#include "ServiceRequest.hpp"
#include "String2Value.hpp"
#include "ULNASTransport.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_n11.hpp"
#include "amf_n2.hpp"
#include "comUt.hpp"
#include "itti.hpp"
#include "itti_msg_n11.hpp"
#include "itti_msg_n2.hpp"
#include "logger.hpp"
#include "nas_algorithms.hpp"
#include "sha256.hpp"
#include <nlohmann/json.hpp>

#include "AuthenticationInfo.h"
#include "ConfirmationData.h"
#include "ConfirmationDataResponse.h"
#include "UEAuthenticationCtx.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "Record.h"
extern "C" {
#include "bstrlib.h"
#include "dynamic_memory_check.h"
}

using namespace oai::amf::model;

using namespace nas;
using namespace amf_application;
using namespace config;

extern itti_mw *itti_inst;
extern amf_n1 *amf_n1_inst;
extern amf_n11 *amf_n11_inst;
extern amf_config amf_cfg;
extern amf_app *amf_app_inst;
extern amf_n2 *amf_n2_inst;
extern statistics stacs;
extern void convert_string_2_hex(std::string &input, std::string &output);
extern unsigned char *format_string_as_hex(std::string str);
extern void octet_stream_2_hex_stream(uint8_t *buf, int len, std::string &out);
extern int ncc;
Sha256 ctx;
random_state_t random_state;
static uint8_t no_random_delta = 0;
static std::map<std::string, std::string> rand_record;

void amf_n1_task(void *);

//------------------------------------------------------------------------------
void amf_n1_task(void *) {
  const task_id_t task_id = TASK_AMF_N1;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto *msg = shared_msg.get();

    switch (msg->msg_type) {
    case UL_NAS_DATA_IND: { // receive nas message buffer from amf_n2
      Logger::amf_n1().info("Received UL_NAS_DATA_IND");
      itti_uplink_nas_data_ind *m =
          dynamic_cast<itti_uplink_nas_data_ind *>(msg);
      amf_n1_inst->handle_itti_message(ref(*m));
    } break;
    case DOWNLINK_NAS_TRANSFER: {
      Logger::amf_n1().info("Received DOWNLINK_NAS_TRANSFER");
      itti_downlink_nas_transfer *m =
          dynamic_cast<itti_downlink_nas_transfer *>(msg);
      amf_n1_inst->handle_itti_message(ref(*m));
    } break;
    default:
      Logger::amf_n1().error("No handler for msg type %d", msg->msg_type);
    }
  }while (true);
}

//------------------------------------------------------------------------------
amf_n1::amf_n1() {
  if (itti_inst->create_task(TASK_AMF_N1, amf_n1_task, nullptr)) {
    Logger::amf_n1().error("Cannot create task TASK_AMF_N1");
    throw std::runtime_error("Cannot create task TASK_AMF_N1");
  }
  Logger::amf_n1().startup("Started");
  Logger::amf_n1().debug("Construct amf_n1 successfully");
}

//------------------------------------------------------------------------------
amf_n1::~amf_n1() {}

//------------------------------------------------------------------------------
void amf_n1::handle_itti_message(itti_downlink_nas_transfer &itti_msg) {
  long amf_ue_ngap_id = itti_msg.amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
  
  std::shared_ptr<nas_context> nc;

  Logger::amf_n1().debug("try to get sequence_number");
  nlohmann::json udsf_response; 
  std::string record_id = "amf_ue_ngap_id=\'" + to_string(amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n1().debug("No existing nas_context from UDSF using amf_ue_ngap_id (%d) ...", amf_ue_ngap_id);
  }else if(udsf_response.dump().length()<8){
    Logger::amf_n1().debug("No existing nas_context from UDSF using amf_ue_ngap_id (%d) .....", amf_ue_ngap_id);
  }else{
    Logger::amf_n1().debug("udsf_response %s",udsf_response.dump().c_str());
    nc = std::shared_ptr<nas_context>(new nas_context());
    nc.get()->nas_context_from_json(udsf_response);
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
    Logger::amf_n1().debug("GET nas_context (%p) from UDSF using amf_ue_ngap_id (%d) .....", nc.get(), amf_ue_ngap_id);
  }

#if 0
  std::shared_ptr<nas_context> nc;
  if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id))
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  else {
    Logger::amf_n1().warn("No existed nas_context with amf_ue_ngap_id(0x%x)",
                          amf_ue_ngap_id);
    return;
  }
#endif
  nas_secu_ctx *secu = nc.get()->security_ctx;
  print_buffer("amf_n1","dukl kas_int:",secu->knas_int, 16);
  Logger::amf_n1().debug("dukl dl.seq %d", secu->dl_count.seq_num);
  bstring protected_nas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK,
                               (uint8_t *)bdata(itti_msg.dl_nas),
                               blength(itti_msg.dl_nas), protected_nas);
  if (itti_msg.is_n2sm_set) {
    if (itti_msg.n2sm_info_type.compare("PDU_RES_REL_CMD") ==
        0) { // PDU SESSION RESOURCE RELEASE COMMAND
      itti_pdu_session_resource_release_command *release_command =
          new itti_pdu_session_resource_release_command(TASK_AMF_N1,
                                                        TASK_AMF_N2);
      release_command->nas = protected_nas;
      release_command->n2sm = itti_msg.n2sm;
      release_command->amf_ue_ngap_id = amf_ue_ngap_id;
      release_command->ran_ue_ngap_id = ran_ue_ngap_id;
      release_command->pdu_session_id = itti_msg.pdu_session_id;
      std::shared_ptr<itti_pdu_session_resource_release_command> i =
          std::shared_ptr<itti_pdu_session_resource_release_command>(
              release_command);
      int ret = itti_inst->send_msg(i);
      if (0 != ret) {
        Logger::amf_n1().error(
            "Could not send ITTI message %s to task TASK_AMF_N2",
            i->get_msg_name());
      }
    } else { // PDU SESSION RESOURCE SETUP_REQUEST
      itti_pdu_session_resource_setup_request *psrsr =
          new itti_pdu_session_resource_setup_request(TASK_AMF_N1, TASK_AMF_N2);
      psrsr->nas = protected_nas;
      psrsr->n2sm = itti_msg.n2sm;
      psrsr->amf_ue_ngap_id = amf_ue_ngap_id;
      psrsr->ran_ue_ngap_id = ran_ue_ngap_id;
      psrsr->pdu_session_id = itti_msg.pdu_session_id;
      std::shared_ptr<itti_pdu_session_resource_setup_request> i =
          std::shared_ptr<itti_pdu_session_resource_setup_request>(psrsr);
      int ret = itti_inst->send_msg(i);
      if (0 != ret) {
        Logger::amf_n1().error(
            "Could not send ITTI message %s to task TASK_AMF_N2",
            i->get_msg_name());
      }
    }

  } else {
    itti_dl_nas_transport *dnt =
        new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas = protected_nas;
    dnt->amf_ue_ngap_id = amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = ran_ue_ngap_id;
    std::shared_ptr<itti_dl_nas_transport> i =
        std::shared_ptr<itti_dl_nas_transport>(dnt);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          i->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_itti_message(itti_uplink_nas_data_ind &nas_data_ind) {
  long amf_ue_ngap_id = nas_data_ind.amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id = nas_data_ind.ran_ue_ngap_id;
  std::string nas_context_key =
      "app_ue_ranid_" + to_string(ran_ue_ngap_id) + "-amfid_" +
      to_string(amf_ue_ngap_id); // key for nas_context, option 1
  std::string snn;
  if (nas_data_ind.mnc.length() == 2) // TODO: remove hardcoded value
    snn = "5G:mnc0" + nas_data_ind.mnc + ".mcc" + nas_data_ind.mcc +
          ".3gppnetwork.org";
  else
    snn = "5G:mnc" + nas_data_ind.mnc + ".mcc" + nas_data_ind.mcc +
          ".3gppnetwork.org";
  Logger::amf_n1().debug("Serving network name %s", snn.c_str());
  plmn_t plmn = {};
  plmn.mnc = nas_data_ind.mnc;
  plmn.mcc = nas_data_ind.mcc;

  bstring recved_nas_msg = nas_data_ind.nas_msg;
  bstring decoded_plain_msg;


  //try to get nas_context using GUTI (Service Request)
  std::shared_ptr<nas_context> nc ;
  if (nas_data_ind.is_guti_valid) {
    std::string guti = nas_data_ind.guti;

    if (is_guti_2_nas_context(guti))//get context from UDSF
    {
      nc = guti_2_nas_context(guti);
    }
    else if(is_guti_2_nas_context_in_udsf(guti))
    {
      
      
      printf("--------service --99999999999999999999999999----\n");
        // nc = guti_2_nas_context_in_udsf(guti);
          nlohmann::json udsf_response; 
          nc = std::shared_ptr<nas_context>(new nas_context());
          std::string record_id = "guti=\'" + guti + "\'";
          std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
          if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response))
          {
            Logger::amf_n1().error("No existing nas_context with GUTI %s",guti.c_str());
            return ;
          }
          nc.get()->nas_context_from_json(udsf_response);
          printf("--------service ------%x\n",nc.get()->security_ctx->ul_count.seq_num);
          for(int ly=0;ly<32;ly++)
          {
            printf("-------------knas_enc--------in amf 1------%x  ",nc.get()->kamf[0][ly]);
          }

          printf("\n--------service -----imsi-%s\n",nc.get()->imsi.c_str());
          
          if(nc == nullptr)
          {
            printf("--------service  nc ----kong 0 !!!!!!!!!!!! ");
          }
    }
    else
    {
        Logger::amf_n1().error("No existing nas_context with GUTI %s",
                             nas_data_ind.guti.c_str());
      // return;
    }
    

  } else {
    //try to get nas_context using amf_ue_ngap_id
    //if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id))//find nas_context from UDSF
    //  nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);

    Logger::amf_n1().debug("try to get nas_context using amf_ue_ngap_id");
    nlohmann::json udsf_response; 
    std::string record_id = "amf_ue_ngap_id=\'" + to_string(amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
    if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
      Logger::amf_n1().debug("No existing nas_context from UDSF using amf_ue_ngap_id (%d) ...", amf_ue_ngap_id);
    }else if(udsf_response.dump().length()<8){
      Logger::amf_n1().debug("No existing nas_context from UDSF using amf_ue_ngap_id (%d) .....", amf_ue_ngap_id);
    }else{
      Logger::amf_n1().debug("udsf_response %s",udsf_response.dump().c_str());
      nc = std::shared_ptr<nas_context>(new nas_context());
      nc.get()->nas_context_from_json(udsf_response);
      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      Logger::amf_n1().debug("GET nas_context (%p) from UDSF using amf_ue_ngap_id (%d) .....", nc.get(), amf_ue_ngap_id);
    }
  }

  //try to parse Plain NAS Message
  SecurityHeaderType type = {};
  if (!check_security_header_type(type, (uint8_t *)bdata(recved_nas_msg))) {
    Logger::amf_n1().error("Not 5GS MOBILITY MANAGEMENT message");
    return;
  }
  uint8_t ulCount = 0;

  switch (type) {
  case PlainNasMsg: {
    Logger::amf_n1().debug("Received plain NAS message");
    decoded_plain_msg = recved_nas_msg;
  } break;
  case IntegrityProtected: {
    Logger::amf_n1().debug("Received integrity protected NAS message");
    ulCount = *((uint8_t *)bdata(recved_nas_msg) + 6);
    Logger::amf_n1().info("Integrity protected message: ulCount(%d)", ulCount);
    decoded_plain_msg = blk2bstr((uint8_t *)bdata(recved_nas_msg) + 7,
                                 blength(recved_nas_msg) - 7);
  } break;
  case IntegrityProtectedAndCiphered: {
    Logger::amf_n1().debug(
        "Received integrity protected and ciphered NAS message");
  } // break;
  case IntegrityProtectedWithNew5GNASSecurityContext: {
    Logger::amf_n1().debug(
        "Received integrity protected with new security context NAS message");
  } // break;
  case IntegrityProtectedAndCipheredWithNew5GNASSecurityContext: {
    Logger::amf_n1().debug(
        "Received integrity protected and ciphered with new security context "
        "NAS message");
    if (nc.get() == nullptr) {
      Logger::amf_n1().debug(
          "Abnormal condition: no existed nas_context. exit...");
      return;
    }
    uint32_t mac32 = 0;
    if (!nas_message_integrity_protected(nc.get()->security_ctx,
                                         NAS_MESSAGE_UPLINK,
                                         (uint8_t *)bdata(recved_nas_msg) + 6,
                                         blength(recved_nas_msg) - 6, mac32)) {
      // IA0_5G
      // TODO:
    } else {
      bool isMatched = false;
      uint8_t *buf = (uint8_t *)bdata(recved_nas_msg);
      int buf_len = blength(recved_nas_msg);
      uint32_t mac32_recv = ntohl((((uint32_t *)(buf + 2))[0]));
      Logger::amf_n1().debug("Received mac32 (0x%x) from the message",
                             mac32_recv);
      if (mac32 == mac32_recv) {
        isMatched = true;
        Logger::amf_n1().error("Integrity matched");
        // nc.get()->security_ctx->ul_count.seq_num ++;
      }
      if (!isMatched) {
        Logger::amf_n1().error("Received message not integrity matched");
        return;
      } else {
        bstring ciphered = blk2bstr(buf + 7, buf_len - 7);
        if (!nas_message_cipher_protected(nc.get()->security_ctx,
                                          NAS_MESSAGE_UPLINK, ciphered,
                                          decoded_plain_msg)) {
          Logger::amf_n1().error("Decrypt NAS message failure");
          return;
        }
      }
    }
  } break;
  }

  if (nas_data_ind.is_nas_signalling_estab_req) {
    Logger::amf_n1().debug("Received NAS signalling establishment request...");
    // dump_nas_message((uint8_t*)bdata(decoded_plain_msg),
    // blength(decoded_plain_msg));
    print_buffer("amf_n1", "Decoded plain NAS Message buffer",
                 (uint8_t *)bdata(decoded_plain_msg),
                 blength(decoded_plain_msg));
    nas_signalling_establishment_request_handle(
        type, nc, nas_data_ind.ran_ue_ngap_id, nas_data_ind.amf_ue_ngap_id,
        decoded_plain_msg, snn, ulCount);
  } else {
    Logger::amf_n1().debug("Received uplink NAS message...");
    print_buffer("amf_n1", "Decoded NAS message buffer",
                 (uint8_t *)bdata(decoded_plain_msg),
                 blength(decoded_plain_msg));
    uplink_nas_msg_handle(nas_data_ind.ran_ue_ngap_id,
                          nas_data_ind.amf_ue_ngap_id, decoded_plain_msg, plmn);
  }
}

// handlers for lower layer
//------------------------------------------------------------------------------
void amf_n1::nas_signalling_establishment_request_handle(
    SecurityHeaderType type, std::shared_ptr<nas_context> nc,
    uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg,
    std::string snn, uint8_t ulCount) {
  // 1. Create nas context, or update nas context
  if ((nc.get() == nullptr) && (type == PlainNasMsg)) {
    Logger::amf_n1().debug(
        "No existing nas_context with amf_ue_ngap_id 0x%x --> Create a new one",
        amf_ue_ngap_id);
    nc = std::shared_ptr<nas_context>(new nas_context);
    if (!nc.get()) {
      Logger::amf_n1().error(
          "Cannot allocate memory for new nas_context, exit...");
      return;
    }
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
    nc.get()->ctx_avaliability_ind = false;
    // change UE connection status CM-IDLE -> CM-CONNECTED
    nc.get()->nas_status = "CM-CONNECTED";
    nc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
    nc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
    nc.get()->serving_network = snn;
    nc.get()->is_common_procedure_for_authentication_running = false;
    // stacs.UE_connected += 1;
  } else {
     Logger::amf_n1().debug("existing nas_context with amf_ue_ngap_id(0x%x)--> Update",amf_ue_ngap_id); 
    // amf_ue_id_2_nas_context(amf_ue_ngap_id);
  }
    
#if 0
    /**********************   hxs add *******************************/
    string auts;
    octet_stream_2_hex_stream((uint8_t*) bdata(nc.get()->auts), blength(nc.get()->auts), auts);
    std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") +record_id ;
    nlohmann::json udsf_nas_context;
    nlohmann::json udsf_response;
    udsf_nas_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;    
    udsf_nas_context["blocks"] = nlohmann::json::array({
                                                  {{"Content-ID", "ctx_avaliability_ind"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ctx_avaliability_ind)}},
                                                  {{"Content-ID", "is_stacs_available"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->is_stacs_available)}},
                                                  {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->amf_ue_ngap_id)}},
                                                  {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ran_ue_ngap_id)}},
                                                  {{"Content-ID", "nas_status"},{"Content-Type", "varchar(32)"},{"content", nc->nas_status }},
                                                  {{"Content-ID", "_5gmm_state"},{"Content-Type", "varchar(32)"},{"content", to_string( nc->_5gmm_state)}},
                                                  
                                                  {{"Content-ID", "registration_type"},{"Content-Type", "varchar(32)"},{"content", to_string( nc->registration_type)}},
                                                  {{"Content-ID", "follow_on_req_pending_ind"},{"Content-Type", "varchar(32)"},{"content",  to_string( nc->follow_on_req_pending_ind)}},
                                                  {{"Content-ID", "ngKsi"},{"Content-Type", "varchar(32)"},{"content", to_string( nc->ngKsi)}},
                                                  
                                                  {{"Content-ID", "guti"},{"Content-Type", "varchar(32)"},{"content", nc->guti}},

                                                  {{"Content-ID", "imsi"},{"Content-Type", "varchar(32)"},{"content", nc->imsi}},
                                                  {{"Content-ID", "mmCapability"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->mmCapability)}},
                                                  {{"Content-ID", "ueSecurityCaplen"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCaplen)}},
                                                  {{"Content-ID", "ueSecurityCapEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapEnc)}},
                                                  {{"Content-ID", "ueSecurityCapInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapInt)}},
                                                  {{"Content-ID", "ueSecurityCapEEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapEEnc)}},
                                                  {{"Content-ID", "ueSecurityCapEInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapEInt)}},
                                                  {{"Content-ID", "serving_network"},{"Content-Type", "varchar(32)"},{"content",  nc->serving_network}},
                                                  {{"Content-ID", "auts"},{"Content-Type", "varchar(32)"},{"content",  auts}},
                                                  
                                                  {{"Content-ID", "is_specific_procedure_for_registration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_specific_procedure_for_registration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_deregistration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_specific_procedure_for_deregistration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_eCell_inactivity_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_specific_procedure_for_eCell_inactivity_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_authentication_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_common_procedure_for_authentication_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_identification_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_common_procedure_for_identification_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_security_mode_control_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_common_procedure_for_security_mode_control_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_nas_transport_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_common_procedure_for_nas_transport_running)}},
                                                  
                                                  {{"Content-ID", "Href"},{"Content-Type", "varchar(1024)"},{"content",  nc->Href}},
                                                  {{"Content-ID", "is_current_security_available"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_current_security_available)}},
                                                  {{"Content-ID", "registration_attempt_counter"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->registration_attempt_counter)}},
                                                  
                                                  {{"Content-ID", "is_imsi_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_imsi_present)}},
                                                  {{"Content-ID", "is_5g_guti_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_5g_guti_present)}},
                                                  {{"Content-ID", "is_auth_vectors_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->is_auth_vectors_present)}},
                                                  {{"Content-ID", "to_be_register_by_new_suci"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->to_be_register_by_new_suci)}},
                                              });
   // std::vector<nas::SNSSAI_t> requestedNssai
    nlohmann::json requestedNssai = {};
    nlohmann::json requestedNssai_m = {};
    requestedNssai["Content-ID"]="requestedNssai";
    requestedNssai["Content-Type"]="JSON";
    requestedNssai["content"] = {};
    for(int i=0;i<nc->requestedNssai.size();i++)
      {
       requestedNssai_m["sst"]=to_string(nc->requestedNssai[i].sst);
       requestedNssai_m["sd"]=to_string(nc->requestedNssai[i].sd);
       requestedNssai_m["mHplmnSst"]=to_string(nc->requestedNssai[i].mHplmnSst);
       requestedNssai_m["mHplmnSd"]=to_string(nc->requestedNssai[i].mHplmnSd);
       requestedNssai["content"].push_back(requestedNssai_m);

      }
    udsf_nas_context["blocks"].push_back(requestedNssai);

//auc_vector_t _vector[MAX_5GS_AUTH_VECTORS]

    nlohmann::json _vector = {};
    nlohmann::json _vector_m = {};
    _vector["Content-ID"]="_vector";
    _vector["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _vector_m["rand_new"]=to_string(nc->_vector[i].rand_new);
      _vector_m["rand"]=to_string(*(nc->_vector[i].rand));
      _vector_m["xres"]=to_string(*nc->_vector[i].xres);
      _vector_m["autn"]=to_string(*nc->_vector[i].autn);
      _vector_m["kasme"]=to_string(*nc->_vector[i].kasme);
      _vector["content"].push_back(_vector_m);

    }
    udsf_nas_context["blocks"].push_back(_vector);

//_5g_he_av
    nlohmann::json _5g_he_av = {};
    nlohmann::json _5g_he_av_m = {};
    _5g_he_av["Content-ID"]="_5g_he_av";
    _5g_he_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_he_av_m["avType"]=to_string(nc->_5g_he_av[i].avType);
      _5g_he_av_m["rand"]=to_string(*(nc->_5g_he_av[i].rand));
      _5g_he_av_m["xres"]=to_string(*nc->_5g_he_av[i].xres);
      _5g_he_av_m["autn"]=to_string(*nc->_5g_he_av[i].autn);
      _5g_he_av_m["xresStar"]=to_string(*nc->_5g_he_av[i].xresStar);
      _5g_he_av_m["kausf"]=to_string(*nc->_5g_he_av[i].kausf);
      _5g_he_av["content"].push_back(_5g_he_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_he_av);

//_5g_av
    nlohmann::json _5g_av = {};
    nlohmann::json _5g_av_m = {};
    _5g_av["Content-ID"]="_5g_av";
    _5g_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_av_m["avType"]=to_string(nc->_5g_av[i].avType);
      _5g_av_m["rand"]=to_string(*(nc->_5g_av[i].rand));
      _5g_av_m["hxres"]=to_string(*nc->_5g_av[i].hxres);
      _5g_av_m["autn"]=to_string(*nc->_5g_av[i].autn);
      _5g_av_m["hxresStar"]=to_string(*nc->_5g_av[i].hxresStar);
      _5g_av_m["kseaf"]=to_string(*nc->_5g_av[i].kseaf);
      _5g_av["content"].push_back(_5g_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_av);

//kamf
    nlohmann::json kamf = {};
    nlohmann::json kamf_m = {};
    kamf["Content-ID"]="kamf";
    kamf["Content-Type"]="JSON";
    for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++){
      for(int j=0;j<32;j++)
      {
        kamf_m=to_string(nc->kamf[i][j]);
      }
    kamf["content"].push_back(kamf_m);
    }
    udsf_nas_context["blocks"].push_back(kamf);


//security_context_t _security和nas_secu_ctx* security_ctx
  nlohmann::json _security = {};
  nlohmann::json ul_count = {};
  nlohmann::json dl_count = {};
  nlohmann::json capability = {};
  nlohmann::json selected_algorithms = {};
  _security["Content-ID"] = "_security";
  _security["Content-Type"] = "JSON";
  _security["content"]["vector_index"] = to_string(nc->_security.vector_index);
  _security["content"]["knas_enc"] = to_string(*nc->_security.knas_enc);
  _security["content"]["knas_int"] = to_string(*nc->_security.knas_int);
  _security["content"]["activated"] = to_string(nc->_security.activated);
  ul_count["spare"]=to_string(nc->_security.ul_count.spare);
  ul_count["overflow"]=to_string(nc->_security.ul_count.overflow);
  ul_count["seq_num"]=to_string(nc->_security.ul_count.seq_num);
  _security.update(ul_count);
  dl_count["spare"]=to_string(nc->_security.dl_count.spare);
  dl_count["overflow"]=to_string(nc->_security.dl_count.overflow);
  dl_count["seq_num"]=to_string(nc->_security.dl_count.seq_num);
  _security.update(dl_count);
  capability["eps_encryption"]=to_string(nc->_security.capability.eps_encryption);
  capability["eps_integrity"]=to_string(nc->_security.capability.eps_integrity);
  capability["umts_encryption"]=to_string(nc->_security.capability.umts_encryption);
  capability["umts_integrity"]=to_string(nc->_security.capability.umts_integrity);
  capability["gprs_encryption"]=to_string(nc->_security.capability.gprs_encryption);
  capability["umts_present"]=to_string(nc->_security.capability.umts_present);
  capability["gprs_present"]=to_string(nc->_security.capability.gprs_present);
  _security.update(capability);
  selected_algorithms["encryption"]=to_string(nc->_security.selected_algorithms.encryption);
  selected_algorithms["integrity"]=to_string(nc->_security.selected_algorithms.encryption);
  _security.update(selected_algorithms);
  udsf_nas_context["blocks"].push_back(_security);

  nlohmann::json security_ctx = {};
  nlohmann::json ul_count_ctx = {};
  nlohmann::json dl_count_ctx = {};
  nlohmann::json ue_algorithms = {};
  nlohmann::json nas_algs = {};
  security_ctx["Content-ID"] = "security_ctx";
  security_ctx["Content-Type"] = "JSON";
  security_ctx["content"] = {};
  // security_ctx["content"]["vector_pointer"] = to_string(nc->security_ctx->vector_pointer);
  // security_ctx["content"]["sc_type"] = to_string(nc->security_ctx->sc_type);
  // security_ctx["content"]["ngksi"] = to_string(nc->security_ctx->ngksi);
  // security_ctx["content"]["knas_enc"] = to_string(*nc->security_ctx->knas_enc);
  // security_ctx["content"]["knas_int"] = to_string(*nc->security_ctx->knas_int);
  // ul_count_ctx["spare"]=to_string(nc->security_ctx->ul_count.spare);
  // ul_count_ctx["overflow"]=to_string(nc->security_ctx->ul_count.overflow);
  // ul_count_ctx["seq_num"]=to_string(nc->security_ctx->ul_count.seq_num);
  // security_ctx.update(ul_count);
  // dl_count_ctx["spare"]=to_string(nc->security_ctx->dl_count.spare);
  // dl_count_ctx["overflow"]=to_string(nc->security_ctx->dl_count.overflow);
  // dl_count_ctx["seq_num"]=to_string(nc->security_ctx->dl_count.seq_num);
  // security_ctx.update(dl_count);
  // ue_algorithms["_5gs_encryption"]=to_string(nc->security_ctx->ue_algorithms._5gs_encryption);
  // ue_algorithms["_5gs_integrity"]=to_string(nc->security_ctx->ue_algorithms._5gs_integrity);
  // security_ctx.update(ue_algorithms);
  // nas_algs["encryption"]=to_string(nc->security_ctx->nas_algs.encryption);
  // nas_algs["integrity"]=to_string(nc->security_ctx->nas_algs.integrity);
  // security_ctx.update(nas_algs);
  udsf_nas_context["blocks"].push_back(security_ctx);


  amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);

  // udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + std::to_string(nc.get()->amf_ue_ngap_id) ;
  // if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
  // Logger::amf_n2().error("No existing nas context with amf_ue_ngap_id (%d)", nc.get()->amf_ue_ngap_id);
  //   return;
  // }
  // Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());


  // nas_context *nc1 = new nas_context();
  // std::shared_ptr<nas_context> nc2;
  // nc1->nas_context_from_json(udsf_response);
  // nc2 = std::shared_ptr<nas_context>(nc1);

//printf("ran_ue_ngap_id-%d\n",nc2.get()->ran_ue_ngap_id);
//printf("amf_ue_ngap_id-%d\n",nc2.get()->amf_ue_ngap_id);
//printf("ctx_avaliability_ind-%d\n",nc2.get()->ctx_avaliability_ind);
//printf("nas_status-%d\n",nc2.get()->nas_status);
//printf("serving_network-%d\n",nc2.get()->serving_network);

    /**********************   hxs add *******************************/
#endif

  uint8_t *buf = (uint8_t *)bdata(plain_msg);
  uint8_t message_type = *(buf + 2);
  Logger::amf_n1().debug("NAS message type 0x%x", message_type);
  switch (message_type) {
  case REGISTRATION_REQUEST: {
    Logger::amf_n1().debug(
        "Received registration request message, handling...");
    registration_request_handle(true, nc, ran_ue_ngap_id, amf_ue_ngap_id, snn,
                                plain_msg);
  } break;
  case SERVICE_REQUEST: {
    Logger::amf_n1().debug("Received service request message, handling...");
    if(nc != nullptr)
    {
      printf("--------service ------%x\n",nc.get()->security_ctx->ul_count.seq_num);
      nc.get()->security_ctx->ul_count.seq_num = ulCount;
    }
    printf("--------service --33333----%x\n",nc.get()->security_ctx->ul_count.seq_num);
    service_request_handle(true, nc, ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case UE_INIT_DEREGISTER: {
    Logger::amf_n1().debug(
        "received initialUeMessage de-registration request messgae , handle "
        "...");
    // ue_initiate_de_registration_handle(ran_ue_ngap_id, amf_ue_ngap_id,
    // plain_msg);
  } break;
  default:
    Logger::amf_n1().error("No handler for NAS message 0x%x", message_type);
  }
}

//------------------------------------------------------------------------------
void amf_n1::uplink_nas_msg_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id,
                                   bstring plain_msg) {
  uint8_t *buf = (uint8_t *)bdata(plain_msg);
  uint8_t message_type = *(buf + 2);
  switch (message_type) {
  case AUTHENTICATION_RESPONSE: {
    Logger::amf_n1().debug(
        "Received authentication response message, handling...");
    authentication_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case AUTHENTICATION_FAILURE: {
    Logger::amf_n1().debug(
        "Received authentication failure message, handling...");
    authentication_failure_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case SECURITY_MODE_COMPLETE: {
    Logger::amf_n1().debug(
        "Received security mode complete message, handling...");
    security_mode_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case SECURITY_MODE_REJECT: {
    Logger::amf_n1().debug(
        "Received security mode reject message, handling...");
    security_mode_reject_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case UL_NAS_TRANSPORT: {
    Logger::amf_n1().debug("Received UL NAS transport message, handling...");
    ul_nas_transport_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case UE_INIT_DEREGISTER: {
    Logger::amf_n1().debug(
        "Received de-registration request message, handling...");
    ue_initiate_de_registration_handle(ran_ue_ngap_id, amf_ue_ngap_id,
                                       plain_msg);
  } break;
  case IDENTITY_RESPONSE: {
    Logger::amf_n1().debug("received identity response messgae , handle ...");
    identity_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case REGISTRATION_COMPLETE: {
    Logger::amf_n1().debug(
        "Received registration complete message, handling...");
    registration_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    // TODO
  } break;
  default: {
    // TODO:
  }
  }
}

//------------------------------------------------------------------------------
void amf_n1::uplink_nas_msg_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id,
                                   bstring plain_msg, plmn_t plmn) {
  uint8_t *buf = (uint8_t *)bdata(plain_msg);
  uint8_t message_type = *(buf + 2);
  switch (message_type) {
  case AUTHENTICATION_RESPONSE: {
    Logger::amf_n1().debug(
        "Received authentication response message, handling...");
    authentication_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case AUTHENTICATION_FAILURE: {
    Logger::amf_n1().debug(
        "Received authentication failure message, handling...");
    authentication_failure_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case SECURITY_MODE_COMPLETE: {
    Logger::amf_n1().debug(
        "Received security mode complete message, handling...");
    security_mode_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case SECURITY_MODE_REJECT: {
    Logger::amf_n1().debug(
        "Received security mode reject message, handling...");
    security_mode_reject_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case UL_NAS_TRANSPORT: {
    Logger::amf_n1().debug("Received ul NAS transport message, handling...");
    ul_nas_transport_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg, plmn);
  } break;
  case UE_INIT_DEREGISTER: {
    Logger::amf_n1().debug(
        "Received de-registration request message, handling...");
    ue_initiate_de_registration_handle(ran_ue_ngap_id, amf_ue_ngap_id,
                                       plain_msg);
  } break;
  case IDENTITY_RESPONSE: {
    Logger::amf_n1().debug("received identity response messgae , handle ...");
    identity_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
  } break;
  case REGISTRATION_COMPLETE: {
    Logger::amf_n1().debug(
        "Received registration complete message, handling...");
    registration_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    // TODO
  } break;
  default: {
    // TODO:
  }
  }
}

// nas message decode
//------------------------------------------------------------------------------
bool amf_n1::check_security_header_type(SecurityHeaderType &type,
                                        uint8_t *buffer) {
  uint8_t octet = 0, decoded_size = 0;
  octet = *(buffer + decoded_size);
  decoded_size++;
  if (octet != 0x7e)
    return false;
  octet = *(buffer + decoded_size);
  decoded_size++;
  // TODO: remove hardcoded value
  switch (octet & 0x0f) {
  case 0x0:
    type = PlainNasMsg;
    break;
  case 0x1:
    type = IntegrityProtected;
    break;
  case 0x2:
    type = IntegrityProtectedAndCiphered;
    break;
  case 0x3:
    type = IntegrityProtectedWithNew5GNASSecurityContext;
    break;
  case 0x4:
    type = IntegrityProtectedAndCipheredWithNew5GNASSecurityContext;
    break;
  }
  return true;
}

// nas message handlers

//------------------------------------------------------------------------------
void amf_n1::identity_response_handle(uint32_t ran_ue_ngap_id,
                                      long amf_ue_ngap_id, bstring plain_msg) {
  IdentityResponse *ir = new IdentityResponse();
  if (!ir->decodefrombuffer(NULL, (uint8_t *)bdata(plain_msg),
                            blength(plain_msg))) {
    Logger::amf_n1().error("decoding identity response error");
    return;
  }
  string supi = "";
  if (ir->ie_mobility_id) {
    nas::SUCI_imsi_t imsi;
    ir->ie_mobility_id->getSuciWithSupiImsi(imsi);
    supi = imsi.mcc + imsi.mnc + imsi.msin;
    Logger::amf_n1().debug("identity response : suci (%s)", supi.c_str());
  }
  std::shared_ptr<nas_context> nc;
  if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id)) {
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
    Logger::amf_n1().debug("find nas_context(%p) by amf_ue_ngap_id(%d)",
                           nc.get(), amf_ue_ngap_id);
  } else {
    nc = std::shared_ptr<nas_context>(new nas_context);
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
    nc.get()->ctx_avaliability_ind = false;
  }
  nc.get()->ctx_avaliability_ind = true;
  nc.get()->nas_status = "CM-CONNECTED";
  nc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  nc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
  nc.get()->is_imsi_present = true;
  nc.get()->imsi = supi;
/***************************hsx add************************/
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;

  udsf_nas_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({amf_ue_ngap_id})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;

  nlohmann::json ctx_avaliability_ind;
  ctx_avaliability_ind["Content-ID"] = "ctx_avaliability_ind";
  ctx_avaliability_ind["Content-Type"] = "JSON";
  ctx_avaliability_ind["content"] = nc.get()->ctx_avaliability_ind;            
  udsf_nas_context["blocks"].push_back(ctx_avaliability_ind); 

  nlohmann::json nas_status;
  nas_status["Content-ID"] = "nas_status";
  nas_status["Content-Type"] = "JSON";
  nas_status["content"] = nc.get()->nas_status;            
  udsf_nas_context["blocks"].push_back(nas_status); 

  nlohmann::json amf_ue_ngap_id_m;
  amf_ue_ngap_id_m["Content-ID"] = "amf_ue_ngap_id";
  amf_ue_ngap_id_m["Content-Type"] = "JSON";
  amf_ue_ngap_id_m["content"] = nc.get()->amf_ue_ngap_id;            
  udsf_nas_context["blocks"].push_back(amf_ue_ngap_id_m);

  nlohmann::json ran_ue_ngap_id_m;
  ran_ue_ngap_id_m["Content-ID"] = "ran_ue_ngap_id";
  ran_ue_ngap_id_m["Content-Type"] = "JSON";
  ran_ue_ngap_id_m["content"] = nc.get()->ran_ue_ngap_id;            
  udsf_nas_context["blocks"].push_back(ran_ue_ngap_id_m); 

  nlohmann::json is_imsi_present;
  is_imsi_present["Content-ID"] = "is_imsi_present";
  is_imsi_present["Content-Type"] = "JSON";
  is_imsi_present["content"] = nc.get()->is_imsi_present;            
  udsf_nas_context["blocks"].push_back(is_imsi_present); 

  nlohmann::json imsi;
  imsi["Content-ID"] = "imsi";
  imsi["Content-Type"] = "JSON";
  imsi["content"] = nc.get()->imsi;            
  udsf_nas_context["blocks"].push_back(imsi); 

  std::string json_part = udsf_nas_context.dump();
  //nlohmann::json udsf_response;
  amf_n2_inst->curl_http_client_udsf(udsf_url,json_part,"PUT",udsf_response);

/***************************hsx add************************/
  supi2amfId[("imsi-" + nc.get()->imsi)] = amf_ue_ngap_id;
  supi2ranId[("imsi-" + nc.get()->imsi)] = ran_ue_ngap_id;
  if (nc.get()->to_be_register_by_new_suci) {
    run_registration_procedure(nc);
  }
}

//------------------------------------------------------------------------------
void amf_n1::service_request_handle(bool isNasSig,
                                    std::shared_ptr<nas_context> nc,
                                    uint32_t ran_ue_ngap_id,
                                    long amf_ue_ngap_id, bstring nas) {


  printf("---------------stateless-----service-----------\n");
  if (!nc.get()) {
    // service reject
    uint8_t nas[4];
    nas[0] = EPD_5GS_MM_MSG;
    nas[1] = PLAIN_5GS_MSG;
    nas[2] = SERVICE_REJECT;
    nas[3] = _5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED;
    itti_dl_nas_transport *dnt =
        new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas = blk2bstr(nas, 4);
    dnt->amf_ue_ngap_id = amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = ran_ue_ngap_id;
    std::shared_ptr<itti_dl_nas_transport> i =
        std::shared_ptr<itti_dl_nas_transport>(dnt);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          i->get_msg_name());
    }
    return;
  }

  set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
  nas_secu_ctx *secu = nc.get()->security_ctx;
  ServiceRequest *serReq = new ServiceRequest();
  serReq->decodefrombuffer(nullptr, (uint8_t *)bdata(nas), blength(nas));
  bdestroy(nas);
  uint16_t pdu_session_status = 0xffff;
  bstring nas_container;
  Logger::amf_n1().debug(
      "try to decode pdu-session-status in service_request(service_request)");
  if (serReq->getNasMessageContainer(nas_container)) {
    Logger::amf_n1().debug(
        "try to get pdu-session-status value from nas message "
        "container(length: %d)",
        blength(nas_container));
    uint8_t *buf = (uint8_t *)bdata(nas_container);
    ServiceRequest *tmp = new ServiceRequest();
    tmp->decodefrombuffer(nullptr, (uint8_t *)bdata(nas_container),
                          blength(nas_container));
    pdu_session_status = tmp->getPduSessionStatus();
    Logger::amf_n1().debug("Get pdu-session-status value: 0x%02x",
                           pdu_session_status);
  }
  ServiceAccept *serApt = new ServiceAccept();
  serApt->setHeader(PLAIN_5GS_MSG);
  string supi = "imsi-" + nc.get()->imsi;
  supi2amfId[supi] = amf_ue_ngap_id;
  supi2ranId[supi] = ran_ue_ngap_id;
  Logger::amf_n1().debug("amf_ue_ngap_id %d, ran_ue_ngap_id %d", amf_ue_ngap_id,
                         ran_ue_ngap_id);
  Logger::amf_n1().debug("Key for pdu session context: SUPI %s", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;
  std::string record_id = "RECORD_ID=\'" + supi + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
  bool is_supi_to_pdu_ctx_udsf = amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response);
  if(!is_supi_to_pdu_ctx_udsf){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      psc.get()->pdu_session_context_from_json(udsf_response);
      //psc = std::shared_ptr<pdu_session_context>(psc1);
  }
  psc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
  psc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  //***************************stateless

  // if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
  //   psc = amf_n11_inst->supi_to_pdu_ctx(supi);
  //   if (!psc) {
  //     Logger::amf_n1().error("Cannot get pdu_session_context");
  //     delete serReq;
  //     delete serApt;
  //     return;
  //   }
  //   psc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
  //   psc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  // } else {
  //   Logger::amf_n1().error("Cannot get pdu_session_context with SUPI %s",
  //                          supi.c_str());
  // }
  if (!is_supi_to_pdu_ctx_udsf || !psc.get()->isn2sm_avaliable) {
  //if (!amf_n11_inst->is_supi_to_pdu_ctx(supi) || !psc.get()->isn2sm_avaliable) {
    Logger::amf_n1().error("Cannot get pdu session information with supi(%s)",
                           supi.c_str());
    //if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
    if (is_supi_to_pdu_ctx_udsf) {
      psc.get()->isn2sm_avaliable = true;
    }
    serApt->setPDU_session_status(0x0000);
    uint8_t buffer[BUFFER_SIZE_256];
    int encoded_size = serApt->encode2buffer(buffer, BUFFER_SIZE_256);
    bstring protectedNas;
    encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                                 NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                                 protectedNas);
    uint8_t *kamf = nc.get()->kamf[secu->vector_pointer];
    uint8_t kgnb[32];
    uint32_t ulcount = secu->ul_count.seq_num | (secu->ul_count.overflow << 8);
    Logger::amf_n1().debug("uplink count(%d)", secu->ul_count.seq_num);
    print_buffer("amf_n1", "kamf", kamf, 32);
    Authentication_5gaka::derive_kgnb(ulcount, 0x01, kamf, kgnb);
    bstring kgnb_bs = blk2bstr(kgnb, 32);

    std::string record_id = "RECORD_ID=\'" + supi + "\'";
    std::string udsf_put_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
      nlohmann::json udsf_put_pdu_session_context;
    // nlohmann::json udsf_response;
      udsf_put_pdu_session_context["meta"] ["tags"] = {
                                          {"RECORD_ID",nlohmann::json::array({supi})},
                                          {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                          } ;
      udsf_put_pdu_session_context["blocks"] = nlohmann::json::array({
        {{"Content-ID", "isn2sm_avaliable"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->isn2sm_avaliable)}},
        {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->ran_ue_ngap_id)}},
        {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->amf_ue_ngap_id)}}
        });
      std::string json_part = udsf_put_pdu_session_context.dump();
      amf_n2_inst->curl_http_client_udsf(udsf_put_url,json_part,"PUT",udsf_response);


    itti_initial_context_setup_request *itti_msg =
        new itti_initial_context_setup_request(TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->nas = protectedNas;
    itti_msg->kgnb = kgnb_bs;
    itti_msg->is_sr = true; // service request indicator
    itti_msg->is_pdu_exist = false;
    std::shared_ptr<itti_initial_context_setup_request> i =
        std::shared_ptr<itti_initial_context_setup_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          i->get_msg_name());
    }
    delete serApt;
    delete serReq;
    return;
  }
  if (pdu_session_status == 0x0000) {
    serApt->setPDU_session_status(0x0000);
  } else {
    //serApt->setPDU_session_status(0x0000);
    serApt->setPDU_session_status(pdu_session_status);
    Logger::amf_n1().debug("setting pdu session status 0x%02x",
                           htonl(pdu_session_status));
    // serApt->setPDU_session_status(0x2000);
  }
  serApt->setPDU_session_reactivation_result(0x0000);
  uint8_t buffer[BUFFER_SIZE_256];
  int encoded_size = serApt->encode2buffer(buffer, BUFFER_SIZE_256);
  bstring protectedNas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                               protectedNas);
  uint8_t *kamf = nc.get()->kamf[secu->vector_pointer];
  uint8_t kgnb[32];
  uint32_t ulcount = secu->ul_count.seq_num | (secu->ul_count.overflow << 8);
  Logger::amf_n1().debug("uplink count(%d)", secu->ul_count.seq_num);
  print_buffer("amf_n1", "kamf", kamf, 32);
  Authentication_5gaka::derive_kgnb(ulcount, 0x01, kamf, kgnb);
  bstring kgnb_bs = blk2bstr(kgnb, 32);
  itti_initial_context_setup_request *itti_msg =
      new itti_initial_context_setup_request(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
  itti_msg->nas = protectedNas;
  itti_msg->kgnb = kgnb_bs;
  itti_msg->is_sr = true; // service request indicator
  itti_msg->pdu_session_id = psc.get()->pdu_session_id;
  itti_msg->is_pdu_exist = true;
  if (psc.get()->isn2sm_avaliable) {
    itti_msg->n2sm = psc.get()->n2sm;
    itti_msg->isn2sm_avaliable = true;
  } else {
    itti_msg->isn2sm_avaliable = false;
    Logger::amf_n1().error("Cannot get pdu session information");
  }
  record_id = "RECORD_ID=\'" + supi + "\'";
  std::string udsf_put_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
      nlohmann::json udsf_put_pdu_session_context;
    // nlohmann::json udsf_response;
      udsf_put_pdu_session_context["meta"] ["tags"] = {
                                          {"RECORD_ID",nlohmann::json::array({supi})},
                                          {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                          } ;
      udsf_put_pdu_session_context["blocks"] = nlohmann::json::array({
        {{"Content-ID", "isn2sm_avaliable"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->isn2sm_avaliable)}},
        {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->ran_ue_ngap_id)}},
        {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->amf_ue_ngap_id)}}
        });
      std::string json_part = udsf_put_pdu_session_context.dump();
      amf_n2_inst->curl_http_client_udsf(udsf_put_url,json_part,"PUT",udsf_response);



  std::shared_ptr<itti_initial_context_setup_request> i =
      std::shared_ptr<itti_initial_context_setup_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error("Could not send ITTI message %s to task TASK_AMF_N2",
                           i->get_msg_name());
  }
  delete serApt;
  delete serReq;
}

//------------------------------------------------------------------------------
void amf_n1::registration_request_handle(bool isNasSig,
                                         std::shared_ptr<nas_context> nc,
                                         uint32_t ran_ue_ngap_id,
                                         long amf_ue_ngap_id, std::string snn,
                                         bstring reg) {
  // Decode registration request message
#if 0
  nlohmann::json imsi_json;
  nlohmann::json is_stacs_available;
  nlohmann::json is_5g_guti_present;
  nlohmann::json is_auth_vectors_present;
  nlohmann::json is_current_security_available;
  nlohmann::json security_ctx;
  nlohmann::json ctx_avaliability_ind;
  nlohmann::json nas_status_json;
  nlohmann::json amf_ue_ngap_id_json;
  nlohmann::json ran_ue_ngap_id_json;
  nlohmann::json serving_network;
  nlohmann::json to_be_register_by_new_suci;
  nlohmann::json ngKsi;
  nlohmann::json registration_type;
  nlohmann::json follow_on_req_pending_ind;
  nlohmann::json mmCapability;
  nlohmann::json ueSecurityCapEnc;
  nlohmann::json ueSecurityCapInt;
  nlohmann::json ueSecurityCaplen;
  nlohmann::json requestedNssai_json;
  nlohmann::json udsf_nas_context;
  nlohmann::json is_imsi_present;
  nlohmann::json _5gmm_state;
  nlohmann::json ueSecurityCapEEnc;
  nlohmann::json ueSecurityCapEInt;
#endif 
  RegistrationRequest *regReq = new RegistrationRequest();
  regReq->decodefrombuffer(nullptr, (uint8_t *)bdata(reg), blength(reg));
  bdestroy(reg); // free buffer
  // Check 5gs Mobility Identity (Mandatory IE)
  std::string guti;
  uint8_t mobility_id_type = regReq->getMobilityIdentityType();
  switch (mobility_id_type) {
  case SUCI: {
    nas::SUCI_imsi_t imsi;
    if (!regReq->getSuciSupiFormatImsi(imsi)) {
      Logger::amf_n1().warn("No SUCI and IMSI for SUPI Format");
    } else {
      if (!nc.get()) {
        nc = std::shared_ptr<nas_context>(new nas_context);
        set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
        nc.get()->ctx_avaliability_ind = false;
        // change UE connection status CM-IDLE -> CM-CONNECTED
        nc.get()->nas_status = "CM-CONNECTED";
        nc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
        nc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
        nc.get()->serving_network = snn;
#if 0
        ctx_avaliability_ind["Content-ID"] = "ctx_avaliability_ind";
        ctx_avaliability_ind["Content-Type"] = "varchar(32)";
        ctx_avaliability_ind["content"] =to_string(nc.get()->ctx_avaliability_ind);
        udsf_nas_context["blocks"].push_back(ctx_avaliability_ind);
        
        nas_status_json["Content-ID"] = "nas_status";
        nas_status_json["Content-Type"] = "varchar(32)";
        nas_status_json["content"] =(nc.get()->nas_status);
        udsf_nas_context["blocks"].push_back(nas_status_json);
        
        amf_ue_ngap_id_json["Content-ID"] = "amf_ue_ngap_id";
        amf_ue_ngap_id_json["Content-Type"] = "varchar(32)";
        amf_ue_ngap_id_json["content"] =to_string(nc.get()->amf_ue_ngap_id);
        udsf_nas_context["blocks"].push_back(amf_ue_ngap_id_json);
        
        ran_ue_ngap_id_json["Content-ID"] = "ran_ue_ngap_id";
        ran_ue_ngap_id_json["Content-Type"] = "varchar(32)";
        ran_ue_ngap_id_json["content"] =to_string(nc.get()->ran_ue_ngap_id);
        udsf_nas_context["blocks"].push_back(ran_ue_ngap_id_json);
        
        serving_network["Content-ID"] = "serving_network";
        serving_network["Content-Type"] = "varchar(32)";
        serving_network["content"] =(nc.get()->serving_network);
        udsf_nas_context["blocks"].push_back(serving_network);
#endif
      }
      nc.get()->is_imsi_present = true;
      nc.get()->imsi = imsi.mcc + imsi.mnc + imsi.msin;
      Logger::amf_n1().debug("Received IMSI %s", nc.get()->imsi.c_str());
      supi2amfId[("imsi-" + nc.get()->imsi)] = amf_ue_ngap_id;
      supi2ranId[("imsi-" + nc.get()->imsi)] = ran_ue_ngap_id;
      // try to find old nas_context and release
      std::shared_ptr<nas_context> old_nc =
          imsi2nas_context[("imsi-" + nc.get()->imsi)];
      if (old_nc.get()) {
        // release
        old_nc.reset();
      }
      imsi2nas_context[("imsi-" + nc.get()->imsi)] = nc;
      Logger::amf_n1().info("Associating IMSI (%s) with nas_context (%p)",
                            ("imsi-" + nc.get()->imsi).c_str(), nc.get());
#if 0                            
      is_imsi_present["Content-ID"] = "is_imsi_present";
      is_imsi_present["Content-Type"] = "varchar(32)";
      is_imsi_present["content"] = to_string(nc.get()->is_imsi_present);
      udsf_nas_context["blocks"].push_back(is_imsi_present);
      imsi_json["Content-ID"] = "imsi";
      imsi_json["Content-Type"] = "varchar(32)";
      imsi_json["content"] = (nc.get()->imsi);
      udsf_nas_context["blocks"].push_back(imsi_json);
#endif
      if (!nc.get()->is_stacs_available) {
        string ue_context_key = "app_ue_ranid_" + to_string(ran_ue_ngap_id) +
                                "-amfid_" + to_string(amf_ue_ngap_id);
        std::shared_ptr<ue_context> uc;
        // uc = amf_app_inst->amf_ue_id_2_ue_context(amf_ue_ngap_id);
        Logger::amf_n1().info(
            "Try to find ue_context in amf_app using ran_amf_id (%s)",
            ue_context_key.c_str());

        uc = amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
        ue_info_t ueItem;
        ueItem.connStatus = "5GMM-CONNECTED"; //"CM-CONNECTED";
        ueItem.registerStatus =
            "5GMM-REG-INITIATED"; // 5GMM-COMMON-PROCEDURE-INITIATED
        ueItem.ranid = ran_ue_ngap_id;
        ueItem.amfid = amf_ue_ngap_id;
        ueItem.imsi = nc.get()->imsi;
        ueItem.mcc = uc.get()->cgi.mcc;
        ueItem.mnc = uc.get()->cgi.mnc;
        ueItem.cellId = uc.get()->cgi.nrCellID;

        stacs.update_ue_info(ueItem);
        set_5gmm_state(nc, _5GMM_COMMON_PROCEDURE_INITIATED);
        nc.get()->is_stacs_available = true;
#if 0
        is_stacs_available["Content-ID"] = "is_stacs_available";
        is_stacs_available["Content-Type"] = "varchar(32)";
        is_stacs_available["content"] = to_string(nc.get()->is_stacs_available);
        udsf_nas_context["blocks"].push_back(is_stacs_available);

        _5gmm_state["Content-ID"] = "_5gmm_state";
        _5gmm_state["Content-Type"] = "varchar(32)";
        _5gmm_state["content"] = to_string(nc.get()->_5gmm_state);
        udsf_nas_context["blocks"].push_back(_5gmm_state);
#endif
          /*
           bool add_new_ue = true;
           for(int i=0; i< stacs.ues.size(); i++){
           if(!stacs.ues[i].imsi.compare(nc.get()->imsi)){
           stacs.ues[i].connStatus = ueItem.connStatus;
           stacs.ues[i].registerStatus = ueItem.registerStatus;
           stacs.ues[i].ranid = ueItem.ranid;
           stacs.ues[i].amfid = ueItem.amfid;
           stacs.ues[i].guti = ueItem.guti;
           stacs.ues[i].mcc = ueItem.mcc;
           stacs.ues[i].mnc = ueItem.mnc;
           stacs.ues[i].cellId = ueItem.cellId;
           add_new_ue = false;
           break;
           }
           }
           if(add_new_ue)
           stacs.ues.push_back(ueItem);
           */
      }
          // nc.get()->imsi = //need interface to transfer SUCI_imsi_t to string
    }
  } break;
  case _5G_GUTI: {
    guti = regReq->get_5g_guti();
    Logger::amf_n1().debug("Decoded GUTI from registeration request message");
    if (!guti.compare("error")) {
      Logger::amf_n1().warn("No GUTI IE");
    }
    if (nc.get()) {
      nc.get()->is_5g_guti_present = true;
      nc.get()->to_be_register_by_new_suci = true;
#if 0
      is_5g_guti_present["Content-ID"] = "is_5g_guti_present";
      is_5g_guti_present["Content-Type"] = "varchar(32)";
      is_5g_guti_present ["content"] = to_string( nc.get()->is_5g_guti_present);
      udsf_nas_context["blocks"].push_back(is_5g_guti_present);
      
      to_be_register_by_new_suci["Content-ID"] = "to_be_register_by_new_suci";
      to_be_register_by_new_suci["Content-Type"] = "varchar(32)";
      to_be_register_by_new_suci ["content"] = to_string( nc.get()->to_be_register_by_new_suci);
      udsf_nas_context["blocks"].push_back(to_be_register_by_new_suci);
#endif
    } 
    // else if(is_guti_2_nas_context_in_udsf(guti))
    // {
    //    nc = guti_2_nas_context_in_udsf(guti);

    else if (is_guti_2_nas_context(guti))
    {
      nc = guti_2_nas_context(guti);
      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      supi2amfId[("imsi-" + nc.get()->imsi)] = amf_ue_ngap_id;
      supi2ranId[("imsi-" + nc.get()->imsi)] = ran_ue_ngap_id;
      nc.get()->is_auth_vectors_present = false;
      nc.get()->is_current_security_available = false;
      nc.get()->security_ctx->sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
#if 0
      is_auth_vectors_present["Content-ID"] = "is_auth_vectors_present";
      is_auth_vectors_present["Content-Type"] = "varchar(32)";
      is_auth_vectors_present ["content"] = to_string( nc.get()->is_auth_vectors_present);
      udsf_nas_context["blocks"].push_back(is_auth_vectors_present);
      
      is_current_security_available["Content-ID"] = "is_current_security_available";
      is_current_security_available["Content-Type"] = "varchar(32)";
      is_current_security_available ["content"] = to_string( nc.get()->is_current_security_available);
      udsf_nas_context["blocks"].push_back(is_current_security_available);
      
      security_ctx["Content-ID"] = "security_ctx";
      security_ctx["Content-Type"] = "JSON";
      security_ctx ["content"]["sc_type"] = to_string( nc.get()->security_ctx->sc_type);
      udsf_nas_context["blocks"].push_back(security_ctx);
#endif
    } else {
      Logger::amf_n1().debug(
          "No existing nas_context with amf_ue_ngap_id(0x%x) --> Create new "
          "one",
          amf_ue_ngap_id);
      nc = std::shared_ptr<nas_context>(new nas_context);
      if (!nc.get()) {
        Logger::amf_n1().error(
            "Cannot allocate memory for new nas_context, exit...");
        return;
      }
      Logger::amf_n1().info(
          "Created new nas_context (%p) associated with amf_ue_ngap_id (%d) "
          "for nas_signalling_establishment_request",
          nc.get(), amf_ue_ngap_id);
      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      nc.get()->ctx_avaliability_ind = false;
      // change UE connection status CM-IDLE -> CM-CONNECTED
      nc.get()->nas_status = "CM-CONNECTED";
      nc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
      nc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
      nc.get()->serving_network = snn;
      nc.get()->is_5g_guti_present = true;
      nc.get()->to_be_register_by_new_suci = true;
      nc.get()->ngKsi = 100;
      // supi2amfId[("imsi-"+nc.get()->imsi)] = amf_ue_ngap_id;
      // supi2ranId[("imsi-"+nc.get()->imsi)] = ran_ue_ngap_id;
#if 0
      ctx_avaliability_ind["Content-ID"] = "ctx_avaliability_ind";
      ctx_avaliability_ind["Content-Type"] = "varchar(32)";
      ctx_avaliability_ind ["content"] = to_string( nc.get()->ctx_avaliability_ind);
      udsf_nas_context["blocks"].push_back(ctx_avaliability_ind);

      
      nas_status_json["Content-ID"] = "nas_status";
      nas_status_json["Content-Type"] = "varchar(32)";
      nas_status_json["content"] = (nc.get()->nas_status);
      udsf_nas_context["blocks"].push_back(nas_status_json);

      
      amf_ue_ngap_id_json["Content-ID"] = "amf_ue_ngap_id";
      amf_ue_ngap_id_json["Content-Type"] = "varchar(32)";
      amf_ue_ngap_id_json ["content"] = to_string( nc.get()->amf_ue_ngap_id);
      udsf_nas_context["blocks"].push_back(amf_ue_ngap_id_json);

      
      ran_ue_ngap_id_json["Content-ID"] = "ran_ue_ngap_id";
      ran_ue_ngap_id_json["Content-Type"] = "varchar(32)";
      ran_ue_ngap_id_json ["content"] = to_string( nc.get()->ran_ue_ngap_id);
      udsf_nas_context["blocks"].push_back(ran_ue_ngap_id_json);

      
      serving_network["Content-ID"] = "serving_network";
      serving_network["Content-Type"] = "varchar(32)";
      serving_network ["content"] = ( nc.get()->serving_network);
      udsf_nas_context["blocks"].push_back(serving_network);

     
      is_5g_guti_present["Content-ID"] = "is_5g_guti_present";
      is_5g_guti_present["Content-Type"] = "varchar(32)";
      is_5g_guti_present ["content"] = to_string( nc.get()->is_5g_guti_present);
      udsf_nas_context["blocks"].push_back(is_5g_guti_present);

      
      to_be_register_by_new_suci["Content-ID"] = "to_be_register_by_new_suci";
      to_be_register_by_new_suci["Content-Type"] = "varchar(32)";
      to_be_register_by_new_suci ["content"] = to_string( nc.get()->to_be_register_by_new_suci);
      udsf_nas_context["blocks"].push_back(to_be_register_by_new_suci);

      
      ngKsi["Content-ID"] = "ngKsi";
      ngKsi["Content-Type"] = "varchar(32)";
      ngKsi ["content"] = to_string( nc.get()->ngKsi);
      udsf_nas_context["blocks"].push_back(ngKsi);
#endif
    }
  } break;
  }

  // Create NAS context
  if (nc.get() == nullptr) {
    // try to get the GUTI -> nas_context
    // if(is_guti_2_nas_context_in_udsf(guti))
    // {
    //    nc = guti_2_nas_context_in_udsf(guti);

    if (is_guti_2_nas_context(guti)) {
      nc = guti_2_nas_context(guti);
      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      supi2amfId[("imsi-" + nc.get()->imsi)] = amf_ue_ngap_id;
      supi2ranId[("imsi-" + nc.get()->imsi)] = ran_ue_ngap_id;

      // test  5g guti //
      nc.get()->is_auth_vectors_present = false;
      nc.get()->is_current_security_available = false;
      nc.get()->security_ctx->sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
#if 0
      is_auth_vectors_present["Content-ID"] = "is_auth_vectors_present";
      is_auth_vectors_present["Content-Type"] = "varchar(32)";
      is_auth_vectors_present ["content"] = to_string( nc.get()->is_auth_vectors_present);
      udsf_nas_context["blocks"].push_back(is_auth_vectors_present);

      is_current_security_available["Content-ID"] = "is_current_security_available";
      is_current_security_available["Content-Type"] = "varchar(32)";
      is_current_security_available ["content"] = to_string( nc.get()->is_current_security_available);
      udsf_nas_context["blocks"].push_back(is_current_security_available);

      security_ctx["Content-ID"] = "security_ctx";
      security_ctx["Content-Type"] = "JSON";
      security_ctx["content"]["sc_type"] = nc.get()->security_ctx->sc_type;
      udsf_nas_context["blocks"].push_back(security_ctx);
#endif
    } else {
      Logger::amf_n1().error("No nas_context with GUTI (%s)", guti.c_str());
      response_registration_reject_msg(
          _5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED, ran_ue_ngap_id,
          amf_ue_ngap_id);
      // release ue_ngap_context and ue_context
      string ue_context_key = "app_ue_ranid_" + to_string(ran_ue_ngap_id) +
                              "-amfid_" + to_string(amf_ue_ngap_id);
      std::shared_ptr<ue_context> uc =
          amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
      if (uc.get())
        uc.reset();
      std::shared_ptr<ue_ngap_context> unc =
          amf_n2_inst->ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id);
      if (unc.get())
        unc.reset();
      delete regReq;
      return;
    }
  } else {
    Logger::amf_n1().debug("Existing nas_context --> Update");
    // nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  }
  nc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
  nc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  nc.get()->serving_network = snn;
#if 0
  amf_ue_ngap_id_json["Content-ID"] = "amf_ue_ngap_id";
  amf_ue_ngap_id_json["Content-Type"] = "varchar(32)";
  amf_ue_ngap_id_json["content"] =to_string(nc.get()->amf_ue_ngap_id);
  udsf_nas_context["blocks"].push_back(amf_ue_ngap_id_json);
  
  ran_ue_ngap_id_json["Content-ID"] = "ran_ue_ngap_id";
  ran_ue_ngap_id_json["Content-Type"] = "varchar(32)";
  ran_ue_ngap_id_json["content"] =to_string(nc.get()->ran_ue_ngap_id);
  udsf_nas_context["blocks"].push_back(ran_ue_ngap_id_json);
  
  serving_network["Content-ID"] = "serving_network";
  serving_network["Content-Type"] = "varchar(32)";
  serving_network["content"] =(nc.get()->serving_network);
  udsf_nas_context["blocks"].push_back(serving_network);
#endif
  // Check 5GS_Registration_type IE (Mandatory IE)
  uint8_t reg_type;
  bool is_follow_on_req_pending;
  if (!regReq->get5GSRegistrationType(is_follow_on_req_pending, reg_type)) {
    Logger::amf_n1().error("Missing Mandatory IE 5GS Registration type...");
    response_registration_reject_msg(_5GMM_CAUSE_INVALID_MANDATORY_INFO,
                                     ran_ue_ngap_id, amf_ue_ngap_id);
    delete regReq;

    return;
  }
  nc.get()->registration_type = reg_type;
  nc.get()->follow_on_req_pending_ind = is_follow_on_req_pending;
#if 0
  registration_type["Content-ID"] = "registration_type";
  registration_type["Content-Type"] = "varchar(32)";
  registration_type["content"] =to_string(nc.get()->registration_type);
  udsf_nas_context["blocks"].push_back(registration_type);
  
  follow_on_req_pending_ind["Content-ID"] = "follow_on_req_pending_ind";
  follow_on_req_pending_ind["Content-Type"] = "varchar(32)";
  follow_on_req_pending_ind["content"] =to_string(nc.get()->follow_on_req_pending_ind);
  udsf_nas_context["blocks"].push_back(follow_on_req_pending_ind);
#endif
  // Check ngKSI (Mandatory IE)
  uint8_t ngKSI = regReq->getngKSI();
  if (ngKSI == -1) {
    Logger::amf_n1().error("Missing Mandatory IE ngKSI...");
    response_registration_reject_msg(_5GMM_CAUSE_INVALID_MANDATORY_INFO,
                                     ran_ue_ngap_id, amf_ue_ngap_id);
    delete regReq;
    return;
  }
  nc.get()->ngKsi = ngKSI;

#if 0
  ngKsi["Content-ID"] = "ngKsi";
  ngKsi["Content-Type"] = "varchar(32)";
  ngKsi ["content"] = to_string( nc.get()->ngKsi);
  udsf_nas_context["blocks"].push_back(ngKsi);
#endif
  // Get non-current native nas key set identity (Optional IE), used for
  // inter-system change from S1 to N1 Get 5GMM Capability IE (optional), not
  // included for periodic registration updating procedure
  uint8_t _5g_mm_cap = regReq->get5GMMCapability();
  if (_5g_mm_cap == -1) {
    Logger::amf_n1().warn("No Optional IE 5GMMCapability available");
  }
  nc.get()->mmCapability = _5g_mm_cap;
#if 0
  mmCapability["Content-ID"] = "mmCapability";
  mmCapability["Content-Type"] = "varchar(32)";
  mmCapability ["content"] = to_string( nc.get()->mmCapability);
  udsf_nas_context["blocks"].push_back(mmCapability);
#endif
  // Get UE Security Capability IE (optional), not included for periodic
  // registration updating procedure
  uint8_t encrypt_alg = {0};
  uint8_t integrity_alg = {0};
  if (!regReq->getUeSecurityCapability(encrypt_alg, integrity_alg)) {
    Logger::amf_n1().warn("No Optional IE UESecurityCapability available");
  }
  else{
    nc.get()->ueSecurityCaplen = regReq->ie_ue_security_capability->getLenght();
    nc.get()->ueSecurityCapEEnc = regReq->ie_ue_security_capability->getEEASel();
    nc.get()->ueSecurityCapEInt = regReq->ie_ue_security_capability->getEIASel();
#if 0
    ueSecurityCaplen["Content-ID"] = "ueSecurityCaplen";
    ueSecurityCaplen["Content-Type"] = "varchar(32)";
    ueSecurityCaplen ["content"] = to_string( nc.get()->ueSecurityCaplen);
    udsf_nas_context["blocks"].push_back(ueSecurityCaplen);

    ueSecurityCapEEnc["Content-ID"] = "ueSecurityCapEEnc";
    ueSecurityCapEEnc["Content-Type"] = "varchar(32)";
    ueSecurityCapEEnc ["content"] = to_string( nc.get()->ueSecurityCapEEnc);
    udsf_nas_context["blocks"].push_back(ueSecurityCapEEnc);

    ueSecurityCapEInt["Content-ID"] = "ueSecurityCapEInt";
    ueSecurityCapEInt["Content-Type"] = "varchar(32)";
    ueSecurityCapEInt["content"] = to_string( nc.get()->ueSecurityCapEInt);
    udsf_nas_context["blocks"].push_back(ueSecurityCapEInt);
#endif
  }
  nc.get()->ueSecurityCapEnc = encrypt_alg;
  nc.get()->ueSecurityCapInt = integrity_alg;
#if 0
  ueSecurityCapEnc["Content-ID"] = "ueSecurityCapEnc";
  ueSecurityCapEnc["Content-Type"] = "varchar(32)";
  ueSecurityCapEnc ["content"] = to_string( nc.get()->ueSecurityCapEnc);
  udsf_nas_context["blocks"].push_back(ueSecurityCapEnc);
  
  ueSecurityCapInt["Content-ID"] = "ueSecurityCapInt";
  ueSecurityCapInt["Content-Type"] = "varchar(32)";
  ueSecurityCapInt ["content"] = to_string( nc.get()->ueSecurityCapInt);
  udsf_nas_context["blocks"].push_back(ueSecurityCapInt);
#endif
 


  uint16_t pdu_session_status = 0xffff;
  pdu_session_status = regReq->getPduSessionStatus();
  bool is_messagecontainer = false;
  bstring nas_msg;
  is_messagecontainer = regReq->getNasMessageContainer(nas_msg);


  // Get Requested NSSAI (Optional IE), if provided
  std::vector<SNSSAI_t> requestedNssai = {};
  if (!regReq->getRequestedNssai(requestedNssai)) {
    Logger::amf_n1().warn("No Optional IE RequestedNssai available");
  }
  delete regReq; // free after getting values from message
  nc.get()->requestedNssai = requestedNssai;
  nc.get()->ctx_avaliability_ind = true;
#if 0
  requestedNssai_json["Content-ID"] = "requestedNssai";
  requestedNssai_json["Content-Type"] = "JSON";
  requestedNssai_json ["content"] = {};
  nlohmann::json requestedNssai_m = {};
  for(int i=0;i<nc->requestedNssai.size();i++)
    {
      requestedNssai_m["sst"]=to_string(nc->requestedNssai[i].sst);
      requestedNssai_m["sd"]=to_string(nc->requestedNssai[i].sd);
      requestedNssai_m["mHplmnSst"]=to_string(nc->requestedNssai[i].mHplmnSst);
      requestedNssai_m["mHplmnSd"]=to_string(nc->requestedNssai[i].mHplmnSd);
      requestedNssai_json["content"].push_back(requestedNssai_m);

    }
  udsf_nas_context["blocks"].push_back(requestedNssai_json);
  ctx_avaliability_ind["Content-ID"] = "ctx_avaliability_ind";
  ctx_avaliability_ind["Content-Type"] = "varchar(32)";
  ctx_avaliability_ind ["content"] = to_string( nc.get()->ctx_avaliability_ind);
  udsf_nas_context["blocks"].push_back(ctx_avaliability_ind);
#endif

  // Get Last visited registered TAI(OPtional IE), if provided
  // Get S1 Ue network capability(OPtional IE), if ue supports S1 mode
  // Get uplink data status(Optional IE), if UE has uplink user data to be sent
  // Get pdu session status(OPtional IE), associated and active pdu sessions
  // available in UE

  // Store NAS information into nas_context
  // Run different registration procedure
  
  /**********************hsx add***********************/
#if 0
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  
  nlohmann::json udsf_response;
  udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;    
  udsf_nas_context["blocks"] = nlohmann::json::array({
                                                   {{"Content-ID", "ctx_avaliability_ind"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ctx_avaliability_ind)}},
                                                   {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->amf_ue_ngap_id)}},
                                                   {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ran_ue_ngap_id)}},
                                                   {{"Content-ID", "registration_type"},{"Content-Type", "varchar(32)"},{"content", to_string( nc->registration_type)}},
                                                   {{"Content-ID", "follow_on_req_pending_ind"},{"Content-Type", "varchar(32)"},{"content",  to_string( nc->follow_on_req_pending_ind)}},
                                                   {{"Content-ID", "ngKsi"},{"Content-Type", "varchar(32)"},{"content", to_string( nc->ngKsi)}},
                                                   {{"Content-ID", "imsi"},{"Content-Type", "varchar(32)"},{"content", nc->imsi}},
                                                   {{"Content-ID", "mmCapability"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->mmCapability)}},
                                                   {{"Content-ID", "ueSecurityCaplen"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCaplen)}},
                                                   {{"Content-ID", "ueSecurityCapEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapEnc)}},
                                                   {{"Content-ID", "ueSecurityCapInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc->ueSecurityCapInt)}},
                                                   {{"Content-ID", "serving_network"},{"Content-Type", "varchar(32)"},{"content",  nc->serving_network}}
                                               });
  
  // printf("----------------udsf_nas_context %s---------\n",udsf_nas_context.dump().c_str());
   amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
#endif
  // udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + std::to_string(nc.get()->amf_ue_ngap_id) ;
  // if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
  // Logger::amf_n2().error("No existing gNG context with assoc_id (%d)", nc.get()->amf_ue_ngap_id);
  //   return;
  // }
  // Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());


  // nas_context *nc1 = new nas_context();
  // std::shared_ptr<nas_context> nc2;
  // nc1->nas_context_from_json(udsf_response);
  // nc2 = std::shared_ptr<nas_context>(nc1);

  //printf("ran_ue_ngap_id-%d\n",nc2.get()->ran_ue_ngap_id);
//printf("amf_ue_ngap_id-%d\n",nc2.get()->amf_ue_ngap_id);
//printf("ctx_avaliability_ind-%d\n",nc2.get()->ctx_avaliability_ind);
//printf("nas_status-%d\n",nc2.get()->nas_status);
//printf("serving_network-%d\n",nc2.get()->serving_network);

  /**********************hsx add***********************/
  switch (reg_type) {
  case INITIAL_REGISTRATION: {
    run_initial_registration_procedure(); // IEs?
    run_registration_procedure(nc);
  } break;
  case MOBILITY_REGISTRATION_UPDATING: {
    Logger::amf_n1().error("Network handling mobility registration ...");
    //run_mobility_registration_update_procedure(nc);
  if(is_messagecontainer)
        run_periodic_registration_update_procedure(nc, nas_msg);
    else
        run_periodic_registration_update_procedure(nc, pdu_session_status);
  } break;
  case PERIODIC_REGISTRATION_UPDATING: {

    char *pathvar = NULL;
    pathvar = getenv("IS_AMF_PERIODIC");
    if(pathvar && !strcmp(pathvar, "true"))
    {
      Logger::amf_n1().debug("Network handling periodic registration ...");
      if(is_messagecontainer)
          run_periodic_registration_update_procedure(nc, nas_msg);
      else
          run_periodic_registration_update_procedure(nc, pdu_session_status);
    } else{
      Logger::amf_n1().warn("Please set environment variable \"IS_AMF_PERIODIC\"");
      Logger::amf_n1().error("Network doesn't support periodic registration, reject ...");
    }
  } break;
  case EMERGENCY_REGISTRATION: {
    if (!amf_cfg.is_emergency_support.compare("false")) {
      Logger::amf_n1().error(
          "Network doesn't support emergency registration, reject ...");
      response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE, ran_ue_ngap_id,
                                       amf_ue_ngap_id); // cause?
      return;
    }
  } break;
  }
}

//------------------------------------------------------------------------------
// authentication vector handlers
bool amf_n1::generate_authentication_vector() {}

// context management functions
//------------------------------------------------------------------------------
bool amf_n1::is_amf_ue_id_2_nas_context(const long &amf_ue_ngap_id) const {
  std::shared_lock lock(m_amfueid2nas_context);
  return bool{amfueid2nas_context.count(amf_ue_ngap_id) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<nas_context>
amf_n1::amf_ue_id_2_nas_context(const long &amf_ue_ngap_id) const {
  std::shared_lock lock(m_amfueid2nas_context);
  return amfueid2nas_context.at(amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
void amf_n1::set_amf_ue_ngap_id_2_nas_context(const long &amf_ue_ngap_id,
                                              std::shared_ptr<nas_context> nc) {
  std::shared_lock lock(m_amfueid2nas_context);
  amfueid2nas_context[amf_ue_ngap_id] = nc;
}

//------------------------------------------------------------------------------
bool amf_n1::is_guti_2_nas_context(const std::string &guti) const {
  std::shared_lock lock(m_guti2nas_context);
  return bool{guti2nas_context.count(guti) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<nas_context>
amf_n1::guti_2_nas_context(const std::string &guti) const {
  std::shared_lock lock(m_guti2nas_context);
  return guti2nas_context.at(guti);
}

//------------------------------------------------------------------------------
void amf_n1::set_guti_2_nas_context(const std::string &guti,
                                    std::shared_ptr<nas_context> nc) {
  std::shared_lock lock(m_guti2nas_context);
  guti2nas_context[guti] = nc;
}
bool amf_n1::is_guti_2_nas_context_in_udsf(const std::string &guti) {
  printf("-------is_guti_2_nas_context_in_udsf-------------\n");
  nlohmann::json udsf_response; 
  std::string record_id = "guti=\'" + guti + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
  return (amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response));
}
std::shared_ptr<nas_context> amf_n1::guti_2_nas_context_in_udsf(const std::string &guti) const {
  printf("-------guti_2_nas_context_in_udsf-------------");
  nlohmann::json udsf_response; 
  std::shared_ptr<nas_context> nc = std::shared_ptr<nas_context>(new nas_context());
  std::string record_id = "guti=\'" + guti + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response))
  {
     Logger::amf_n1().error("No existing nas_context with GUTI %s",guti.c_str());
     return nc;
  }
  nc.get()->nas_context_from_json(udsf_response);
  return nc;
}

bool amf_n1::is_amf_ue_id_2_nas_context_in_udsf(const long& amf_ue_ngap_id) const {
  Logger::amf_n1().debug("try to get ");
  nlohmann::json udsf_response; 
  std::string record_id = "RECORD_ID=\'" + to_string(amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
  return (amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response));
}
//std::shared_ptr<nas_context> amf_n1::amf_ue_id_2_nas_context_in_udsf(const long& amf_ue_ngap_id) const {
void amf_n1::amf_ue_id_2_nas_context_in_udsf(const long& amf_ue_ngap_id, std::shared_ptr<nas_context>&nc) const {
  nlohmann::json udsf_response; 
  std::string record_id = "RECORD_ID=\'" + to_string(amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response))
  {
     Logger::amf_n1().error("No existing nas_context with amf_ue_ngap_id %s",to_string(amf_ue_ngap_id).c_str());
     return ;
  }
  nc.get()->nas_context_from_json(udsf_response);
  return ;
}

// to lower layer TASK_N2
//------------------------------------------------------------------------------
void amf_n1::itti_send_dl_nas_buffer_to_task_n2(bstring &b,
                                                uint32_t ran_ue_ngap_id,
                                                long amf_ue_ngap_id) {
  itti_dl_nas_transport *msg =
      new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
  msg->ran_ue_ngap_id = ran_ue_ngap_id;
  msg->amf_ue_ngap_id = amf_ue_ngap_id;
  msg->nas = b;
  std::shared_ptr<itti_dl_nas_transport> i =
      std::shared_ptr<itti_dl_nas_transport>(msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error("Could not send ITTI message %s to task TASK_AMF_N2",
                           i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
// response messages
void amf_n1::response_registration_reject_msg(uint8_t cause_value,
                                              uint32_t ran_ue_ngap_id,
                                              long amf_ue_ngap_id) {
  RegistrationReject *registrationRej = new RegistrationReject();
  registrationRej->setHeader(PLAIN_5GS_MSG);
  registrationRej->set_5GMM_Cause(cause_value);
  uint8_t buffer[BUFFER_SIZE_1024] = {0};
  int encoded_size = registrationRej->encode2buffer(buffer, BUFFER_SIZE_1024);
  // dump_nas_message(buffer, encoded_size);
  print_buffer("amf_n1", "Registration-Reject message buffer", buffer,
               encoded_size);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Registration-Reject message error");
    return;
  } else {
    delete registrationRej;
  }
  bstring b = blk2bstr(buffer, encoded_size);
  itti_send_dl_nas_buffer_to_task_n2(b, ran_ue_ngap_id, amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
// specific procedures running logic
void amf_n1::run_registration_procedure(std::shared_ptr<nas_context> &nc) {
  Logger::amf_n1().debug("Start to run registration procedure");
#if 0
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json is_specific_procedure_for_registration_running;
  nlohmann::json is_auth_vectors_present;
  nlohmann::json ngksi;
#endif 
  if (!nc.get()->ctx_avaliability_ind) {
    Logger::amf_n1().error("NAS context is not available");
    return;
  }
  nc.get()->is_specific_procedure_for_registration_running = true;
#if 0
  is_specific_procedure_for_registration_running["Content-ID"] = "is_specific_procedure_for_registration_running";
  is_specific_procedure_for_registration_running["Content-Type"] = "varchar(32)";
  is_specific_procedure_for_registration_running["content"] = to_string(nc.get()->is_stacs_available);
  udsf_nas_context["blocks"].push_back(is_specific_procedure_for_registration_running);
#endif
  if (nc.get()->is_imsi_present) {
    Logger::amf_n1().debug("SUCI SUPI format IMSI is available");
    if (!nc.get()->is_auth_vectors_present) {
      Logger::amf_n1().debug(
          "Authentication vector in nas_context is not available");
      if (auth_vectors_generator(nc)) { // all authentication in one(AMF)
        ngksi_t ngksi = 0;
        if (nc.get()->security_ctx &&
            nc.get()->ngKsi != NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE) {
          // ngksi = (nc.get()->ngKsi + 1) % (NGKSI_MAX_VALUE + 1);
          ngksi = (nc.get()->amf_ue_ngap_id + 1); // % (NGKSI_MAX_VALUE + 1);
        }
        nc.get()->ngKsi = ngksi;
        handle_auth_vector_successful_result(nc);
      } else {
        Logger::amf_n1().error("Request authentication vectors failure");
        response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE,
                                         nc.get()->ran_ue_ngap_id,
                                         nc.get()->amf_ue_ngap_id); // cause?
      }
    } else {
      Logger::amf_n1().debug(
          "Authentication vector in nas_context is available");
      ngksi_t ngksi = 0;
      if (nc.get()->security_ctx &&
          nc.get()->ngKsi != NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE) {
        // ngksi = (nc.get()->ngKsi + 1) % (NGKSI_MAX_VALUE + 1);
        ngksi = (nc.get()->amf_ue_ngap_id + 1); // % (NGKSI_MAX_VALUE + 1);
        Logger::amf_n1().debug("New ngKsi(%d)", ngksi);
        // ... how to handle?
      }
      nc.get()->ngKsi = ngksi;
      handle_auth_vector_successful_result(nc);
    }
#if 0
  ngksi["Content-ID"] = "ngKsi";
  ngksi["Content-Type"] = "varchar(32)";
  ngksi["content"] = to_string(nc.get()->ngKsi);
  udsf_nas_context["blocks"].push_back(ngksi);
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + to_string(nc.get()->amf_ue_ngap_id) ;
  amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
#endif
  } else if (nc.get()->is_5g_guti_present) {
    Logger::amf_n1().debug("Start to run identification procedure");
    nc.get()->is_auth_vectors_present = false;
#if 0
      is_auth_vectors_present["Content-ID"] = "is_auth_vectors_present";
      is_auth_vectors_present["Content-Type"] = "varchar(32)";
      is_auth_vectors_present ["content"] = to_string( nc.get()->is_auth_vectors_present);
      udsf_nas_context["blocks"].push_back(is_auth_vectors_present);
#endif

    // ... identification procedure
    IdentityRequest *ir = new IdentityRequest();
    // itti_msg->cause = 28;//cause nas(2)--deregister
    ir->setHeader(PLAIN_5GS_MSG);
    ir->set_5GS_Identity_Type(SUCI);
    uint8_t buffer[100];
    int encoded_size = ir->encode2buffer(buffer, 100);
    delete ir;
    itti_dl_nas_transport *dnt = new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas = blk2bstr(buffer, encoded_size);
    dnt->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
#if 0
    /*************************hsx add**************************/
    std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
    udsf_nas_context["meta"] ["tags"] = {
                                        {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                        {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                        } ;    
    udsf_nas_context["blocks"] = nlohmann::json::array({
                                                {{"Content-ID", "is_auth_vectors_present"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->is_auth_vectors_present)}},    
                                                {{"Content-ID", "Href"},{"Content-Type", "varchar(1024)"},{"content",  nc.get()->Href}}
                                            });
    //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
    /*************************hsx add**************************/
#endif
    std::shared_ptr<itti_dl_nas_transport> i = std::shared_ptr<itti_dl_nas_transport>(dnt);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          i->get_msg_name());
    }
  }
 
}

//------------------------------------------------------------------------------
// authentication vectors
// TODO: should be done in UDM/AUSF
bool amf_n1::auth_vectors_generator(std::shared_ptr<nas_context> &nc) {
  Logger::amf_n1().debug("Start to generate authentication vectors");
  if (amf_cfg.is_Nausf) {
    if (!authentication_vectors_from_ausf(nc))
      return false;
  } else {
    authentication_vectors_generator_in_udm(nc);
    authentication_vectors_generator_in_ausf(nc);
    Logger::amf_n1().debug("Deriving kamf");
    for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
      Authentication_5gaka::derive_kamf(
          nc.get()->imsi, nc.get()->_5g_av[i].kseaf, nc.get()->kamf[i],
          0x0000); // second parameter: abba
    }
  }
  return true;
}
//------------------------------------------------------------------------------
#define CURL_TIMEOUT_MS 300L

std::size_t callback_ausf(const char *in, std::size_t size, std::size_t num,
                          std::string *out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

void amf_n1::curl_http_client(std::string remoteUri, std::string Method,
                              std::string msgBody, std::string &Response) {
  Logger::amf_n1().info("Send HTTP message with body %s", msgBody.c_str());

  uint32_t str_len = msgBody.length();
  char *body_data = (char *)malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void *)body_data, (void *)msgBody.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL *curl = curl_easy_init();

  if (curl) {
    CURLcode res = {};
    struct curl_slist *headers = nullptr;
    if (!Method.compare("POST") || !Method.compare("PATCH") ||
        !Method.compare("PUT")) {
      std::string content_type = "Content-Type: application/json";
      headers = curl_slist_append(headers, content_type.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    curl_easy_setopt(curl, CURLOPT_URL, remoteUri.c_str());
    if (!Method.compare("POST"))
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
    else if (!Method.compare("PATCH"))
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    else if (!Method.compare("PUT")) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    } else
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.nausf.if_name.c_str());

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback_ausf);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());
    if (!Method.compare("POST") || !Method.compare("PATCH") ||
        !Method.compare("PUT")) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, msgBody.length());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);
    }

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get the response
    std::string response = *httpData.get();
    std::string json_data_response = "";
    std::string resMsg = "";
    bool is_response_ok = true;
    Logger::amf_n1().info("Get response with httpcode (%d)", httpCode);

    if (httpCode == 0) {
      Logger::amf_n1().info("Cannot get response when calling %s",
                            remoteUri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      return;
    }

    nlohmann::json response_data = {};

    if (httpCode != 200 && httpCode != 201 && httpCode != 204) {
      is_response_ok = false;
      if (response.size() < 1) {
        Logger::amf_n1().info("There's no content in the response");
        // TODO: send context response error
        return;
      }
      Logger::amf_n1().info("Wrong response code");

      return;
    }

    else {
      Response = *httpData.get();
    }

    if (!is_response_ok) {
      try {
        response_data = nlohmann::json::parse(json_data_response);
      } catch (nlohmann::json::exception &e) {
        Logger::amf_n1().info("Could not get Json content from the response");
        // Set the default Cause
        response_data["error"]["cause"] = "504 Gateway Timeout";
      }

      Logger::amf_n1().info("Get response with jsonData: %s",
                            json_data_response.c_str());

      std::string cause = response_data["error"]["cause"];
      Logger::amf_n1().info("Call Network Function services failure");
      Logger::amf_n1().info("Cause value: %s", cause.c_str());
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  if (body_data) {
    free(body_data);
    body_data = NULL;
  }
  fflush(stdout);
}

bool amf_n1::authentication_vectors_from_ausf(
    std::shared_ptr<nas_context> &nc) {
#if 0
    nlohmann::json udsf_nas_context;
    nlohmann::json udsf_response;
    nlohmann::json Href;
    nlohmann::json _5g_av = {};
    nlohmann::json _5g_av_m = {};
#endif

  Logger::amf_n1().debug("authentication_vectors_from_ausf");
  std::string ausf_ip =
      std::string(inet_ntoa(*((struct in_addr *)&amf_cfg.nausf.addr4)));
  std::string ausf_port = std::to_string(amf_cfg.nausf.port);
  std::string remoteUri =
      ausf_ip + ":" + ausf_port + "/nausf-auth/v1/ue-authentications";
  std::string msgBody;
  std::string Response;

  nlohmann::json authenticationinfo_j;
  AuthenticationInfo authenticationinfo;
  authenticationinfo.setSupiOrSuci(nc.get()->imsi);
  authenticationinfo.setServingNetworkName(nc.get()->serving_network);
  ResynchronizationInfo resynchronizationInfo;
  uint8_t auts_len = blength(nc.get()->auts);
  Logger::amf_n1().debug("auts_len = %d",auts_len);
  uint8_t *auts_value = (uint8_t *)bdata(nc.get()->auts);
  std::string authenticationinfo_auts;
  std::string authenticationinfo_rand;
  if (auts_len > 0) {
  //if (auts_value) {
    Logger::amf_n1().debug("********* has auts *********");
    char *auts_s = (char *)malloc(auts_len * 2 + 1);
    // char *rand_s = (char *)malloc(RAND_LENGTH * 2 + 1);
    memset(auts_s, 0, sizeof(auts_s));
    // memset(rand_s, 0, sizeof(rand_s));
    // uint8_t rand_value[RAND_LENGTH];

    Logger::amf_n1().debug("********* auts_len (%d) *********", auts_len);
    for (int i = 0; i < auts_len; i++) {
      sprintf(&auts_s[i * 2], "%02X", auts_value[i]);
    }

    authenticationinfo_auts = auts_s;
    print_buffer("amf_n1", "********** ausf ***********", auts_value, auts_len);
    Logger::amf_n1().info("********** ausf_s (%s) ***********", auts_s);
    // generate_random(rand_value, RAND_LENGTH);
    std::map<std::string, std::string>::iterator iter;
    iter = rand_record.find(nc.get()->imsi);
    if (iter != rand_record.end()) {
      authenticationinfo_rand = iter->second;
      Logger::amf_n1().info("********** rand_s (%s) ***********",
                            authenticationinfo_rand.c_str());
    } else {
      Logger::amf_n1().error("********** There's no last rand ***********");
    }
    //    rand_value = nc.get()->_5g_av[0].rand;
    //    for(int i=0;i<RAND_LENGTH;i++)
    //    {
    //    sprintf(&rand_s[i*2],"%02X",rand_value[i]);
    //    }
    //    authenticationinfo_rand = rand_s;
    //    print_buffer("amf_n1", "********** rand  ***********", rand_value,
    //    RAND_LENGTH);

    resynchronizationInfo.setAuts(authenticationinfo_auts);
    resynchronizationInfo.setRand(authenticationinfo_rand);
    authenticationinfo.setResynchronizationInfo(resynchronizationInfo);
    free_wrapper((void **)&auts_s);
    //    free_wrapper((void**) &rand_s);
  }

  to_json(authenticationinfo_j, authenticationinfo);
  msgBody = authenticationinfo_j.dump();
  curl_http_client(remoteUri, "POST", msgBody, Response);

  Logger::amf_n1().info("POST response : %s", Response.c_str());

  try {
    UEAuthenticationCtx ueauthenticationctx;
    nlohmann::json::parse(Response.c_str()).get_to(ueauthenticationctx);

    unsigned char *r5gauthdata_rand =
        format_string_as_hex(ueauthenticationctx.getR5gAuthData().getRand());
    memcpy(nc.get()->_5g_av[0].rand, r5gauthdata_rand, 16);
    rand_record[nc.get()->imsi] =
        ueauthenticationctx.getR5gAuthData().getRand();
    print_buffer("amf_n1", "5G AV: rand", nc.get()->_5g_av[0].rand, 16);
    free_wrapper((void **)&r5gauthdata_rand);

    unsigned char *r5gauthdata_autn =
        format_string_as_hex(ueauthenticationctx.getR5gAuthData().getAutn());
    memcpy(nc.get()->_5g_av[0].autn, r5gauthdata_autn, 16);
    print_buffer("amf_n1", "5G AV: autn", nc.get()->_5g_av[0].autn, 16);
    free_wrapper((void **)&r5gauthdata_autn);

    unsigned char *r5gauthdata_hxresstar = format_string_as_hex(
        ueauthenticationctx.getR5gAuthData().getHxresStar());
    memcpy(nc.get()->_5g_av[0].hxresStar, r5gauthdata_hxresstar, 16);
    print_buffer("amf_n1", "5G AV: hxres*", nc.get()->_5g_av[0].hxresStar, 16);
    free_wrapper((void **)&r5gauthdata_hxresstar);

#if 0
    _5g_av["Content-ID"]="_5g_av";
    _5g_av["Content-Type"]="JSON";
   
    _5g_av_m["hxresStar"]=to_string(*(nc->_5g_av[0].hxresStar));
    _5g_av_m["autn"]=to_string(*(nc->_5g_av[0].autn));
    _5g_av_m["rand"]=to_string(*(nc->_5g_av[0].rand));
    _5g_av["content"].push_back(_5g_av_m);
    udsf_nas_context["blocks"].push_back(_5g_av);
#endif

    std::map<std::string, LinksValueSchema>::iterator iter;
    iter = ueauthenticationctx.getLinks().find("5G_AKA");
    if (iter != ueauthenticationctx.getLinks().end()) {
      nc.get()->Href = iter->second.getHref();

#if 0 
     Href["Content-ID"]="Href";
     Href["Content-Type"]="varchar(1024)";
     Href["content"] = nc.get()->Href;
     udsf_nas_context["blocks"].push_back(Href);
#endif
      Logger::amf_n1().info("Links is: ", nc.get()->Href);
    } else {
      Logger::amf_n1().error("Not found 5G_AKA");
    }
  } catch (nlohmann::json::exception &e) {
    Logger::amf_n1().info("Could not get Json content from AUSF response");
    // TODO: error handling
    return false;
  }
#if 0
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
 std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  
  udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;    
  //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
#endif
  return true;
}

bool amf_n1::_5g_aka_confirmation_from_ausf(std::shared_ptr<nas_context> &nc,
                                            bstring resStar) {
  Logger::amf_n1().debug("_5g_aka_confirmation_from_ausf");
  std::string remoteUri = nc.get()->Href;
  Logger::amf_n1().debug("remote URI: %s", remoteUri.c_str());
  Logger::amf_n1().debug("UE's imsi: %s", nc.get()->imsi.c_str());

  std::string msgBody;
  std::string Response;
  std::string resStar_string;

  Logger::amf_n1().debug("testing ................1");
  //std::map<std::string, std::string>::iterator iter;
  Logger::amf_n1().debug("testing ................1.1");
  //iter = rand_record.find(nc.get()->imsi);
  Logger::amf_n1().debug("testing ................1.2");
  //rand_record.erase(iter);
  Logger::amf_n1().debug("testing ................2");
  // convert_string_2_hex(resStar, resStar_string);
  uint8_t resStar_len = blength(resStar);
  uint8_t *resStar_value = (uint8_t *)bdata(resStar);
  char *resStar_s = (char *)malloc(resStar_len * 2 + 1);

  for (int i = 0; i < resStar_len; i++) {
    sprintf(&resStar_s[i * 2], "%02X", resStar_value[i]);
  }
  resStar_string = resStar_s;
  print_buffer("amf_n1", "********** resStar  ***********", resStar_value,
               resStar_len);
  Logger::amf_n1().info("********** resStar_s (%s) ***********", resStar_s);

  nlohmann::json confirmationdata_j;
  ConfirmationData confirmationdata;
  confirmationdata.setResStar(resStar_string);

  to_json(confirmationdata_j, confirmationdata);
  msgBody = confirmationdata_j.dump();
  curl_http_client(remoteUri, "PUT", msgBody, Response);
  free_wrapper((void **)&resStar_s);
  // free(resStar_string.c_str());
  Logger::amf_n1().info("Get Json content from AUSF response: %s",
                        Response.c_str());

  try {
    ConfirmationDataResponse confirmationdataresponse;
    nlohmann::json::parse(Response.c_str()).get_to(confirmationdataresponse);
    unsigned char *kseaf_hex =
        format_string_as_hex(confirmationdataresponse.getKseaf());
    memcpy(nc.get()->_5g_av[0].kseaf, kseaf_hex, 32);
    print_buffer("amf_n1", "5G AV: kseaf", nc.get()->_5g_av[0].kseaf, 32);
    free_wrapper((void **)&kseaf_hex);

    Logger::amf_n1().debug("Deriving kamf");
    for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
      Authentication_5gaka::derive_kamf(
          nc.get()->imsi, nc.get()->_5g_av[i].kseaf, nc.get()->kamf[i],
          0x0000); // second parameter: abba
      print_buffer("amf_n1", "kamf", nc.get()->kamf[i], 32);
    }
#if 0
    nlohmann::json udsf_nas_context;
    nlohmann::json udsf_response;
//_5g_av
    nlohmann::json _5g_av = {};
    nlohmann::json _5g_av_m = {};
    _5g_av["Content-ID"]="_5g_av";
    _5g_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_av_m["avType"]=to_string(nc->_5g_av[i].avType);
      _5g_av_m["rand"]=to_string(*(nc->_5g_av[i].rand));
      _5g_av_m["hxres"]=to_string(*nc->_5g_av[i].hxres);
      _5g_av_m["autn"]=to_string(*nc->_5g_av[i].autn);
      _5g_av_m["hxresStar"]=to_string(*nc->_5g_av[i].hxresStar);
      _5g_av_m["kseaf"]=to_string(*nc->_5g_av[i].kseaf);
      _5g_av["content"].push_back(_5g_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_av);

//kamf
    nlohmann::json kamf = {};
    nlohmann::json kamf_m = {};
    kamf["Content-ID"]="kamf";
    kamf["Content-Type"]="JSON";
    for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++){
      for(int j=0;j<32;j++)
      {
        kamf_m=to_string(nc.get()->kamf[i][j]);
      }
    kamf["content"].push_back(kamf_m);
    }
    udsf_nas_context["blocks"].push_back(kamf);


    std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id  ;
    udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;    
    //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
#endif
  } catch (nlohmann::json::exception &e) {
    Logger::amf_n1().info("Could not get Json content from AUSF response");
    // TODO: error handling
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::authentication_vectors_generator_in_ausf(
    std::shared_ptr<nas_context> &nc) { // A.5, 3gpp ts33.501
  Logger::amf_n1().debug("Authentication_vectors_generator_in_ausf");
  uint8_t inputString[MAX_5GS_AUTH_VECTORS][40];
  uint8_t *xresStar[MAX_5GS_AUTH_VECTORS];
  uint8_t *rand[MAX_5GS_AUTH_VECTORS];

  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json _5g_av = {};
  nlohmann::json _5g_av_m = {};

  for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
    xresStar[i] = nc.get()->_5g_he_av[i].xresStar;
    rand[i] = nc.get()->_5g_he_av[i].rand;
    memcpy(&inputString[i][0], rand[i], 16);
    memcpy(&inputString[i][16], xresStar[i], 16);
    unsigned char sha256Out[Sha256::DIGEST_SIZE];
    sha256((unsigned char *)inputString[i], 32, sha256Out);
    for (int j = 0; j < 16; j++)
    nc.get()->_5g_av[i].hxresStar[j] = (uint8_t)sha256Out[j];
    memcpy(nc.get()->_5g_av[i].rand, nc.get()->_5g_he_av[i].rand, 16);
    memcpy(nc.get()->_5g_av[i].autn, nc.get()->_5g_he_av[i].autn, 16);

    

    uint8_t kseaf[32];
    Authentication_5gaka::derive_kseaf(nc.get()->serving_network,
                                       nc.get()->_5g_he_av[i].kausf, kseaf);
    memcpy(nc.get()->_5g_av[i].kseaf, kseaf, 32);


    
  
    // print_buffer("amf_n1", "5G AV: rand", nc.get()->_5g_av[i].rand, 16);
    // print_buffer("amf_n1", "5G AV: autn", nc.get()->_5g_av[i].autn, 16);
    // print_buffer("amf_n1", "5G AV: kseaf", nc.get()->_5g_av[i].kseaf, 32);
    // print_buffer("amf_n1", "5G AV: hxres*", nc.get()->_5g_av[i].hxresStar,
    // 16);


  
  }
  _5g_av["Content-ID"]="_5g_av";
  _5g_av["Content-Type"]="JSON";
  for(int ii=0;ii < MAX_5GS_AUTH_VECTORS;ii++)
  {
    _5g_av_m["hxresStar"]=to_string(*(nc->_5g_av[ii].hxresStar));
    _5g_av_m["kseaf"]=to_string(*(nc->_5g_av[ii].kseaf));
    _5g_av["content"].push_back(_5g_av_m);
  }
  udsf_nas_context["blocks"].push_back(_5g_av);
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") +record_id;
  
  udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;                       
  //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);

  return true;
}

//------------------------------------------------------------------------------
// TODO: Get from UDM
bool amf_n1::authentication_vectors_generator_in_udm(
    std::shared_ptr<nas_context> &nc) {
  Logger::amf_n1().debug("Generate authentication vectors");
  uint8_t *sqn = NULL;
  uint8_t *auts = (uint8_t *)bdata(nc.get()->auts);
  _5G_HE_AV_t *vector = nc.get()->_5g_he_av;
  // Access to MySQL to fetch UE-related information
  if (!connect_to_mysql()) {
    Logger::amf_n1().error("Cannot connect to MySQL");
    return false;
  }
  Logger::amf_n1().debug("Connected to MySQL successfully");
  mysql_auth_info_t mysql_resp = {};
  if (get_mysql_auth_info(nc.get()->imsi, mysql_resp)) {
    if (auts) {
      sqn = Authentication_5gaka::sqn_ms_derive(mysql_resp.opc, mysql_resp.key,
                                                auts, mysql_resp.rand);
      if (sqn) {
        generate_random(vector[0].rand, RAND_LENGTH);
        mysql_push_rand_sqn(nc.get()->imsi, vector[0].rand, sqn);
        mysql_increment_sqn(nc.get()->imsi);
        free_wrapper((void **)&sqn);
      }
      if (!get_mysql_auth_info(nc.get()->imsi, mysql_resp)) {
        Logger::amf_n1().error("Cannot get data from MySQL");
        return false;
      }
      sqn = mysql_resp.sqn;
      for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
        generate_random(vector[i].rand, RAND_LENGTH);
        print_buffer("amf_n1", "Generated random rand (5G HE AV)",
                     vector[i].rand, 16);
        generate_5g_he_av_in_udm(mysql_resp.opc, nc.get()->imsi, mysql_resp.key,
                                 sqn, nc.get()->serving_network,
                                 vector[i]); // serving network name
      }
      mysql_push_rand_sqn(nc.get()->imsi, vector[MAX_5GS_AUTH_VECTORS - 1].rand,
                          sqn);
    } else {
      Logger::amf_n1().debug("No auts ...");
      Logger::amf_n1().debug("Receive information from MySQL with IMSI %s",
                             nc.get()->imsi.c_str());
      // print_buffer("amf_n1", "Received from MYSQL: rand", mysql_resp.rand,
      // 16); print_buffer("amf_n1", "Received from MYSQL: opc", mysql_resp.opc,
      // 16); print_buffer("amf_n1", "Received from MYSQL: key", mysql_resp.key,
      // 16); print_buffer("amf_n1", "Received from MYSQL: sqn", mysql_resp.sqn,
      // 6);
      for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
        generate_random(vector[i].rand, RAND_LENGTH);
        // print_buffer("amf_n1", "generated random: rand(5G HE AV)",
        // vector[i].rand, 16);
        sqn = mysql_resp.sqn;
        generate_5g_he_av_in_udm(mysql_resp.opc, nc.get()->imsi, mysql_resp.key,
                                 sqn, nc.get()->serving_network,
                                 vector[i]); // serving network name
      }
      mysql_push_rand_sqn(nc.get()->imsi, vector[MAX_5GS_AUTH_VECTORS - 1].rand,
                          sqn);
    }
    mysql_increment_sqn(nc.get()->imsi);
  } else {
    Logger::amf_n1().error("Failed to fetch user data from MySQL");
    return false;
  }
}

//------------------------------------------------------------------------------
void amf_n1::test_generate_5g_he_av_in_udm(const uint8_t opc[16],
                                           uint8_t key[16], uint8_t sqnak[6],
                                           std::string serving_network,
                                           _5G_HE_AV_t &vector) {
  uint8_t amf[] = {0x90, 0x01};
  uint8_t mac_a[8];
  uint8_t ck[16];
  uint8_t ik[16];
  uint8_t ak[6];
  Authentication_5gaka::f2345(opc, key, vector.rand, vector.xres, ck, ik,
                              ak); // to compute XRES, CK, IK, AK
  uint8_t sqn[6];
  for (int i = 0; i < 6; i++)
    sqn[i] = sqnak[i] ^ ak[i];
  Authentication_5gaka::f1(opc, key, vector.rand, sqn, amf,
                           mac_a); // to compute MAC, Figure 7, ts33.102
  print_buffer("amf_n1", "sqn^ak", sqnak, 6);
  print_buffer("amf_n1", "rand", vector.rand, 16);
  print_buffer("amf_n1", "mac_a", mac_a, 8);
  annex_a_4_33501(ck, ik, vector.xres, vector.rand, serving_network,
                  vector.xresStar);
  Authentication_5gaka::generate_autn(sqn, ak, amf, mac_a,
                                      vector.autn); // generate AUTN
  Authentication_5gaka::derive_kausf(ck, ik, serving_network, sqn, ak,
                                     vector.kausf); // derive Kausf
}

//------------------------------------------------------------------------------
void amf_n1::generate_random(uint8_t *random_p, ssize_t length) {
  gmp_randinit_default(random_state.state);
  gmp_randseed_ui(random_state.state, time(NULL));
  if (!amf_cfg.auth_para.random.compare("true")) {
    Logger::amf_n1().debug("AMF config random -> true");
    random_t random_nb;
    mpz_init(random_nb);
    mpz_init_set_ui(random_nb, 0);
    pthread_mutex_lock(&random_state.lock);
    mpz_urandomb(random_nb, random_state.state, 8 * length);
    pthread_mutex_unlock(&random_state.lock);
    mpz_export(random_p, NULL, 1, length, 0, 0, random_nb);
    int r = 0, mask = 0, shift;
    for (int i = 0; i < length; i++) {
      if ((i % sizeof(i)) == 0)
        r = rand();
      shift = 8 * (i % sizeof(i));
      mask = 0xFF << shift;
      random_p[i] = (r & mask) >> shift;
    }
    Logger::amf_n1().debug(
        "*************** generate rand function %x ***************", r);

  } else {
    Logger::amf_n1().error("AMF config random -> false");
    pthread_mutex_lock(&random_state.lock);
    for (int i = 0; i < length; i++) {
      random_p[i] = i + no_random_delta;
    }
    no_random_delta += 1;
    pthread_mutex_unlock(&random_state.lock);
  }
}

//------------------------------------------------------------------------------
// TODO: read from UDM
void amf_n1::generate_5g_he_av_in_udm(const uint8_t opc[16], string imsi,
                                      uint8_t key[16], uint8_t sqn[6],
                                      std::string serving_network,
                                      _5G_HE_AV_t &vector) {
  Logger::amf_n1().debug("Generate 5g_he_av as in UDM");
  uint8_t amf[] = {0x90, 0x01};
  uint8_t mac_a[8];
  uint8_t ck[16];
  uint8_t ik[16];
  uint8_t ak[6];
  uint64_t _imsi = fromString<uint64_t>(imsi);

  Authentication_5gaka::f1(opc, key, vector.rand, sqn, amf,
                           mac_a); // to compute MAC, Figure 7, ts33.102
  // print_buffer("amf_n1", "Result For F1-Alg: mac_a", mac_a, 8);
  Authentication_5gaka::f2345(opc, key, vector.rand, vector.xres, ck, ik,
                              ak); // to compute XRES, CK, IK, AK
  annex_a_4_33501(ck, ik, vector.xres, vector.rand, serving_network,
                  vector.xresStar);
  // print_buffer("amf_n1", "Result For KDF: xres*(5G HE AV)", vector.xresStar,
  // 16);
  Authentication_5gaka::generate_autn(sqn, ak, amf, mac_a,
                                      vector.autn); // generate AUTN
  // print_buffer("amf_n1", "Generated autn(5G HE AV)", vector.autn, 16);
  Authentication_5gaka::derive_kausf(ck, ik, serving_network, sqn, ak,
                                     vector.kausf); // derive Kausf
  // print_buffer("amf_n1", "Result For KDF: Kausf(5G HE AV)", vector.kausf,
  // 32);
  Logger::amf_n1().debug("Generate_5g_he_av_in_udm finished!");
  // ue_authentication_simulator(vector.rand, vector.autn);
  return;
}

//------------------------------------------------------------------------------
void amf_n1::annex_a_4_33501(uint8_t ck[16], uint8_t ik[16], uint8_t *input,
                             uint8_t rand[16], std::string serving_network,
                             uint8_t *output) {
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(&netName, serving_network.c_str(),
                       serving_network.length());
  uint8_t S[100];
  S[0] = 0x6B;
  memcpy(&S[1], netName.buf, netName.size);
  S[1 + netName.size] = (netName.size & 0xff00) >> 8;
  S[2 + netName.size] = (netName.size & 0x00ff);
  for (int i = 0; i < 16; i++)
    S[3 + netName.size + i] = rand[i];
  S[19 + netName.size] = 0x00;
  S[20 + netName.size] = 0x10;
  for (int i = 0; i < 8; i++)
    S[21 + netName.size + i] = input[i];
  S[29 + netName.size] = 0x00;
  S[30 + netName.size] = 0x08;

  uint8_t plmn[3] = {0x46, 0x0f, 0x11};
  uint8_t oldS[100];
  oldS[0] = 0x6B;
  memcpy(&oldS[1], plmn, 3);
  oldS[4] = 0x00;
  oldS[5] = 0x03;
  for (int i = 0; i < 16; i++)
    oldS[6 + i] = rand[i];
  oldS[22] = 0x00;
  oldS[23] = 0x10;
  for (int i = 0; i < 8; i++)
    oldS[24 + i] = input[i];
  oldS[32] = 0x00;
  oldS[33] = 0x08;
  print_buffer("amf_n1", "Input string: ", S, 31 + netName.size);
  uint8_t key[32];
  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16); // KEY
  // Authentication_5gaka::kdf(key, 32, oldS, 33, output, 16);
  uint8_t out[32];
  Authentication_5gaka::kdf(key, 32, S, 31 + netName.size, out, 32);
  for (int i = 0; i < 16; i++)
    output[i] = out[16 + i];
  print_buffer("amf_n1", "XRES*(new)", out, 32);
}

//------------------------------------------------------------------------------
void amf_n1::handle_auth_vector_successful_result(
    std::shared_ptr<nas_context> nc) {
#if 0
  nlohmann::json ngKsi;
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json is_auth_vectors_present;
#endif
  Logger::amf_n1().debug(
      "Received security vectors, try to setup security with the UE");
  nc.get()->is_auth_vectors_present = true;

  ngksi_t ngksi = 0;
  if (!nc.get()->security_ctx) {
    nc.get()->security_ctx = new nas_secu_ctx();
    if (nc.get()->security_ctx &&
        nc.get()->ngKsi != NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE)
      ngksi = (nc.get()->amf_ue_ngap_id + 1) % (NGKSI_MAX_VALUE + 1);
    // ensure which vector is available?
    nc.get()->ngKsi = ngksi;
#if 0
/************************hsx add***************************/
   is_auth_vectors_present["Content-ID"] = "is_auth_vectors_present";
      is_auth_vectors_present["Content-Type"] = "varchar(32)";
      is_auth_vectors_present ["content"] = to_string( nc.get()->is_auth_vectors_present);
      udsf_nas_context["blocks"].push_back(is_auth_vectors_present);



  ngKsi["Content-ID"] = "ngKsi";
  ngKsi["Content-Type"] = "varchar(32)";
  ngKsi["content"] = to_string(nc.get()->ngKsi);
  udsf_nas_context["blocks"].push_back(ngKsi);
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  udsf_nas_context["meta"] ["tags"] = {
                                        {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                        {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                        } ;    
 // amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
  
  /************************hsx add***************************/
#endif
  }
  int vindex = nc.get()->security_ctx->vector_pointer;
  if (!start_authentication_procedure(nc, vindex, nc.get()->ngKsi)) {
    Logger::amf_n1().error("Start authentication procedure failure, reject...");
    Logger::amf_n1().error("Ran_ue_ngap_id 0x%x", nc.get()->ran_ue_ngap_id);
    response_registration_reject_msg(_5GMM_CAUSE_INVALID_MANDATORY_INFO,
                                     nc.get()->ran_ue_ngap_id,
                                     nc.get()->amf_ue_ngap_id); // cause?
  } else {
    // update mm state -> COMMON-PROCEDURE-INITIATED
  }
}

//------------------------------------------------------------------------------
bool amf_n1::start_authentication_procedure(std::shared_ptr<nas_context> nc,
                                            int vindex, uint8_t ngksi) {
#if 0
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json  is_common_procedure_for_authentication_running;
#endif
  Logger::amf_n1().debug("****Starting authentication procedure****");
  if (check_nas_common_procedure_on_going(nc)) {
    Logger::amf_n1().error("Existed NAS common procedure on going, reject...");
    response_registration_reject_msg(_5GMM_CAUSE_INVALID_MANDATORY_INFO,
                                     nc.get()->ran_ue_ngap_id,
                                     nc.get()->amf_ue_ngap_id); // cause?
    return false;
  }

  nc.get()->is_common_procedure_for_authentication_running = true;
#if 0
  is_common_procedure_for_authentication_running["Content-ID"] = "is_common_procedure_for_authentication_running";
  is_common_procedure_for_authentication_running["Content-Type"] = "varchar(32)";
  is_common_procedure_for_authentication_running["content"] = to_string(nc.get()->is_common_procedure_for_authentication_running);
  udsf_nas_context["blocks"].push_back(is_common_procedure_for_authentication_running);
#endif
  AuthenticationRequest *authReq = new AuthenticationRequest();
  authReq->setHeader(PLAIN_5GS_MSG);
  authReq->setngKSI(NAS_KEY_SET_IDENTIFIER_NATIVE, ngksi);
  uint8_t abba[2];
  abba[0] = 0x00;
  abba[1] = 0x00;
  authReq->setABBA(2, abba);
  uint8_t *rand = nc.get()->_5g_av[vindex].rand;
  if (rand)
    authReq->setAuthentication_Parameter_RAND(rand);
  Logger::amf_n1().debug("Sending Authentication request with rand");
  printf("0x");
  for (int i = 0; i < 16; i++)
    printf("%x", rand[i]);
  printf("\n");

  uint8_t *autn = nc.get()->_5g_av[vindex].autn;
  if (autn)
    authReq->setAuthentication_Parameter_AUTN(autn);
  uint8_t buffer[1024] = {0};
  int encoded_size = authReq->encode2buffer(buffer, 1024);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Authentication Request message error");
    return false;
  } else {
    delete authReq;
  }
  bstring b = blk2bstr(buffer, encoded_size);
  print_buffer("amf_n1", "Authentication-Request message buffer",
               (uint8_t *)bdata(b), blength(b));
  Logger::amf_n1().debug("amf_ue_ngap_id 0x%x", nc.get()->amf_ue_ngap_id);
#if 0 
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  nlohmann::json udsf_ue_nas_context;
  udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf",nlohmann::json::array({"AMF_1234"})}
                                      } ;
//amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
  // udsf_ue_nas_context["blocks"] =; 
  //_5g_av.rand
  //nc.get()->_5g_av[0].autn
 // nc.get()->_5g_av[0].hxresStar
  // nc.get()->Href

  //amf_n2_inst->curl_http_client_udsf(udsf_url,json_part,"PUT");
#endif

  //Update nas_context into UDSF

  http_update_nas_context_into_udsf(nc);
  Logger::amf_n1().debug("dukl start_authentication_procedure dl.seq %d",nc.get()->security_ctx->dl_count.seq_num);

  itti_send_dl_nas_buffer_to_task_n2(b, nc.get()->ran_ue_ngap_id,
                                     nc.get()->amf_ue_ngap_id);
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::check_nas_common_procedure_on_going(
    std::shared_ptr<nas_context> nc) {
  if (nc.get()->is_common_procedure_for_authentication_running) {
    Logger::amf_n1().debug("Existed authentication procedure is running");
    return true;
  }
  if (nc.get()->is_common_procedure_for_identification_running) {
    Logger::amf_n1().debug("Existed identification procedure is running");
    return true;
  }
  if (nc.get()->is_common_procedure_for_security_mode_control_running) {
    Logger::amf_n1().debug("Existed SMC procedure is running");
    return true;
  }
  if (nc.get()->is_common_procedure_for_nas_transport_running) {
    Logger::amf_n1().debug("Existed NAS transport procedure is running");
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::authentication_response_handle(uint32_t ran_ue_ngap_id,
                                            long amf_ue_ngap_id,
                                            bstring plain_msg) {
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json  is_common_procedure_for_authentication_running;
  std::shared_ptr<nas_context> nc;

  if (!is_amf_ue_id_2_nas_context(amf_ue_ngap_id)) {
    Logger::amf_n1().error(
        "No existed NAS context for UE with amf_ue_ngap_id (0x%x)",
        amf_ue_ngap_id);
    response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE, ran_ue_ngap_id,
                                     amf_ue_ngap_id); // cause?
    return;
  }
  nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  Logger::amf_n1().info("Found nas_context(%p) with amf_ue_ngap_id (0x%x)",
                        nc.get(), amf_ue_ngap_id);
  // Stop timer? common procedure finished!
  nc.get()->is_common_procedure_for_authentication_running = false;

  // is_common_procedure_for_authentication_running["Content-ID"] = "is_common_procedure_for_authentication_running";
  // is_common_procedure_for_authentication_running["Content-Type"] = "varchar(32)";
  // is_common_procedure_for_authentication_running["content"] = to_string(nc.get()->is_common_procedure_for_authentication_running);
  // udsf_nas_context["blocks"].push_back(is_common_procedure_for_authentication_running);

  // MM state: COMMON-PROCEDURE-INITIATED -> DEREGISTRED
  // Decode AUTHENTICATION RESPONSE message
  AuthenticationResponse *auth = new AuthenticationResponse();
  auth->decodefrombuffer(nullptr, (uint8_t *)bdata(plain_msg),
                         blength(plain_msg));
  bstring resStar;
  bool isAuthOk = true;
  // Get response RES*
  if (!auth->getAuthenticationResponseParameter(resStar)) {
    Logger::amf_n1().warn(
        "Cannot receive AuthenticationResponseParameter (RES*)");
  } else {
    if (amf_cfg.is_Nausf) {
      // std::string data = bdata(resStar);
      if (!_5g_aka_confirmation_from_ausf(nc, resStar))
        isAuthOk = false;
    } else {
      // Get stored XRES*
      int secu_index = nc.get()->security_ctx->vector_pointer;
      uint8_t *hxresStar = nc.get()->_5g_av[secu_index].hxresStar;
      // Calculate HRES* from received RES*, then compare with XRES stored in
      // nas_context
      uint8_t inputstring[32];
      uint8_t *res = (uint8_t *)bdata(resStar);
      Logger::amf_n1().debug("Start to calculate HRES* from received RES*");
      memcpy(&inputstring[0], nc.get()->_5g_av[secu_index].rand, 16);
      memcpy(&inputstring[16], res, blength(resStar));
      unsigned char sha256Out[Sha256::DIGEST_SIZE];
      sha256((unsigned char *)inputstring, 16 + blength(resStar), sha256Out);
      uint8_t hres[16];
      for (int i = 0; i < 16; i++)
        hres[i] = (uint8_t)sha256Out[i];
      print_buffer("amf_n1", "Received RES* From Authentication-Response", res,
                   16);
      print_buffer("amf_n1", "Stored XRES* in 5G HE AV",
                   nc.get()->_5g_he_av[secu_index].xresStar, 16);
      print_buffer("amf_n1", "Stored XRES in 5G HE AV",
                   nc.get()->_5g_he_av[secu_index].xres, 8);
      print_buffer("amf_n1", "Computed HRES* from RES*", hres, 16);
      print_buffer("amf_n1", "Computed HXRES* from XRES*", hxresStar, 16);
      for (int i = 0; i < 16; i++) {
        if (hxresStar[i] != hres[i])
          isAuthOk = false;
      }
    }
  // std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  // std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_nas_context/records/") +record_id ;
  // nlohmann::json udsf_ue_nas_context;
  // udsf_ue_nas_context["meta"] ["tags"] = {
  //                                     {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
  //                                     {"from_nf",nlohmann::json::array({"AMF_1234"})}
  //                                     } ;
  //  amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
  }
  // If success, start SMC procedure; else if failure, response registration
  // reject message with corresponding cause
  if (!isAuthOk) {
    Logger::amf_n1().error(
        "Authentication failed for UE with amf_ue_ngap_id 0x%x",
        amf_ue_ngap_id);
    response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE, ran_ue_ngap_id,
                                     amf_ue_ngap_id); // cause?
    return;
  } else {
    Logger::amf_n1().debug("Authentication successful by network!");
    if (!nc.get()->is_current_security_available) {
      Logger::amf_n1().debug("Security context with UE (imsi: %s) is not available, Start SMC procedure to Establish a new Context", nc.get()->imsi.c_str());
      if (!start_security_mode_control_procedure(nc)) {
        Logger::amf_n1().error("Start SMC procedure failure");
      } else {
        // ...
      }
    }else{
      Logger::amf_n1().debug("Security context with UE (imsi: %s) is available ...", nc.get()->imsi.c_str());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::authentication_failure_handle(uint32_t ran_ue_ngap_id,
                                           long amf_ue_ngap_id,
                                           bstring plain_msg) {
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json auts_json;
  nlohmann::json  is_common_procedure_for_authentication_running;
  std::shared_ptr<nas_context> nc;
  if (!is_amf_ue_id_2_nas_context(amf_ue_ngap_id)) {
    Logger::amf_n1().error(
        "No existed NAS context for UE with amf_ue_ngap_id(0x%x)",
        amf_ue_ngap_id);
    response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE, ran_ue_ngap_id,
                                     amf_ue_ngap_id); // cause?
    return;
  }
  nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  nc.get()->is_common_procedure_for_authentication_running = false;

  is_common_procedure_for_authentication_running["Content-ID"] = "is_common_procedure_for_authentication_running";
  is_common_procedure_for_authentication_running["Content-Type"] = "varchar(32)";
  is_common_procedure_for_authentication_running["content"] = to_string(nc.get()->is_common_procedure_for_authentication_running);
  udsf_nas_context["blocks"].push_back(is_common_procedure_for_authentication_running);

  // 1. decode AUTHENTICATION FAILURE message
  AuthenticationFailure *authFail = new AuthenticationFailure();
  authFail->decodefrombuffer(NULL, (uint8_t *)bdata(plain_msg),
                             blength(plain_msg));
  uint8_t mm_cause = authFail->get5GMmCause();
  if (mm_cause == -1) {
    Logger::amf_n1().error("Missing mandatory IE 5G_MM_CAUSE");
    response_registration_reject_msg(_5GMM_CAUSE_INVALID_MANDATORY_INFO,
                                     ran_ue_ngap_id,
                                     amf_ue_ngap_id); // cause?
    return;
  }
  switch (mm_cause) {
  case _5GMM_CAUSE_SYNCH_FAILURE: {
    Logger::amf_n1().debug("Initial new authentication procedure");
    bstring auts;
    if (!authFail->getAutsInAuthFailPara(auts)) {
      Logger::amf_n1().warn(
          "IE Authentication Failure Parameter (auts) not received");
    }
    nc.get()->auts = auts;
/*********************************hsx add**********************************/
    string auts_m;
    octet_stream_2_hex_stream((uint8_t*) bdata(nc.get()->auts), blength(nc.get()->auts), auts_m);
  auts_json["Content-ID"] = "auts";
  auts_json["Content-Type"] = "JSON";
  auts_json["content"] = auts_m;            
  udsf_nas_context["blocks"].push_back(auts_json); 

 std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id;
  udsf_nas_context["meta"] ["tags"] = {
                                        {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                        {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                        } ;    
  //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);

/*********************************hsx add**********************************/
    printf("Received auts: 0x ");
    for (int i = 0; i < blength(auts); i++)
      printf("%x ", ((uint8_t *)bdata(auts))[i]);
    printf("\n");
    if (auth_vectors_generator(nc)) { // all authentication in one(AMF)
      handle_auth_vector_successful_result(nc);
    } else {
      Logger::amf_n1().error("Request authentication vectors failure");
      response_registration_reject_msg(_5GMM_CAUSE_ILLEGAL_UE,
                                       nc.get()->ran_ue_ngap_id,
                                       nc.get()->amf_ue_ngap_id); // cause?
    }
    // authentication_failure_synch_failure_handle(nc, auts);
  } break;
  }
}

//------------------------------------------------------------------------------
bool amf_n1::start_security_mode_control_procedure(
    std::shared_ptr<nas_context> nc) {
#if 0
  nlohmann::json udsf_nas_context;
  nlohmann::json udsf_response;
  nlohmann::json is_current_security_available;
  nlohmann::json  is_common_procedure_for_security_mode_control_running;

  is_common_procedure_for_security_mode_control_running["Content-ID"] = "is_common_procedure_for_security_mode_control_running";
  is_common_procedure_for_security_mode_control_running["Content-Type"] = "varchar(32)";
  is_common_procedure_for_security_mode_control_running["content"] = to_string(nc.get()->is_common_procedure_for_security_mode_control_running);
  udsf_nas_context["blocks"].push_back(is_common_procedure_for_security_mode_control_running);
#endif



  Logger::amf_n1().debug("Start Security Mode Control procedure");
  nc.get()->is_common_procedure_for_security_mode_control_running = true;
  bool security_context_is_new = false;
  uint8_t amf_nea = EA0_5G;
  uint8_t amf_nia = IA0_5G;
  // decide which ea/ia alg used by UE, which is supported by network
  security_data_t *data = (security_data_t *)calloc(1, sizeof(security_data_t));
  nas_secu_ctx *secu_ctx = new nas_secu_ctx();
  nc.get()->security_ctx = secu_ctx;
  if (!data)
    Logger::amf_n1().error("Cannot allocate memory for security_data_t");

  if (secu_ctx->sc_type == SECURITY_CTX_TYPE_NOT_AVAILABLE &&
      nc.get()->is_common_procedure_for_security_mode_control_running) {
    Logger::amf_n1().debug(
        "Using INTEGRITY_PROTECTED_WITH_NEW_SECU_CTX for SecurityModeControl "
        "message");
    data->saved_selected_nea =
        secu_ctx->nas_algs
            .encryption; // emm_ctx->_security.selected_algorithms.encryption;
    data->saved_selected_nia = secu_ctx->nas_algs.integrity;
    data->saved_ngksi = secu_ctx->ngksi;
    data->saved_overflow =
        secu_ctx->dl_count.overflow; // emm_ctx->_security.dl_count.overflow;
    data->saved_seq_num = secu_ctx->dl_count.seq_num;
    data->saved_sc_type = secu_ctx->sc_type;
    secu_ctx->ngksi = nc.get()->ngKsi;
    secu_ctx->dl_count.overflow = 0;
    secu_ctx->dl_count.seq_num = 0;
    secu_ctx->ul_count.overflow = 0;
    secu_ctx->ul_count.seq_num = 0;
    int rc = security_select_algorithms(nc.get()->ueSecurityCapEnc,
                                        nc.get()->ueSecurityCapInt, amf_nea,
                                        amf_nia);
    secu_ctx->nas_algs.integrity = amf_nia;
    secu_ctx->nas_algs.encryption = amf_nea;
    secu_ctx->sc_type = SECURITY_CTX_TYPE_FULL_NATIVE;
    Authentication_5gaka::derive_knas(NAS_INT_ALG, secu_ctx->nas_algs.integrity,
                                      nc.get()->kamf[secu_ctx->vector_pointer],
                                      secu_ctx->knas_int);
    Authentication_5gaka::derive_knas(
        NAS_ENC_ALG, secu_ctx->nas_algs.encryption,
        nc.get()->kamf[secu_ctx->vector_pointer], secu_ctx->knas_enc);
    print_buffer("amf_n1","dukl in SMC, Knas_int", secu_ctx->knas_int, 16);
    security_context_is_new = true;
    nc.get()->is_current_security_available = true;
#if 0
    is_current_security_available["Content-ID"] = "is_current_security_available";
    is_current_security_available["Content-Type"] = "varchar(32)";
    is_current_security_available ["content"] = to_string( nc.get()->is_current_security_available);
    udsf_nas_context["blocks"].push_back(is_current_security_available);
#endif
    // nlohmann::json security_ctx;
    // nlohmann::json ue_algorithms;
    // nlohmann::json nas_algs;
    // nlohmann::json ul_count;
    // nlohmann::json dl_count;
    // security_ctx["Content-ID"] = "security_ctx";
    // security_ctx["Content-Type"] = "JSON";
    // security_ctx["content"] = {};
    // security_ctx["content"]["vector_pointer"] = to_string(nc->security_ctx->vector_pointer);
    // security_ctx["content"]["sc_type"] = to_string(nc->security_ctx->sc_type);
    // security_ctx["content"]["ngksi"] = to_string(nc->security_ctx->ngksi);
    // security_ctx["content"]["knas_enc"] = to_string(*nc->security_ctx->knas_enc);
    // security_ctx["content"]["knas_int"] = to_string(*nc->security_ctx->knas_int);
    // ul_count["spare"]=to_string(nc->security_ctx->ul_count.spare);
    // ul_count["overflow"]=to_string(nc->security_ctx->ul_count.overflow);
    // ul_count["seq_num"]=to_string(nc->security_ctx->ul_count.seq_num);
    // security_ctx.push_back(ul_count);
    // dl_count["spare"]=to_string(nc->security_ctx->dl_count.spare);
    // dl_count["overflow"]=to_string(nc->security_ctx->dl_count.overflow);
    // dl_count["seq_num"]=to_string(nc->security_ctx->dl_count.seq_num);
    // security_ctx.push_back(dl_count);
    // ue_algorithms["_5gs_encryption"]=to_string(nc->security_ctx->ue_algorithms._5gs_encryption);
    // ue_algorithms["overflow"]=to_string(nc->security_ctx->ue_algorithms._5gs_integrity);
    // security_ctx.push_back(ue_algorithms);
    // nas_algs["encryption"]=to_string(nc->security_ctx->nas_algs.encryption);
    // nas_algs["integrity"]=to_string(nc->security_ctx->nas_algs.integrity);
    // security_ctx.push_back(nas_algs);
    // udsf_nas_context["blocks"].push_back(security_ctx);


  }

  SecurityModeCommand *smc = new SecurityModeCommand();
  smc->setHeader(PLAIN_5GS_MSG);
  smc->setNAS_Security_Algorithms(amf_nea, amf_nia);
  Logger::amf_n1().debug("Encoded ngKSI 0x%x", nc.get()->ngKsi);
  smc->setngKSI(NAS_KEY_SET_IDENTIFIER_NATIVE, nc.get()->ngKsi & 0x07);
  smc->setUE_Security_Capability(nc.get()->ueSecurityCapEnc,
                                 nc.get()->ueSecurityCapInt);
  if (smc->ie_ue_security_capability != NULL) {
    smc->ie_ue_security_capability->setLenght(nc.get()->ueSecurityCaplen);
    smc->ie_ue_security_capability->setEEASel(nc.get()->ueSecurityCapEEnc);
    smc->ie_ue_security_capability->setEIASel(nc.get()->ueSecurityCapEInt);
  } else {
    Logger::amf_n1().error("smc->ie_ue_security_capability is NULL");
  }

#if 0
  std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id ;
  udsf_nas_context["meta"] ["tags"] = {
                                        {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                        {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                        } ;    
  //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
#endif


  smc->setIMEISV_Request(0xe1);
  smc->setAdditional_5G_Security_Information(true, false);
  uint8_t buffer[1024];
  int encoded_size = smc->encode2buffer(buffer, 1024);
  print_buffer("amf_n1", "Security-Mode-Command message buffer", buffer,
               encoded_size);
  bstring intProtctedNas;
  encode_nas_message_protected(
      secu_ctx, security_context_is_new, INTEGRITY_PROTECTED_WITH_NEW_SECU_CTX,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, intProtctedNas);
  print_buffer("amf_n1", "Encrypted Security-Mode-Command message buffer",
               (uint8_t *)bdata(intProtctedNas), blength(intProtctedNas));
  itti_send_dl_nas_buffer_to_task_n2(intProtctedNas, nc.get()->ran_ue_ngap_id,
                                     nc.get()->amf_ue_ngap_id);
  // secu_ctx->dl_count.seq_num ++;
  http_update_nas_context_into_udsf(nc);
  Logger::amf_n1().debug("dukl security_mode_command dl.seq %d",nc.get()->security_ctx->dl_count.seq_num);
  return true;
}

//------------------------------------------------------------------------------
int amf_n1::security_select_algorithms(uint8_t nea, uint8_t nia,
                                       uint8_t &amf_nea, uint8_t &amf_nia) {
  for (int i = 0; i < 8; i++) {
    if (nea & (0x80 >> amf_cfg.nas_cfg.prefered_ciphering_algorithm[i])) {
      amf_nea = amf_cfg.nas_cfg.prefered_ciphering_algorithm[i];
      printf("amf_nea: 0x%x\n", amf_nea);
      break;
    }
  }
  for (int i = 0; i < 8; i++) {
    if (nia & (0x80 >> amf_cfg.nas_cfg.prefered_integrity_algorithm[i])) {
      amf_nia = amf_cfg.nas_cfg.prefered_integrity_algorithm[i];
      printf("amf_nia: 0x%x\n", amf_nia);
      break;
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
void amf_n1::security_mode_complete_handle(uint32_t ran_ue_ngap_id,
                                           long amf_ue_ngap_id,
                                           bstring nas_msg) {
  nlohmann::json udsf_nas_context;
  Logger::amf_n1().debug("Handling security mode complete ...");
  string ue_context_key = "app_ue_ranid_" + to_string(ran_ue_ngap_id) +
                          "-amfid_" + to_string(amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc;
  uc = amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
  if (uc.get() == nullptr) {
    // TODO:
    Logger::amf_n1().error(
        "ue_context in amf_app using ran_amf_id (%s) does not existed!",
        ue_context_key.c_str());
  }
  Logger::amf_n1().info(
      "Found ue_context (%p) in amf_app using ran_amf_id (%s)", uc.get(),
      ue_context_key.c_str());

  // encoding REGISTRATION ACCEPT
  RegistrationAccept *regAccept = new RegistrationAccept();
  regAccept->setHeader(PLAIN_5GS_MSG);
  regAccept->set_5GS_Registration_Result(false, false, false, 0x01);
  std::string mcc;
  std::string mnc;
  uint32_t tmsi = 0;
  if (!amf_app_inst->generate_5g_guti(ran_ue_ngap_id, amf_ue_ngap_id, mcc, mnc,
                                      tmsi)) {
    Logger::amf_n1().error("Generate 5G GUTI error! exit");
    // TODO:
    return;
  }
  regAccept->set5G_GUTI(mcc, mnc, amf_cfg.guami.regionID,
                        amf_cfg.guami.AmfSetID, amf_cfg.guami.AmfPointer, tmsi);

  std::vector<p_tai_t> tai_list = {};
  p_tai_t item0 = {};
  item0.type = 0x00;
  nas_plmn_t plmn;
  plmn.mcc = amf_cfg.plmn_list[0].mcc;
  plmn.mnc = amf_cfg.plmn_list[0].mnc;
  item0.plmn_list.push_back(plmn);
  item0.tac_list.push_back(amf_cfg.plmn_list[0].tac);
  tai_list.push_back(item0);
  regAccept->setTaiList(tai_list);

  std::vector<struct SNSSAI_s> nssai = {};
  SNSSAI_t snssai = {};
  snssai.sst = 1;
  snssai.sd = -1;
  snssai.mHplmnSst = -1;
  snssai.mHplmnSd = -1;
  nssai.push_back(snssai);
  regAccept->setALLOWED_NSSAI(nssai);

  std::string guti = mcc + mnc + amf_cfg.guami.regionID +
                     amf_cfg.guami.AmfSetID + amf_cfg.guami.AmfPointer +
                     std::to_string(tmsi);
  Logger::amf_n1().debug("Allocated GUTI %s", guti.c_str());

  // TODO: remove hardcoded values
  regAccept->set_5GS_Network_Feature_Support(0x01, 0x00);
  char *pathvar = NULL;
  pathvar = getenv("AMF_T3512_MIN");
  uint8_t t3512_Value = 0x1e;
  if(pathvar)
  {
    uint8_t value = atoi(pathvar);
    if(value)
    {
        t3512_Value = value;
    }
  }
  else
  {
    Logger::amf_n1().warn("Please set environment variable \"AMF_T3512_MIN\"");
  }
  regAccept->setT3512_Value(0x5, t3512_Value);
  uint8_t buffer[BUFFER_SIZE_1024] = {0};
  int encoded_size = regAccept->encode2buffer(buffer, BUFFER_SIZE_1024);
  print_buffer("amf_n1", "Registration-Accept message buffer", buffer,
               encoded_size);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Registration-Accept message error");
    return;
  } else {
    delete regAccept;
  }

  if (!uc.get()->isUeContextRequest) {
    Logger::amf_n1().debug(
        "UE Context is not requested, UE with ran_ue_ngap_id %d, "
        "amf_ue_ngap_id %d attached",
        ran_ue_ngap_id, amf_ue_ngap_id);
    // send registration accept back
  } else {
    // encoding InitialContextSetupRequest(NGAP message) back
    std::shared_ptr<nas_context> nc;
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
    Logger::amf_n1().info(
        "UE (IMSI %s, GUTI %s, current RAN ID %d, current AMF ID %d) has been "
        "registered to the network",
        nc.get()->imsi.c_str(), guti.c_str(), ran_ue_ngap_id, amf_ue_ngap_id);
    if (nc.get()->is_stacs_available) {
      /* int index = 0;
       for (int i = 0; i < stacs.ues.size(); i++) {
       if (!(nc.get()->imsi).compare(stacs.ues[i].imsi)) {
       index = i;
       break;
       }
       }
       */
      stacs.update_5gmm_state(nc.get()->imsi, "5GMM-REGISTERED");
      // update_ue_information_statics(stacs.ues[index], "", "RM-REGISTRED",
      // ran_ue_ngap_id, amf_ue_ngap_id, "", guti, "", "", 0);
    } else {
      // ue_infos ueItem;
      // update_ue_information_statics(ueItem, "CM-CONNECTED",
      // "REGISTRATION-REGISTRED", ran_ue_ngap_id, amf_ue_ngap_id,
      // nc.get()->imsi, "", mcc, mnc, uc.get()->cgi.nrCellID);
      // stacs.ues.push_back(ueItem);
       nlohmann::json is_stacs_available;
      nc.get()->is_stacs_available = true;
      is_stacs_available["Content-ID"] = "is_stacs_available";
      is_stacs_available["Content-Type"] = "varchar(32)";
      is_stacs_available["content"] = to_string(nc.get()->is_stacs_available);
      udsf_nas_context["blocks"].push_back(is_stacs_available);
    }
    set_5gmm_state(nc, _5GMM_REGISTERED);

    set_guti_2_nas_context(guti, nc);
    nc.get()->guti = guti;
    nc.get()->is_common_procedure_for_security_mode_control_running = false;
    nlohmann::json is_common_procedure_for_security_mode_control_running;
    is_common_procedure_for_security_mode_control_running["Content-ID"] = "is_common_procedure_for_security_mode_control_running";
    is_common_procedure_for_security_mode_control_running["Content-Type"] = "varchar(32)";
    is_common_procedure_for_security_mode_control_running["content"] = to_string(nc.get()->is_common_procedure_for_security_mode_control_running);
    udsf_nas_context["blocks"].push_back(is_common_procedure_for_security_mode_control_running);

    nlohmann::json guti_udsf;
    guti_udsf["Content-ID"] = "guti_udsf";
    guti_udsf["Content-Type"] = "varchar(32)";
    guti_udsf["content"] = nc.get()->guti;
    udsf_nas_context["blocks"].push_back(guti_udsf);

    nas_secu_ctx *secu = nc.get()->security_ctx;
    // protect nas message
    bstring protectedNas;
    encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                                 NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                                 protectedNas);
    uint8_t *kamf = nc.get()->kamf[secu->vector_pointer];
    uint8_t kgnb[32];
    uint32_t ulcount = secu->ul_count.seq_num | (secu->ul_count.overflow << 8);
    Authentication_5gaka::derive_kgnb(0, 0x01, kamf, kgnb);
    ncc = 1;
    print_buffer("amf_n1", "kamf", kamf, 32);
    // Authentication_5gaka::derive_kgnb(ulcount, 0x01, kamf, kgnb);
    bstring kgnb_bs = blk2bstr(kgnb, 32);
    itti_initial_context_setup_request *itti_msg =
        new itti_initial_context_setup_request(TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->kgnb = kgnb_bs;
    itti_msg->nas = protectedNas;
    itti_msg->is_sr = false; // TODO: for Setup Request procedure

    http_update_nas_context_into_udsf(nc);
    Logger::amf_n1().debug("dukl security mode complete dl.seq %d",nc.get()->security_ctx->dl_count.seq_num);

#if 0
/****************************hsx add*****************************/
    std::string udsf_url = "http://10.28.234.76:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + to_string(nc.get()->amf_ue_ngap_id) ;
    nlohmann::json udsf_response;
    udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;    
    amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);

/****************************hsx add*****************************/  
#endif
    std::shared_ptr<itti_initial_context_setup_request> i =
        std::shared_ptr<itti_initial_context_setup_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          i->get_msg_name());
    }
  }

}

//------------------------------------------------------------------------------
void amf_n1::security_mode_reject_handle(uint32_t ran_ue_ngap_id,
                                         long amf_ue_ngap_id, bstring nas_msg) {
  Logger::amf_n1().debug(
      "Receiving security mode reject message, handling ...");
}

void amf_n1::registration_complete_handle(uint32_t ran_ue_ngap_id,
                                          long amf_ue_ngap_id,
                                          bstring nas_msg) {
  Logger::amf_n1().debug(
      "receiving registration complete, encoding Configuration Update Command");

  time_t tt;
  time(&tt);
  tt = tt + 8 * 3600; // transform the time zone
  tm *t = gmtime(&tt);

  uint8_t conf[45] = {0};
  uint8_t header[3] = {0x7e, 0x00, 0x54};
  uint8_t full_name[18] = {0x43, 0x10, 0x81, 0xc1, 0x76, 0x58,
                           0x9e, 0x9e, 0xbf, 0xcd, 0x74, 0x90,
                           0xb3, 0x4c, 0xbf, 0xbf, 0xe5, 0x6b};
  uint8_t short_name[11] = {0x45, 0x09, 0x81, 0xc1, 0x76, 0x58,
                            0x9e, 0x9e, 0xbf, 0xcd, 0x74};
  uint8_t time_zone[2] = {0x46, 0x23};
  uint8_t time[8] = {0};
  time[0] = 0x47;
  time[1] = 0x12;
  time[2] = ((t->tm_mon + 1) & 0x0f) << 4 | ((t->tm_mon + 1) & 0xf0) >> 4;
  time[3] = ((t->tm_mday + 1) & 0x0f) << 4 | ((t->tm_mday + 1) & 0xf0) >> 4;
  time[4] = ((t->tm_hour + 1) & 0x0f) << 4 | ((t->tm_hour + 1) & 0xf0) >> 4;
  time[5] = ((t->tm_min + 1) & 0x0f) << 4 | ((t->tm_min + 1) & 0xf0) >> 4;
  time[6] = ((t->tm_sec + 1) & 0x0f) << 4 | ((t->tm_sec + 1) & 0xf0) >> 4;
  time[7] = 0x23;
  uint8_t daylight[3] = {0x49, 0x01, 0x00};
  memcpy(conf, header, 3);
  memcpy(conf + 3, full_name, 18);
  memcpy(conf + 21, short_name, 11);
  memcpy(conf + 32, time_zone, 2);
  memcpy(conf + 34, time, 8);
  memcpy(conf + 42, daylight, 3);

  std::shared_ptr<nas_context> nc;
  if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id))
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  else {
    Logger::amf_n1().warn("No existed nas_context with amf_ue_ngap_id(0x%x)",
                          amf_ue_ngap_id);
    return;
  }
  nas_secu_ctx *secu = nc.get()->security_ctx;

#if 0
//向UDSF 同步全部NAS消息
/**********************   hxs add *******************************/
    string auts;
    octet_stream_2_hex_stream((uint8_t*) bdata(nc.get()->auts), blength(nc.get()->auts), auts);
    std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") +record_id ;
    nlohmann::json udsf_nas_context;
    nlohmann::json udsf_response;
    udsf_nas_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;    
    udsf_nas_context["blocks"] = nlohmann::json::array({
                                                  {{"Content-ID", "ctx_avaliability_ind"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ctx_avaliability_ind)}},
                                                  {{"Content-ID", "is_stacs_available"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->is_stacs_available)}},
                                                  {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->amf_ue_ngap_id)}},
                                                  {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ran_ue_ngap_id)}},
                                                  {{"Content-ID", "nas_status"},{"Content-Type", "varchar(32)"},{"content", nc.get()->nas_status }},
                                                  {{"Content-ID", "_5gmm_state"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->_5gmm_state)}},
                                                  
                                                  {{"Content-ID", "registration_type"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->registration_type)}},
                                                  {{"Content-ID", "follow_on_req_pending_ind"},{"Content-Type", "varchar(32)"},{"content",  to_string( nc.get()->follow_on_req_pending_ind)}},
                                                  {{"Content-ID", "ngKsi"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->ngKsi)}},
                                                  
                                                   {{"Content-ID", "guti"},{"Content-Type", "varchar(32)"},{"content", nc.get()->guti}},
                                                  {{"Content-ID", "imsi"},{"Content-Type", "varchar(32)"},{"content", nc->imsi}},
                                                  {{"Content-ID", "mmCapability"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->mmCapability)}},
                                                  {{"Content-ID", "ueSecurityCaplen"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCaplen)}},
                                                  {{"Content-ID", "ueSecurityCapEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEnc)}},
                                                  {{"Content-ID", "ueSecurityCapInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapInt)}},
                                                  {{"Content-ID", "ueSecurityCapEEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEEnc)}},
                                                  {{"Content-ID", "ueSecurityCapEInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEInt)}},
                                                  {{"Content-ID", "serving_network"},{"Content-Type", "varchar(32)"},{"content",  nc.get()->serving_network}},
                                                  {{"Content-ID", "auts"},{"Content-Type", "varchar(32)"},{"content",  auts}},
                                                  
                                                  {{"Content-ID", "is_specific_procedure_for_registration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_registration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_deregistration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_deregistration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_eCell_inactivity_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_eCell_inactivity_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_authentication_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_authentication_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_identification_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_identification_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_security_mode_control_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_security_mode_control_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_nas_transport_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_nas_transport_running)}},
                                                  
                                                  {{"Content-ID", "Href"},{"Content-Type", "varchar(1024)"},{"content",  nc.get()->Href}},
                                                  {{"Content-ID", "is_current_security_available"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_current_security_available)}},
                                                  {{"Content-ID", "registration_attempt_counter"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->registration_attempt_counter)}},
                                                  
                                                  {{"Content-ID", "is_imsi_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_imsi_present)}},
                                                  {{"Content-ID", "is_5g_guti_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_5g_guti_present)}},
                                                  {{"Content-ID", "is_auth_vectors_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_auth_vectors_present)}},
                                                  {{"Content-ID", "to_be_register_by_new_suci"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->to_be_register_by_new_suci)}},
                                              });
   // std::vector<nas::SNSSAI_t> requestedNssai
    nlohmann::json requestedNssai = {};
    nlohmann::json requestedNssai_m = {};
    requestedNssai["Content-ID"]="requestedNssai";
    requestedNssai["Content-Type"]="JSON";
    requestedNssai["content"] = {};
    for(int i=0;i<nc.get()->requestedNssai.size();i++)
      {
       requestedNssai_m["sst"]=to_string(nc.get()->requestedNssai[i].sst);
       requestedNssai_m["sd"]=to_string(nc.get()->requestedNssai[i].sd);
       requestedNssai_m["mHplmnSst"]=to_string(nc.get()->requestedNssai[i].mHplmnSst);
       requestedNssai_m["mHplmnSd"]=to_string(nc.get()->requestedNssai[i].mHplmnSd);
       requestedNssai["content"].push_back(requestedNssai_m);

      }
    udsf_nas_context["blocks"].push_back(requestedNssai);

//auc_vector_t _vector[MAX_5GS_AUTH_VECTORS]

    nlohmann::json _vector = {};
    nlohmann::json _vector_m = {};
    _vector["Content-ID"]="_vector";
    _vector["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _vector_m["rand_new"]=to_string(nc.get()->_vector[i].rand_new);
      _vector_m["rand"]=to_string(*(nc.get()->_vector[i].rand));
      _vector_m["xres"]=to_string(*nc.get()->_vector[i].xres);
      _vector_m["autn"]=to_string(*nc.get()->_vector[i].autn);
      _vector_m["kasme"]=to_string(*nc.get()->_vector[i].kasme);
      _vector["content"].push_back(_vector_m);

    }
    udsf_nas_context["blocks"].push_back(_vector);

//_5g_he_av
    nlohmann::json _5g_he_av = {};
    nlohmann::json _5g_he_av_m = {};
    _5g_he_av["Content-ID"]="_5g_he_av";
    _5g_he_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_he_av_m["avType"]=to_string(nc.get()->_5g_he_av[i].avType);
      _5g_he_av_m["rand"]=to_string(*(nc.get()->_5g_he_av[i].rand));
      _5g_he_av_m["xres"]=to_string(*nc.get()->_5g_he_av[i].xres);
      _5g_he_av_m["autn"]=to_string(*nc.get()->_5g_he_av[i].autn);
      _5g_he_av_m["xresStar"]=to_string(*nc.get()->_5g_he_av[i].xresStar);
      _5g_he_av_m["kausf"]=to_string(*nc.get()->_5g_he_av[i].kausf);
      _5g_he_av["content"].push_back(_5g_he_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_he_av);

//_5g_av
    nlohmann::json _5g_av = {};
    nlohmann::json _5g_av_m = {};
    _5g_av["Content-ID"]="_5g_av";
    _5g_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_av_m["avType"]=to_string(nc.get()->_5g_av[i].avType);
      _5g_av_m["rand"]=to_string(*(nc.get()->_5g_av[i].rand));
      _5g_av_m["hxres"]=to_string(*nc.get()->_5g_av[i].hxres);
      _5g_av_m["autn"]=to_string(*nc.get()->_5g_av[i].autn);
      _5g_av_m["hxresStar"]=to_string(*nc.get()->_5g_av[i].hxresStar);
      _5g_av_m["kseaf"]=to_string(*nc.get()->_5g_av[i].kseaf);
      _5g_av["content"].push_back(_5g_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_av);

//kamf
    nlohmann::json kamf = {};
    nlohmann::json kamf_m = {};
    kamf["Content-ID"]="kamf";
    kamf["Content-Type"]="JSON";
    // for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++){
    //   for(int j=0;j<32;j++)
    //   {
    //     kamf_m=to_string(nc.get()->kamf[i][j]);
    //     printf("--------------kamf_m =========== %s\n",kamf_m.dump().c_str());
    //   }
    // kamf["content"].push_back(kamf_m);
    // }
    string  kamf_str;
    octet_stream_2_hex_stream(nc.get()->kamf[0],32,kamf_str);
    kamf["content"] = kamf_str;
    udsf_nas_context["blocks"].push_back(kamf);
    printf("--------------kamf =========== %s\n",kamf_str.c_str());


//security_context_t _security和nas_secu_ctx* security_ctx
  nlohmann::json _security = {};
  nlohmann::json ul_count = {};
  nlohmann::json dl_count = {};
  nlohmann::json capability = {};
  nlohmann::json selected_algorithms = {};
  _security["Content-ID"] = "_security";
  _security["Content-Type"] = "JSON";
  _security["content"]["vector_index"] = to_string(nc.get()->_security.vector_index);
  _security["content"]["knas_enc"] = to_string(*nc.get()->_security.knas_enc);
  _security["content"]["knas_int"] = to_string(*nc.get()->_security.knas_int);
  _security["content"]["activated"] = to_string(nc.get()->_security.activated);
  ul_count["spare"]=to_string(nc.get()->_security.ul_count.spare);
  ul_count["overflow"]=to_string(nc.get()->_security.ul_count.overflow);
  ul_count["seq_num"]=to_string(nc.get()->_security.ul_count.seq_num);
  _security.update(ul_count);
  dl_count["spare"]=to_string(nc.get()->_security.dl_count.spare);
  dl_count["overflow"]=to_string(nc.get()->_security.dl_count.overflow);
  dl_count["seq_num"]=to_string(nc.get()->_security.dl_count.seq_num);
  _security.update(dl_count);
  capability["eps_encryption"]=to_string(nc.get()->_security.capability.eps_encryption);
  capability["eps_integrity"]=to_string(nc.get()->_security.capability.eps_integrity);
  capability["umts_encryption"]=to_string(nc.get()->_security.capability.umts_encryption);
  capability["umts_integrity"]=to_string(nc.get()->_security.capability.umts_integrity);
  capability["gprs_encryption"]=to_string(nc.get()->_security.capability.gprs_encryption);
  capability["umts_present"]=to_string(nc.get()->_security.capability.umts_present);
  capability["gprs_present"]=to_string(nc.get()->_security.capability.gprs_present);
  _security.update(capability);
  selected_algorithms["encryption"]=to_string(nc.get()->_security.selected_algorithms.encryption);
  selected_algorithms["integrity"]=to_string(nc.get()->_security.selected_algorithms.encryption);
  _security.update(selected_algorithms);
  udsf_nas_context["blocks"].push_back(_security);

 nlohmann::json security_ctx;
  nlohmann::json ue_algorithms;
  nlohmann::json nas_algs;
  nlohmann::json vector_pointer;
  nlohmann::json sc_type;
  nlohmann::json ngksi;
  nlohmann::json knas_enc;
  nlohmann::json knas_int;
  // nlohmann::json ul_count;
  // nlohmann::json dl_count;
  security_ctx["Content-ID"] = "security_ctx";
  security_ctx["Content-Type"] = "JSON";
  security_ctx["content"] = {};

  vector_pointer["Content-ID"] = "vector_pointer";
  vector_pointer["Content-Type"] = "varchar(32)";
  vector_pointer["content"]=to_string(nc.get()->security_ctx->vector_pointer);
  security_ctx["content"].push_back(vector_pointer);

  sc_type["Content-ID"] = "sc_type";
  sc_type["Content-Type"] = "varchar(32)";
  sc_type["content"]=to_string(nc.get()->security_ctx->sc_type);
  security_ctx["content"].push_back(sc_type);

  ngksi["Content-ID"] = "ngksi";
  ngksi["Content-Type"] = "varchar(32)";
  ngksi["content"]=to_string(nc.get()->security_ctx->ngksi);
  security_ctx["content"].push_back(ngksi);

  knas_enc["Content-ID"] = "knas_enc";
  knas_enc["Content-Type"] = "varchar(32)";
  string  knas_enc_str;
  octet_stream_2_hex_stream(nc.get()->security_ctx->knas_enc,AUTH_KNAS_INT_SIZE,knas_enc_str);
  printf("-------------knas_enc string--------in amf 1------%s \n",knas_enc_str.c_str());
    for(int ly=0;ly<16;ly++)
  {
    printf("-------------knas_enc--------in amf 1------%x \n",nc.get()->security_ctx->knas_enc[ly]);
  }
  knas_enc["content"]=knas_enc_str;
  security_ctx["content"].push_back(knas_enc);

  knas_int["Content-ID"] = "knas_int";
  knas_int["Content-Type"] = "varchar(32)";
  //knas_int["content"]=to_string(*nc.get()->security_ctx->knas_int);
  string  knas_int_str;
  octet_stream_2_hex_stream(nc.get()->security_ctx->knas_int,AUTH_KNAS_INT_SIZE,knas_int_str);
  printf("-------------knas_int string--------in amf 1------%s \n",knas_int_str.c_str());
    for(int ly=0;ly<16;ly++)
  {
    printf("-------------knas_int--------in amf 1------%x \n",nc.get()->security_ctx->knas_int[ly]);
  }
  knas_int["content"]=knas_int_str;
  security_ctx["content"].push_back(knas_int);

  
  ul_count["Content-ID"] = "ul_count";
  ul_count["Content-Type"] = "JSON";
  ul_count["spare"]=to_string(nc.get()->security_ctx->ul_count.spare);
  ul_count["overflow"]=to_string(nc.get()->security_ctx->ul_count.overflow);
  ul_count["seq_num"]=to_string(nc.get()->security_ctx->ul_count.seq_num);
  security_ctx["content"].push_back(ul_count);
  dl_count["Content-ID"] = "dl_count";
  dl_count["Content-Type"] = "JSON";
  dl_count["spare"]=to_string(nc.get()->security_ctx->dl_count.spare);
  dl_count["overflow"]=to_string(nc.get()->security_ctx->dl_count.overflow);
  dl_count["seq_num"]=to_string(nc.get()->security_ctx->dl_count.seq_num);
  security_ctx["content"].push_back(dl_count);
  ue_algorithms["Content-ID"] = "ue_algorithms";
  ue_algorithms["Content-Type"] = "JSON";
  ue_algorithms["_5gs_encryption"]=to_string(nc.get()->security_ctx->ue_algorithms._5gs_encryption);
  ue_algorithms["_5gs_integrity"]=to_string(nc.get()->security_ctx->ue_algorithms._5gs_integrity);
  security_ctx["content"].push_back(ue_algorithms);
  nas_algs["Content-ID"] = "nas_algs";
  nas_algs["Content-Type"] = "JSON";
  nas_algs["encryption"]=to_string(nc.get()->security_ctx->nas_algs.encryption);
  nas_algs["integrity"]=to_string(nc.get()->security_ctx->nas_algs.integrity);
  security_ctx["content"].push_back(nas_algs);

  udsf_nas_context["blocks"].push_back(security_ctx);

  amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);

    /**********************   hxs add *******************************/
#endif
  // protect nas message
  bstring protectedNas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK, conf, 45, protectedNas);
  http_update_nas_context_into_udsf(nc);
  Logger::amf_n1().debug("dukl registration complete complete dl.seq %d",nc.get()->security_ctx->dl_count.seq_num);

  itti_send_dl_nas_buffer_to_task_n2(protectedNas, ran_ue_ngap_id,
                                     amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
void amf_n1::encode_nas_message_protected(
    nas_secu_ctx *nsc, bool is_secu_ctx_new, uint8_t security_header_type,
    uint8_t direction, uint8_t *input_nas_buf, int input_nas_len,
    bstring &protected_nas) {
  Logger::amf_n1().debug("Encoding nas_message_protected...");
  uint8_t protected_nas_buf[1024];
  int encoded_size = 0;
  switch (security_header_type & 0x0f) {
  case INTEGRITY_PROTECTED: {
  } break;
  case INTEGRITY_PROTECTED_AND_CIPHERED: {
    bstring input = blk2bstr(input_nas_buf, input_nas_len);
    bstring ciphered;
    nas_message_cipher_protected(nsc, NAS_MESSAGE_DOWNLINK, input, ciphered);
    protected_nas_buf[0] = EPD_5GS_MM_MSG;
    protected_nas_buf[1] = INTEGRITY_PROTECTED_AND_CIPHERED;
    protected_nas_buf[6] = (uint8_t)nsc->dl_count.seq_num;
    memcpy(&protected_nas_buf[7], (uint8_t *)bdata(ciphered),
           blength(ciphered));
    uint32_t mac32;
    if (!(nas_message_integrity_protected(nsc, NAS_MESSAGE_DOWNLINK,
                                          protected_nas_buf + 6,
                                          input_nas_len + 1, mac32))) {
      memcpy(protected_nas_buf, input_nas_buf, input_nas_len);
      encoded_size = input_nas_len;
    } else {
      *(uint32_t *)(protected_nas_buf + 2) = htonl(mac32);
      encoded_size = 7 + input_nas_len;
    }
  } break;
  case INTEGRITY_PROTECTED_WITH_NEW_SECU_CTX: {
    if (!nsc || !is_secu_ctx_new) {
      Logger::amf_n1().error("Security context is too old");
    }
    protected_nas_buf[0] = EPD_5GS_MM_MSG;
    protected_nas_buf[1] = INTEGRITY_PROTECTED_WITH_NEW_SECU_CTX;
    protected_nas_buf[6] = (uint8_t)nsc->dl_count.seq_num;
    memcpy(&protected_nas_buf[7], input_nas_buf, input_nas_len);
    uint32_t mac32;
    if (!(nas_message_integrity_protected(nsc, NAS_MESSAGE_DOWNLINK,
                                          protected_nas_buf + 6,
                                          input_nas_len + 1, mac32))) {
      memcpy(protected_nas_buf, input_nas_buf, input_nas_len);
      encoded_size = input_nas_len;
    } else {
      Logger::amf_n1().debug("mac32: 0x%x", mac32);
      *(uint32_t *)(protected_nas_buf + 2) = htonl(mac32);
      encoded_size = 7 + input_nas_len;
    }
  } break;
  case INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECU_CTX: {
  } break;
  }
  protected_nas = blk2bstr(protected_nas_buf, encoded_size);
  nsc->dl_count.seq_num++;
}

//------------------------------------------------------------------------------
bool amf_n1::nas_message_integrity_protected(nas_secu_ctx *nsc,
                                             uint8_t direction,
                                             uint8_t *input_nas,
                                             int input_nas_len,
                                             uint32_t &mac32) {
  uint32_t count = 0x00000000;
  if (direction) {
    count = 0x00000000 | ((nsc->dl_count.overflow & 0x0000ffff) << 8) |
            ((nsc->dl_count.seq_num & 0x000000ff));
  } else {
    count = 0x00000000 | ((nsc->ul_count.overflow & 0x0000ffff) << 8) |
            ((nsc->ul_count.seq_num & 0x000000ff));
  }
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = nsc->knas_int;
  print_buffer("amf_n1", "Parameters for NIA: knas_int", nsc->knas_int,
               AUTH_KNAS_INT_SIZE);
  stream_cipher.key_length = AUTH_KNAS_INT_SIZE;
  stream_cipher.count = *(input_nas);
  // stream_cipher.count = count;
  if (!direction) {
    nsc->ul_count.seq_num = stream_cipher.count;
    Logger::amf_n1().debug("Uplink count in uplink: %d", nsc->ul_count.seq_num);
  }
  Logger::amf_n1().debug("Parameters for NIA, count: 0x%x", count);
  stream_cipher.bearer = 0x01; // 33.501 section 8.1.1
  Logger::amf_n1().debug("Parameters for NIA, bearer: 0x%x",
                         stream_cipher.bearer);
  stream_cipher.direction = direction; // "1" for downlink
  Logger::amf_n1().debug("Parameters for NIA, direction: 0x%x", direction);
  stream_cipher.message = (uint8_t *)input_nas;
  print_buffer("amf_n1", "Parameters for NIA, message: ", input_nas,
               input_nas_len);
  stream_cipher.blength = input_nas_len * 8;
  switch (nsc->nas_algs.integrity & 0x0f) {
  case IA0_5G: {
    Logger::amf_n1().debug("Integrity with algorithms: 5G-IA0");
    return false; // plain msg
  } break;
  case IA1_128_5G: {
    Logger::amf_n1().debug("Integrity with algorithms: 128-5G-IA1");
    nas_algorithms::nas_stream_encrypt_nia1(&stream_cipher, mac);
    print_buffer("amf_n1", "Result for NIA1, mac: ", mac, 4);
    mac32 = ntohl(*((uint32_t *)mac));
    Logger::amf_n1().debug("Result for NIA1, mac32: 0x%x", mac32);
    return true;
  } break;
  case IA2_128_5G: {
    Logger::amf_n1().debug("Integrity with algorithms: 128-5G-IA2");
    nas_algorithms::nas_stream_encrypt_nia2(&stream_cipher, mac);
    print_buffer("amf_n1", "Result for NIA2, mac: ", mac, 4);
    mac32 = ntohl(*((uint32_t *)mac));
    Logger::amf_n1().debug("Result for NIA2, mac32: 0x%x", mac32);
    return true;
  } break;
  }
}

//------------------------------------------------------------------------------
bool amf_n1::nas_message_cipher_protected(nas_secu_ctx *nsc, uint8_t direction,
                                          bstring input_nas,
                                          bstring &output_nas) {
  uint8_t *buf = (uint8_t *)bdata(input_nas);
  int buf_len = blength(input_nas);
  uint32_t count = 0x00000000;
  if (direction)
  {
    count = 0x00000000 | ((nsc->dl_count.overflow & 0x0000ffff) << 8) |
            ((nsc->dl_count.seq_num & 0x000000ff));
  }
  else
  {
    Logger::amf_n1().debug("nsc->ul_count.overflow %x", nsc->ul_count.overflow);
    count = 0x00000000 | ((nsc->ul_count.overflow & 0x0000ffff) << 8) |
            ((nsc->ul_count.seq_num & 0x000000ff));
  }
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = nsc->knas_enc;
  stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
  stream_cipher.count = count;
  stream_cipher.bearer = 0x01;         // 33.501 section 8.1.1
  stream_cipher.direction = direction; // "1" for downlink
  stream_cipher.message = (uint8_t *)bdata(input_nas);
  stream_cipher.blength = blength(input_nas) << 3;
  switch (nsc->nas_algs.encryption & 0x0f) {
  case EA0_5G: {
    Logger::amf_n1().debug("Cipher protected with EA0_5G");
    output_nas = blk2bstr(buf, buf_len);
    return true;
  } break;
  case EA1_128_5G: {
    Logger::amf_n1().debug("Cipher protected with EA1_128_5G");
    Logger::amf_n1().debug("stream_cipher.blength %d", stream_cipher.blength);
    Logger::amf_n1().debug("stream_cipher.message %x",
                           stream_cipher.message[0]);
    print_buffer("amf_n1", "stream_cipher.key ", stream_cipher.key, 16);
    Logger::amf_n1().debug("stream_cipher.count %x", stream_cipher.count);

    uint8_t *ciphered =
        (uint8_t *)malloc(((stream_cipher.blength + 31) / 32) * 4);

    nas_algorithms::nas_stream_encrypt_nea1(&stream_cipher, ciphered);
    output_nas = blk2bstr(ciphered, ((stream_cipher.blength + 31) / 32) * 4);
    free(ciphered);
  } break;
  case EA2_128_5G: {
    Logger::amf_n1().debug("Cipher protected with EA2_128_5G");
    nas_algorithms::nas_stream_encrypt_nea2(&stream_cipher,
                                            (uint8_t *)bdata(output_nas));
  } break;
  }
}

//------------------------------------------------------------------------------
void amf_n1::run_initial_registration_procedure() {}

//------------------------------------------------------------------------------
void amf_n1::ue_initiate_de_registration_handle(uint32_t ran_ue_ngap_id,
                                                long amf_ue_ngap_id,
                                                bstring nas) {
  Logger::amf_n1().debug("Handling UE-initiated De-registration Request");
  /*
   string guti = "1234567890";//need modify

   _5G_GUTI_t Guti;
   _5GSMobilityIdentity *ulNas = new _5GSMobilityIdentity();
   ulNas->decodefrombuffer((uint8_t*)bdata(nas)+4, blength(nas),false);
   ulNas->get5GGUTI(Guti);
   delete ulNas;
   string amf_region_id = std::to_string(Guti.amf_region_id);
   guti = Guti.mcc + Guti.mnc + std::to_string(Guti.amf_region_id) +
   std::to_string(Guti.amf_set_id) + std::to_string(Guti.amf_pointer) +
   std::to_string(Guti._5g_tmsi); std::shared_ptr<nas_context> nc;
   if(!is_guti_2_nas_context(guti))
   return;
   nc = guti_2_nas_context(guti);
   nc.get()-> is_auth_vectors_present = false;
   nc.get()-> is_current_security_available = false;
   nc.get()->security_ctx->sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
   Logger::ngap().debug("sending itti ue context release command to
   TASK_AMF_N2"); itti_ue_context_release_command * itti_msg = new
   itti_ue_context_release_command(TASK_AMF_N1, TASK_AMF_N2);
   itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
   itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
   itti_msg->cause.setChoiceOfCause(Ngap_Cause_PR_nas);
   itti_msg->cause.setValue(2);//cause nas(2)--deregister
   std::shared_ptr<itti_ue_context_release_command> i =
   std::shared_ptr<itti_ue_context_release_command>(itti_msg); int ret =
   itti_inst->send_msg(i); if (0 != ret) { Logger::ngap().error("Could not send
   ITTI message %s to task TASK_AMF_N2", i->get_msg_name());
   }
   */

  std::shared_ptr<nas_context> nc;
  if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id))
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  else {
    Logger::amf_n1().warn("No existed nas_context with amf_ue_ngap_id(0x%x)",
                          amf_ue_ngap_id);
    return;
  }

  // decode NAS msg
  DeregistrationRequest *deregReq = new DeregistrationRequest();
  deregReq->decodefrombuffer(NULL, (uint8_t *)bdata(nas), blength(nas));
  /*
   _5gs_deregistration_type_t type = {};
   deregReq->getDeregistrationType(type);
   uint8_t deregType = 0;
   deregReq->getDeregistrationType(deregType);
   Logger::amf_n1().debug("Deregistration Type %X", deregType);
   */

  // TODO: validate 5G Mobile Identity
  uint8_t mobile_id_type = 0;
  deregReq->getMobilityIdentityType(mobile_id_type);
  Logger::amf_n1().debug("5G Mobile Identity %X", mobile_id_type);
  switch (mobile_id_type) {
  case _5G_GUTI: {
    Logger::amf_n1().debug("5G Mobile Identity, GUTI %s",
                           deregReq->get_5g_guti().c_str());
  } break;
  default: {
  }
  }

  // Prepare DeregistrationAccept
  DeregistrationAccept *deregAccept = new DeregistrationAccept();
  deregAccept->setHeader(PLAIN_5GS_MSG);

  uint8_t buffer[BUFFER_SIZE_512] = {0};
  int encoded_size = deregAccept->encode2buffer(buffer, BUFFER_SIZE_512);

  print_buffer("amf_n1", "De-registration Accept message buffer", buffer,
               encoded_size);
  if (encoded_size < 1) {
    Logger::nas_mm().error("Encode De-registration Accept message error");
    return;
  }

  bstring b = blk2bstr(buffer, encoded_size);
  itti_send_dl_nas_buffer_to_task_n2(b, ran_ue_ngap_id, amf_ue_ngap_id);

  set_5gmm_state(nc, _5GMM_DEREGISTERED);
  if (nc.get()->is_stacs_available) {
    stacs.update_5gmm_state(nc.get()->imsi, "5GMM-DEREGISTERED");
  }
}

//------------------------------------------------------------------------------
void amf_n1::network_initiate_de_registration_handle(uint32_t ran_ue_ngap_id,
                                                long amf_ue_ngap_id) {
  Logger::amf_n1().debug("Handling network-initiated De-registration Request");

  std::shared_ptr<nas_context> nc;
  if (is_amf_ue_id_2_nas_context(amf_ue_ngap_id))
    nc = amf_ue_id_2_nas_context(amf_ue_ngap_id);
  else {
    Logger::amf_n1().warn("No existed nas_context with amf_ue_ngap_id(0x%x)",
                          amf_ue_ngap_id);
    return;
  }

  // encode NAS msg
  DeregistrationRequest *deregReq = new DeregistrationRequest();
  deregReq->setHeader(PLAIN_5GS_MSG,DEREGISTRATION_REQUEST_UE_TERMINATED);
  //deregReq->setngKSI(NAS_KEY_SET_IDENTIFIER_NATIVE,nc.get()->ngKsi);

  deregReq->setDeregistrationType(0x05);

  uint8_t buffer[BUFFER_SIZE_512] = {0};
  int encoded_size = deregReq->encode2buffer(buffer, BUFFER_SIZE_512);

  print_buffer("amf_n1", "De-registration Request message buffer", buffer,
               encoded_size);
  if (encoded_size < 1) {
    Logger::nas_mm().error("Encode De-registration Request message error");
    return;
  }

  bstring intProtctedNas;
  encode_nas_message_protected(
      nc.get()->security_ctx,false, INTEGRITY_PROTECTED_AND_CIPHERED,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, intProtctedNas);

  itti_send_dl_nas_buffer_to_task_n2(intProtctedNas, ran_ue_ngap_id, amf_ue_ngap_id);

  set_5gmm_state(nc, _5GMM_DEREGISTERED);
  if (nc.get()->is_stacs_available) {
    stacs.update_5gmm_state(nc.get()->imsi, "5GMM-DEREGISTERED");
  }
}


//------------------------------------------------------------------------------
void amf_n1::ul_nas_transport_handle(uint32_t ran_ue_ngap_id,
                                     long amf_ue_ngap_id, bstring nas) {
  // Decode UL_NAS_TRANSPORT message
  Logger::amf_n1().debug("Handling UL NAS Transport");
  ULNASTransport *ulNas = new ULNASTransport();
  ulNas->decodefrombuffer(NULL, (uint8_t *)bdata(nas), blength(nas));
  uint8_t payload_type = ulNas->getPayloadContainerType();
  uint8_t pdu_session_id = ulNas->getPduSessionId();
  uint8_t request_type = ulNas->getRequestType();
  SNSSAI_t snssai = {};
  ulNas->getSnssai(snssai);
  bstring dnn = bfromcstr("default");
  bstring sm_msg;
  if (ulNas->getDnn(dnn)) {
  } else {
    dnn = bfromcstr("default");
  }
  print_buffer("amf_n1", "Decoded DNN bitstring", (uint8_t *)bdata(dnn),
               blength(dnn));
  switch (payload_type) {
  case N1_SM_INFORMATION: {
    if (!ulNas->getPayloadContainer(sm_msg)) {
      Logger::amf_n1().error("Cannot decode Payload Container");
      return;
    }
    // send_itti_to_smf_services_consumer(ran_ue_ngap_id, amf_ue_ngap_id,
    // request_type, pdu_session_id, dnn, sm_msg);
    itti_smf_services_consumer *itti_msg =
        new itti_smf_services_consumer(TASK_AMF_N1, TASK_AMF_N11);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->req_type = request_type;
    itti_msg->pdu_sess_id = pdu_session_id;
    itti_msg->dnn = dnn;
    itti_msg->sm_msg = sm_msg;
    itti_msg->snssai.sST = snssai.sst;
    itti_msg->snssai.sD = std::to_string(snssai.sd);
    std::shared_ptr<itti_smf_services_consumer> i =
        std::shared_ptr<itti_smf_services_consumer>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N11",
          i->get_msg_name());
    }

  } break;
  }
}

//------------------------------------------------------------------------------
void amf_n1::ul_nas_transport_handle(uint32_t ran_ue_ngap_id,
                                     long amf_ue_ngap_id, bstring nas,
                                     plmn_t plmn) {
  // Decode UL_NAS_TRANSPORT message
  Logger::amf_n1().debug("Handling UL NAS Transport");
  ULNASTransport *ulNas = new ULNASTransport();
  ulNas->decodefrombuffer(NULL, (uint8_t *)bdata(nas), blength(nas));
  uint8_t payload_type = ulNas->getPayloadContainerType();
  uint8_t pdu_session_id = ulNas->getPduSessionId();
  uint8_t request_type = ulNas->getRequestType();
  SNSSAI_t snssai = {};
  ulNas->getSnssai(snssai);
  bstring dnn = bfromcstr("default");
  bstring sm_msg;
  if (ulNas->getDnn(dnn)) {
  } else {
    dnn = bfromcstr("default");
  }
  print_buffer("amf_n1", "Decoded DNN bitstring", (uint8_t *)bdata(dnn),
               blength(dnn));
  switch (payload_type) {
  case N1_SM_INFORMATION: {
    if (!ulNas->getPayloadContainer(sm_msg)) {
      Logger::amf_n1().error("Cannot decode Payload Container");
      return;
    }
    // send_itti_to_smf_services_consumer(ran_ue_ngap_id, amf_ue_ngap_id,
    // request_type, pdu_session_id, dnn, sm_msg);
    itti_smf_services_consumer *itti_msg =
        new itti_smf_services_consumer(TASK_AMF_N1, TASK_AMF_N11);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->req_type = request_type;
    itti_msg->pdu_sess_id = pdu_session_id;
    itti_msg->dnn = dnn;
    itti_msg->sm_msg = sm_msg;
    itti_msg->snssai.sST = snssai.sst;
    itti_msg->snssai.sD = std::to_string(snssai.sd);
    itti_msg->plmn.mnc = plmn.mnc;
    itti_msg->plmn.mcc = plmn.mcc;
    itti_msg->plmn.tac = plmn.tac;
    std::shared_ptr<itti_smf_services_consumer> i =
        std::shared_ptr<itti_smf_services_consumer>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N11",
          i->get_msg_name());
    }

  } break;
  }
}

//------------------------------------------------------------------------------
void amf_n1::send_itti_to_smf_services_consumer(uint32_t ran_ue_ngap_id,
                                                long amf_ue_ngap_id,
                                                uint8_t request_type,
                                                uint8_t pdu_session_id,
                                                bstring dnn, bstring sm_msg) {
  itti_smf_services_consumer *itti_msg =
      new itti_smf_services_consumer(TASK_AMF_N1, TASK_AMF_N11);
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
  itti_msg->req_type = request_type;
  itti_msg->pdu_sess_id = pdu_session_id;
  itti_msg->dnn = dnn;
  itti_msg->sm_msg = sm_msg;
  std::shared_ptr<itti_smf_services_consumer> i =
      std::shared_ptr<itti_smf_services_consumer>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N11",
        i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n1::dump_nas_message(uint8_t *buf, int len) {
  for (int i = 0; i < len; i++)
    Logger::amf_n1().debug("[octet%d](0x%x)", i + 1, buf[i]);
}

//------------------------------------------------------------------------------
void amf_n1::ue_authentication_simulator(uint8_t *rand, uint8_t *autn) {
  print_buffer("amf_n1", "[ue] received rand", rand, 16);
  print_buffer("amf_n1", "[ue] received autn", autn, 16);
  uint8_t opc[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                     0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  string serving_network = "5G:mnc011.mcc460.3gppnetwork.org";
  print_buffer("amf_n1", "[ue] local opc", opc, 16);
  print_buffer("amf_n1", "[ue] local key", key, 16);
  uint8_t res[8], resStar[16];
  uint8_t ck[16], ik[16], ak[6];
  Authentication_5gaka::f2345(opc, key, rand, res, ck, ik,
                              ak); // to compute XRES, CK, IK, AK
  print_buffer("amf_n1", "[ue] Result for f2345: res", res, 8);
  print_buffer("amf_n1", "[ue] Result for f2345: ck", ck, 16);
  print_buffer("amf_n1", "[ue] Result for f2345: ik", ik, 16);
  print_buffer("amf_n1", "[ue] Result for f2345: ak", ak, 6);
  annex_a_4_33501(ck, ik, res, rand, serving_network, resStar);
  print_buffer("amf_n1", "[ue] computed RES*", resStar, 16);
  uint8_t sqn[6];
  for (int i = 0; i < 6; i++)
    sqn[i] = ak[i] ^ autn[i];
  print_buffer("amf_n1", "[ue] sqn", sqn, 6);
  uint8_t amf[2];
  amf[0] = autn[6];
  amf[1] = autn[7];
  Logger::amf_n1().debug("[ue] amf 0x%x%x", amf[0], amf[1]);
  uint8_t mac_s[8];
  Authentication_5gaka::f1(opc, key, rand, sqn, amf, mac_s);
  print_buffer("amf_n1", "[ue] mas_s", mac_s, 8);
  print_buffer("amf_n1", "[ue] mas_a", autn + 8, 8);
}

//------------------------------------------------------------------------------
void amf_n1::sha256(unsigned char *message, int msg_len,
                    unsigned char *output) {
  memset(output, 0, Sha256::DIGEST_SIZE);
  ctx.init();
  ctx.update(message, msg_len);
  ctx.finalResult(output);
}

//------------------------------------------------------------------------------
void amf_n1::run_mobility_registration_update_procedure(
    std::shared_ptr<nas_context> nc) {
  // encoding REGISTRATION ACCEPT
  RegistrationAccept *regAccept = new RegistrationAccept();
  regAccept->setHeader(PLAIN_5GS_MSG);
  regAccept->set_5GS_Registration_Result(false, false, true, 0x01);
  regAccept->set5G_GUTI(amf_cfg.guami.mcc, amf_cfg.guami.mnc,
                        amf_cfg.guami.regionID, amf_cfg.guami.AmfSetID,
                        amf_cfg.guami.AmfPointer, 0x264a34c0);
  regAccept->setT3512_Value(0x5, 0x1e);

  std::vector<p_tai_t> tai_list;
  p_tai_t item0;
  item0.type = 0x00;
  nas_plmn_t plmn;
  plmn.mcc = amf_cfg.plmn_list[0].mcc;
  plmn.mnc = amf_cfg.plmn_list[0].mnc;
  item0.plmn_list.push_back(plmn);
  item0.tac_list.push_back(amf_cfg.plmn_list[0].tac);
  tai_list.push_back(item0);
  regAccept->setTaiList(tai_list);

  std::vector<struct SNSSAI_s> nssai;
  SNSSAI_t snssai;
  snssai.sst = 0;
  snssai.sd = -1;
  snssai.mHplmnSst = -1;
  snssai.mHplmnSd = -1;
  nssai.push_back(snssai);
  regAccept->setALLOWED_NSSAI(nssai);

  // std::string guti = amf_cfg.guami.mcc + amf_cfg.guami.mnc +
  // amf_cfg.guami.regionID + amf_cfg.guami.AmfSetID + amf_cfg.guami.AmfPointer
  // + "0001";
  std::string guti = "1234567890"; // TODO: need modify
  Logger::amf_n1().debug("Allocated GUTI %s", guti.c_str());

  regAccept->set_5GS_Network_Feature_Support(0x00, 0x00);
  uint8_t buffer[1024] = {0};
  int encoded_size = regAccept->encode2buffer(buffer, 1024);
  print_buffer("amf_n1", "Registration-Accept Message Buffer", buffer,
               encoded_size);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Registration-Accept message error");
    return;
  } else {
    delete regAccept;
  }
  nas_secu_ctx *secu = nc.get()->security_ctx;
  // protect nas message
  bstring protectedNas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                               protectedNas);

  string supi = "imsi-" + nc.get()->imsi;
  Logger::amf_n1().debug("Key for pdu session context SUPI (%s)", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;
   std::string record_id = "RECORD_ID=\'" + supi+ "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
    return;
  }
  Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
  psc.get()->pdu_session_context_from_json(udsf_response);
 // psc = std::shared_ptr<pdu_session_context>(psc1);
  //***************************stateless

  // if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
  //   psc = amf_n11_inst->supi_to_pdu_ctx(supi);
  // } else {
  //   Logger::amf_n1().error("Cannot get pdu_session_context with SUPI (%s)",
  //                          supi.c_str());
  // }

  uint8_t *kamf = nc.get()->kamf[secu->vector_pointer];
  uint8_t kgnb[32];
  uint32_t ulcount = secu->ul_count.seq_num | (secu->ul_count.overflow << 8);
  Authentication_5gaka::derive_kgnb(ulcount, 0x01, kamf, kgnb);
  print_buffer("amf_n1", "kamf", kamf, 32);
  // Authentication_5gaka::derive_kgnb(ulcount, 0x01, kamf, kgnb);
  bstring kgnb_bs = blk2bstr(kgnb, 32);
  itti_initial_context_setup_request *itti_msg =
      new itti_initial_context_setup_request(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
  itti_msg->kgnb = kgnb_bs;
  itti_msg->nas = protectedNas;
  itti_msg->is_sr = true; // service request indicator
  itti_msg->pdu_session_id = psc.get()->pdu_session_id;
  itti_msg->n2sm = psc.get()->n2sm;
  std::shared_ptr<itti_initial_context_setup_request> i =
      std::shared_ptr<itti_initial_context_setup_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error("Could not send ITTI message %s to task TASK_AMF_N2",
                           i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n1::run_periodic_registration_update_procedure(
    std::shared_ptr<nas_context> nc,uint16_t pdu_session_status) {
  // encoding REGISTRATION ACCEPT
  RegistrationAccept *regAccept = new RegistrationAccept();
  regAccept->setHeader(PLAIN_5GS_MSG);
  regAccept->set_5GS_Registration_Result(false, false, false, 0x01);
  regAccept->set5G_GUTI(amf_cfg.guami.mcc, amf_cfg.guami.mnc,
                        amf_cfg.guami.regionID, amf_cfg.guami.AmfSetID,
                        amf_cfg.guami.AmfPointer, 0x264a34c0);
  regAccept->setT3512_Value(0x5, 0x1e);

  std::vector<p_tai_t> tai_list;
  p_tai_t item0;
  item0.type = 0x00;
  nas_plmn_t plmn;
  plmn.mcc = amf_cfg.plmn_list[0].mcc;
  plmn.mnc = amf_cfg.plmn_list[0].mnc;
  item0.plmn_list.push_back(plmn);
  item0.tac_list.push_back(amf_cfg.plmn_list[0].tac);
  tai_list.push_back(item0);
  regAccept->setTaiList(tai_list);
//
//  std::vector<struct SNSSAI_s> nssai;
//  SNSSAI_t snssai;
//  snssai.sst = 0;
//  snssai.sd = -1;
//  snssai.mHplmnSst = -1;
//  snssai.mHplmnSd = -1;
//  nssai.push_back(snssai);
//  regAccept->setALLOWED_NSSAI(nssai);

  if (pdu_session_status == 0x0000) {
      regAccept->setPDU_session_status(0x0000);
  } else {
      regAccept->setPDU_session_status(pdu_session_status);
      Logger::amf_n1().debug("setting pdu session status 0x%02x",
                             htonl(pdu_session_status));
      // serApt->setPDU_session_status(0x2000);
  }

  regAccept->set_5GS_Network_Feature_Support(0x01, 0x00);
  uint8_t buffer[1024] = {0};
  int encoded_size = regAccept->encode2buffer(buffer, 1024);
  print_buffer("amf_n1", "Registration-Accept Message Buffer", buffer,
               encoded_size);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Registration-Accept message error");
    return;
  } else {
    delete regAccept;
  }
  nas_secu_ctx *secu = nc.get()->security_ctx;
  // protect nas message
  bstring protectedNas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                               protectedNas);

  string supi = "imsi-" + nc.get()->imsi;
  Logger::amf_n1().debug("Key for pdu session context SUPI (%s)", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;
   std::string record_id = "RECORD_ID=\'" +supi+ "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      psc.get()->pdu_session_context_from_json(udsf_response);
     // psc = std::shared_ptr<pdu_session_context>(psc1);
  }
  //***************************stateless

  // if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
  //   psc = amf_n11_inst->supi_to_pdu_ctx(supi);
  // } else {
  //   Logger::amf_n1().error("Cannot get pdu_session_context with SUPI (%s)",
  //                          supi.c_str());
  // }

  itti_dl_nas_transport *itti_msg =
      new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
  itti_msg->nas = protectedNas;
  std::shared_ptr<itti_dl_nas_transport> i =
      std::shared_ptr<itti_dl_nas_transport>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error("Could not send ITTI message %s to task TASK_AMF_N2",
                           i->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void amf_n1::run_periodic_registration_update_procedure(
    std::shared_ptr<nas_context> nc,bstring& nas_msg) {
  // decoding REGISTRATION request
  RegistrationRequest *regReq = new RegistrationRequest();
  regReq->decodefrombuffer(nullptr, (uint8_t *)bdata(nas_msg), blength(nas_msg));
  bdestroy(nas_msg); // free buffer

  // encoding REGISTRATION ACCEPT
  RegistrationAccept *regAccept = new RegistrationAccept();
  regAccept->setHeader(PLAIN_5GS_MSG);
  regAccept->set_5GS_Registration_Result(false, false, false, 0x01);
  regAccept->set5G_GUTI(amf_cfg.guami.mcc, amf_cfg.guami.mnc,
                        amf_cfg.guami.regionID, amf_cfg.guami.AmfSetID,
                        amf_cfg.guami.AmfPointer, 0x264a34c0);
  regAccept->setT3512_Value(0x5, 0x1e);

  std::vector<p_tai_t> tai_list;
  p_tai_t item0;
  item0.type = 0x00;
  nas_plmn_t plmn;
  plmn.mcc = amf_cfg.plmn_list[0].mcc;
  plmn.mnc = amf_cfg.plmn_list[0].mnc;
  item0.plmn_list.push_back(plmn);
  item0.tac_list.push_back(amf_cfg.plmn_list[0].tac);
  tai_list.push_back(item0);
  regAccept->setTaiList(tai_list);
//
//  std::vector<struct SNSSAI_s> nssai;
//  SNSSAI_t snssai;
//  snssai.sst = 0;
//  snssai.sd = -1;
//  snssai.mHplmnSst = -1;
//  snssai.mHplmnSd = -1;
//  nssai.push_back(snssai);
//  regAccept->setALLOWED_NSSAI(nssai);

  uint16_t pdu_session_status = 0xffff;
  pdu_session_status = regReq->getPduSessionStatus();
  if (pdu_session_status == 0x0000) {
      regAccept->setPDU_session_status(0x0000);
  } else {
      regAccept->setPDU_session_status(pdu_session_status);
      Logger::amf_n1().debug("setting pdu session status 0x%02x",
                             htonl(pdu_session_status));
      // serApt->setPDU_session_status(0x2000);
  }
  delete regReq;

  regAccept->set_5GS_Network_Feature_Support(0x01, 0x00);
  uint8_t buffer[1024] = {0};
  int encoded_size = regAccept->encode2buffer(buffer, 1024);
  print_buffer("amf_n1", "Registration-Accept Message Buffer", buffer,
               encoded_size);
  if (!encoded_size) {
    Logger::nas_mm().error("Encode Registration-Accept message error");
    return;
  } else {
    delete regAccept;
  }
  nas_secu_ctx *secu = nc.get()->security_ctx;
  // protect nas message
  bstring protectedNas;
  encode_nas_message_protected(secu, false, INTEGRITY_PROTECTED_AND_CIPHERED,
                               NAS_MESSAGE_DOWNLINK, buffer, encoded_size,
                               protectedNas);

  string supi = "imsi-" + nc.get()->imsi;
  Logger::amf_n1().debug("Key for pdu session context SUPI (%s)", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;
   std::string record_id = "RECORD_ID=\'" +supi+ "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      psc.get()->pdu_session_context_from_json(udsf_response);
      //psc = std::shared_ptr<pdu_session_context>(psc1);
  }
  //***************************stateless

  // if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
  //   psc = amf_n11_inst->supi_to_pdu_ctx(supi);
  // } else {
  //   Logger::amf_n1().error("Cannot get pdu_session_context with SUPI (%s)",
  //                          supi.c_str());
  // }

  itti_dl_nas_transport *itti_msg =
      new itti_dl_nas_transport(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
  itti_msg->nas = protectedNas;
  std::shared_ptr<itti_dl_nas_transport> i =
      std::shared_ptr<itti_dl_nas_transport>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n1().error("Could not send ITTI message %s to task TASK_AMF_N2",
                           i->get_msg_name());
  }
}



//------------------------------------------------------------------------------
void amf_n1::set_5gmm_state(std::shared_ptr<nas_context> nc,
                            _5gmm_state_t state) {
  Logger::amf_n1().debug("Set 5GMM state to %s",
                         _5gmm_state_e2str[state].c_str());
  std::unique_lock lock(m_nas_context);
  nc.get()->_5gmm_state = state;
  /*****************************hsx add****************************/
 nlohmann::json _5gmm_state;
  nlohmann::json udsf_response;
  nlohmann::json  udsf_nas_context;
 _5gmm_state["Content-ID"] = "_5gmm_state";
  _5gmm_state["Content-Type"] = "varchar(32)";
  _5gmm_state["content"] = to_string(nc.get()->_5gmm_state);
  udsf_nas_context["blocks"].push_back(_5gmm_state);

 std::string record_id = "RECORD_ID=\'" +to_string(nc.get()->amf_ue_ngap_id)+ "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") + record_id;
  
  udsf_nas_context["meta"] ["tags"] = {
                                      {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                      {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                      } ;    

  //amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
  /*****************************hsx add****************************/
  // TODO:
}

//------------------------------------------------------------------------------
void amf_n1::get_5gmm_state(std::shared_ptr<nas_context> nc,
                            _5gmm_state_t &state) {
  // TODO:
  state = nc.get()->_5gmm_state;
}

void amf_n1::http_update_nas_context_into_udsf(std::shared_ptr<nas_context> nc){
    string auts;
    if(nc.get()->auts)
      octet_stream_2_hex_stream((uint8_t*) bdata(nc.get()->auts), blength(nc.get()->auts), auts);
    std::string record_id = "RECORD_ID=\'" + to_string(nc.get()->amf_ue_ngap_id) + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") +record_id ;
    nlohmann::json udsf_nas_context;
    nlohmann::json udsf_response;
    udsf_nas_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({to_string(nc.get()->amf_ue_ngap_id)})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;    
    udsf_nas_context["blocks"] = nlohmann::json::array({
                                                  {{"Content-ID", "ctx_avaliability_ind"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ctx_avaliability_ind)}},
                                                  {{"Content-ID", "is_stacs_available"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->is_stacs_available)}},
                                                  {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->amf_ue_ngap_id)}},
                                                  {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(nc.get()->ran_ue_ngap_id)}},
                                                  {{"Content-ID", "nas_status"},{"Content-Type", "varchar(32)"},{"content", nc.get()->nas_status }},
                                                  {{"Content-ID", "_5gmm_state"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->_5gmm_state)}},
                                                  
                                                  {{"Content-ID", "registration_type"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->registration_type)}},
                                                  {{"Content-ID", "follow_on_req_pending_ind"},{"Content-Type", "varchar(32)"},{"content",  to_string( nc.get()->follow_on_req_pending_ind)}},
                                                  {{"Content-ID", "ngKsi"},{"Content-Type", "varchar(32)"},{"content", to_string( nc.get()->ngKsi)}},
                                                  
                                                   {{"Content-ID", "guti"},{"Content-Type", "varchar(32)"},{"content", nc.get()->guti}},
                                                  {{"Content-ID", "imsi"},{"Content-Type", "varchar(32)"},{"content", nc->imsi}},
                                                  {{"Content-ID", "mmCapability"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->mmCapability)}},
                                                  {{"Content-ID", "ueSecurityCaplen"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCaplen)}},
                                                  {{"Content-ID", "ueSecurityCapEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEnc)}},
                                                  {{"Content-ID", "ueSecurityCapInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapInt)}},
                                                  {{"Content-ID", "ueSecurityCapEEnc"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEEnc)}},
                                                  {{"Content-ID", "ueSecurityCapEInt"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->ueSecurityCapEInt)}},
                                                  {{"Content-ID", "serving_network"},{"Content-Type", "varchar(32)"},{"content",  nc.get()->serving_network}},
                                                  {{"Content-ID", "auts"},{"Content-Type", "varchar(32)"},{"content",  auts}},
                                                  
                                                  {{"Content-ID", "is_specific_procedure_for_registration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_registration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_deregistration_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_deregistration_running)}},
                                                  {{"Content-ID", "is_specific_procedure_for_eCell_inactivity_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_specific_procedure_for_eCell_inactivity_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_authentication_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_authentication_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_identification_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_identification_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_security_mode_control_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_security_mode_control_running)}},
                                                  {{"Content-ID", "is_common_procedure_for_nas_transport_running"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_common_procedure_for_nas_transport_running)}},
                                                  
                                                  {{"Content-ID", "Href"},{"Content-Type", "varchar(1024)"},{"content",  nc.get()->Href}},
                                                  {{"Content-ID", "is_current_security_available"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_current_security_available)}},
                                                  {{"Content-ID", "registration_attempt_counter"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->registration_attempt_counter)}},
                                                  
                                                  {{"Content-ID", "is_imsi_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_imsi_present)}},
                                                  {{"Content-ID", "is_5g_guti_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_5g_guti_present)}},
                                                  {{"Content-ID", "is_auth_vectors_present"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->is_auth_vectors_present)}},
                                                  {{"Content-ID", "to_be_register_by_new_suci"},{"Content-Type", "varchar(32)"},{"content",  to_string(nc.get()->to_be_register_by_new_suci)}},
                                              });
   // std::vector<nas::SNSSAI_t> requestedNssai
    nlohmann::json requestedNssai = {};
    nlohmann::json requestedNssai_m = {};
    requestedNssai["Content-ID"]="requestedNssai";
    requestedNssai["Content-Type"]="JSON";
    requestedNssai["content"] = {};
    for(int i=0;i<nc.get()->requestedNssai.size();i++)
      {
       requestedNssai_m["sst"]=to_string(nc.get()->requestedNssai[i].sst);
       requestedNssai_m["sd"]=to_string(nc.get()->requestedNssai[i].sd);
       requestedNssai_m["mHplmnSst"]=to_string(nc.get()->requestedNssai[i].mHplmnSst);
       requestedNssai_m["mHplmnSd"]=to_string(nc.get()->requestedNssai[i].mHplmnSd);
       requestedNssai["content"].push_back(requestedNssai_m);

      }
    udsf_nas_context["blocks"].push_back(requestedNssai);

//auc_vector_t _vector[MAX_5GS_AUTH_VECTORS]

    nlohmann::json _vector = {};
    nlohmann::json _vector_m = {};
    _vector["Content-ID"]="_vector";
    _vector["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _vector_m["rand_new"]=to_string(nc.get()->_vector[i].rand_new);
      _vector_m["rand"]=to_string(*(nc.get()->_vector[i].rand));
      _vector_m["xres"]=to_string(*nc.get()->_vector[i].xres);
      _vector_m["autn"]=to_string(*nc.get()->_vector[i].autn);
      _vector_m["kasme"]=to_string(*nc.get()->_vector[i].kasme);
      _vector["content"].push_back(_vector_m);

    }
    udsf_nas_context["blocks"].push_back(_vector);

//_5g_he_av
    nlohmann::json _5g_he_av = {};
    nlohmann::json _5g_he_av_m = {};
    _5g_he_av["Content-ID"]="_5g_he_av";
    _5g_he_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_he_av_m["avType"]=to_string(nc.get()->_5g_he_av[i].avType);
      _5g_he_av_m["rand"]=to_string(*(nc.get()->_5g_he_av[i].rand));
      _5g_he_av_m["xres"]=to_string(*nc.get()->_5g_he_av[i].xres);
      _5g_he_av_m["autn"]=to_string(*nc.get()->_5g_he_av[i].autn);
      _5g_he_av_m["xresStar"]=to_string(*nc.get()->_5g_he_av[i].xresStar);
      _5g_he_av_m["kausf"]=to_string(*nc.get()->_5g_he_av[i].kausf);
      _5g_he_av["content"].push_back(_5g_he_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_he_av);

//_5g_av
    nlohmann::json _5g_av = {};
    nlohmann::json _5g_av_m = {};
    _5g_av["Content-ID"]="_5g_av";
    _5g_av["Content-Type"]="JSON";
   for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++)
    {
      _5g_av_m["avType"]=to_string(nc.get()->_5g_av[i].avType);
      _5g_av_m["rand"]=to_string(*(nc.get()->_5g_av[i].rand));
      _5g_av_m["hxres"]=to_string(*nc.get()->_5g_av[i].hxres);
      _5g_av_m["autn"]=to_string(*nc.get()->_5g_av[i].autn);
      _5g_av_m["hxresStar"]=to_string(*nc.get()->_5g_av[i].hxresStar);
      _5g_av_m["kseaf"]=to_string(*nc.get()->_5g_av[i].kseaf);
      _5g_av["content"].push_back(_5g_av_m);
    }
    udsf_nas_context["blocks"].push_back(_5g_av);

//kamf
    nlohmann::json kamf = {};
    nlohmann::json kamf_m = {};
    kamf["Content-ID"]="kamf";
    kamf["Content-Type"]="JSON";
    // for(int i=0;i < MAX_5GS_AUTH_VECTORS;i++){
    //   for(int j=0;j<32;j++)
    //   {
    //     kamf_m=to_string(nc.get()->kamf[i][j]);
    //     printf("--------------kamf_m =========== %s\n",kamf_m.dump().c_str());
    //   }
    // kamf["content"].push_back(kamf_m);
    // }
    string  kamf_str;
    octet_stream_2_hex_stream(nc.get()->kamf[0],32,kamf_str);
    kamf["content"] = kamf_str;
    udsf_nas_context["blocks"].push_back(kamf);
    printf("--------------kamf =========== %s\n",kamf_str.c_str());


//security_context_t _security和nas_secu_ctx* security_ctx
  nlohmann::json _security = {};
  nlohmann::json ul_count = {};
  nlohmann::json dl_count = {};
  nlohmann::json capability = {};
  nlohmann::json selected_algorithms = {};
  _security["Content-ID"] = "_security";
  _security["Content-Type"] = "JSON";
  _security["content"]["vector_index"] = to_string(nc.get()->_security.vector_index);
  _security["content"]["knas_enc"] = to_string(*nc.get()->_security.knas_enc);
  _security["content"]["knas_int"] = to_string(*nc.get()->_security.knas_int);
  _security["content"]["activated"] = to_string(nc.get()->_security.activated);
  ul_count["spare"]=to_string(nc.get()->_security.ul_count.spare);
  ul_count["overflow"]=to_string(nc.get()->_security.ul_count.overflow);
  ul_count["seq_num"]=to_string(nc.get()->_security.ul_count.seq_num);
  _security.update(ul_count);
  dl_count["spare"]=to_string(nc.get()->_security.dl_count.spare);
  dl_count["overflow"]=to_string(nc.get()->_security.dl_count.overflow);
  dl_count["seq_num"]=to_string(nc.get()->_security.dl_count.seq_num);
  _security.update(dl_count);
  capability["eps_encryption"]=to_string(nc.get()->_security.capability.eps_encryption);
  capability["eps_integrity"]=to_string(nc.get()->_security.capability.eps_integrity);
  capability["umts_encryption"]=to_string(nc.get()->_security.capability.umts_encryption);
  capability["umts_integrity"]=to_string(nc.get()->_security.capability.umts_integrity);
  capability["gprs_encryption"]=to_string(nc.get()->_security.capability.gprs_encryption);
  capability["umts_present"]=to_string(nc.get()->_security.capability.umts_present);
  capability["gprs_present"]=to_string(nc.get()->_security.capability.gprs_present);
  _security.update(capability);
  selected_algorithms["encryption"]=to_string(nc.get()->_security.selected_algorithms.encryption);
  selected_algorithms["integrity"]=to_string(nc.get()->_security.selected_algorithms.encryption);
  _security.update(selected_algorithms);
  udsf_nas_context["blocks"].push_back(_security);

 nlohmann::json security_ctx;
  nlohmann::json ue_algorithms;
  nlohmann::json nas_algs;
  nlohmann::json vector_pointer;
  nlohmann::json sc_type;
  nlohmann::json ngksi;
  nlohmann::json knas_enc;
  nlohmann::json knas_int;
  // nlohmann::json ul_count;
  // nlohmann::json dl_count;
  security_ctx["Content-ID"] = "security_ctx";
  security_ctx["Content-Type"] = "JSON";
  security_ctx["content"] = {};

  vector_pointer["Content-ID"] = "vector_pointer";
  vector_pointer["Content-Type"] = "varchar(32)";
  vector_pointer["content"]=to_string(nc.get()->security_ctx->vector_pointer);
  security_ctx["content"].push_back(vector_pointer);

  sc_type["Content-ID"] = "sc_type";
  sc_type["Content-Type"] = "varchar(32)";
  sc_type["content"]=to_string(nc.get()->security_ctx->sc_type);
  security_ctx["content"].push_back(sc_type);

  ngksi["Content-ID"] = "ngksi";
  ngksi["Content-Type"] = "varchar(32)";
  ngksi["content"]=to_string(nc.get()->security_ctx->ngksi);
  security_ctx["content"].push_back(ngksi);

  knas_enc["Content-ID"] = "knas_enc";
  knas_enc["Content-Type"] = "varchar(32)";
  string  knas_enc_str;
  octet_stream_2_hex_stream(nc.get()->security_ctx->knas_enc,AUTH_KNAS_INT_SIZE,knas_enc_str);
  printf("-------------knas_enc string--------in amf 1------%s \n",knas_enc_str.c_str());
    for(int ly=0;ly<16;ly++)
  {
    printf("-------------knas_enc--------in amf 1------%x \n",nc.get()->security_ctx->knas_enc[ly]);
  }
  knas_enc["content"]=knas_enc_str;
  security_ctx["content"].push_back(knas_enc);

  knas_int["Content-ID"] = "knas_int";
  knas_int["Content-Type"] = "varchar(32)";
  //knas_int["content"]=to_string(*nc.get()->security_ctx->knas_int);
  string  knas_int_str;
  octet_stream_2_hex_stream(nc.get()->security_ctx->knas_int,AUTH_KNAS_INT_SIZE,knas_int_str);
  printf("-------------knas_int string--------in amf 1------%s \n",knas_int_str.c_str());
    for(int ly=0;ly<16;ly++)
  {
    printf("-------------knas_int--------in amf 1------%x \n",nc.get()->security_ctx->knas_int[ly]);
  }
  knas_int["content"]=knas_int_str;
  security_ctx["content"].push_back(knas_int);

  
  ul_count["Content-ID"] = "ul_count";
  ul_count["Content-Type"] = "JSON";
  ul_count["spare"]=to_string(nc.get()->security_ctx->ul_count.spare);
  ul_count["overflow"]=to_string(nc.get()->security_ctx->ul_count.overflow);
  ul_count["seq_num"]=to_string(nc.get()->security_ctx->ul_count.seq_num);
  security_ctx["content"].push_back(ul_count);
  dl_count["Content-ID"] = "dl_count";
  dl_count["Content-Type"] = "JSON";
  dl_count["spare"]=to_string(nc.get()->security_ctx->dl_count.spare);
  dl_count["overflow"]=to_string(nc.get()->security_ctx->dl_count.overflow);
  dl_count["seq_num"]=to_string(nc.get()->security_ctx->dl_count.seq_num);
  security_ctx["content"].push_back(dl_count);
  ue_algorithms["Content-ID"] = "ue_algorithms";
  ue_algorithms["Content-Type"] = "JSON";
  ue_algorithms["_5gs_encryption"]=to_string(nc.get()->security_ctx->ue_algorithms._5gs_encryption);
  ue_algorithms["_5gs_integrity"]=to_string(nc.get()->security_ctx->ue_algorithms._5gs_integrity);
  security_ctx["content"].push_back(ue_algorithms);
  nas_algs["Content-ID"] = "nas_algs";
  nas_algs["Content-Type"] = "JSON";
  nas_algs["encryption"]=to_string(nc.get()->security_ctx->nas_algs.encryption);
  nas_algs["integrity"]=to_string(nc.get()->security_ctx->nas_algs.integrity);
  security_ctx["content"].push_back(nas_algs);

  udsf_nas_context["blocks"].push_back(security_ctx);

  Logger::amf_n1().debug("All nas_context informations : \n %s", udsf_nas_context.dump().c_str());

  amf_n2_inst->curl_http_client_udsf(udsf_url,udsf_nas_context.dump(),"PUT",udsf_response);
}
