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

/*! \file amf_app.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_app.hpp"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "itti.hpp"

#include "DLNASTransport.hpp"
#include <nlohmann/json.hpp>
#include "ngap_app.hpp"
#include "amf_config.hpp"
#include "amf_n1.hpp"
#include "amf_n11.hpp"
#include "amf_n2.hpp"
#include "amf_statistics.hpp"
#include "ngap_app.hpp"
#include <time.h>
using namespace ngap;
using namespace nas;
//using namespace amf ;
using namespace amf_application;
using namespace config;

extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern amf_app* amf_app_inst;
extern itti_mw* itti_inst;
amf_n2* amf_n2_inst   = nullptr;
amf_n1* amf_n1_inst   = nullptr;
amf_n11* amf_n11_inst = nullptr;
extern amf_config amf_cfg;
extern statistics stacs;

void amf_app_task(void*);
uint32_t golbal_tmsi = 1;

//------------------------------------------------------------------------------
amf_app::amf_app(const amf_config& amf_cfg) {
  Logger::amf_app().startup("Creating AMF application functionality layer");
  if (itti_inst->create_task(TASK_AMF_APP, amf_app_task, nullptr)) {
    Logger::amf_app().error("Cannot create task TASK_AMF_APP");
    throw std::runtime_error("Cannot create task TASK_AMF_APP");
  }

  try {
    amf_n1_inst = new amf_n1();
    amf_n2_inst =
        new amf_n2(std::string(inet_ntoa(amf_cfg.n2.addr4)), amf_cfg.n2.port);
    amf_n11_inst = new amf_n11();
  } catch (std::exception& e) {
    Logger::amf_app().error("Cannot create AMF APP: %s", e.what());
    throw;
  }

  // Register to NRF
  if (amf_cfg.enable_nf_registration) register_to_nrf();

  timer_id_t tid = itti_inst->timer_setup(
      amf_cfg.statistics_interval, 0, TASK_AMF_APP,
      TASK_AMF_APP_PERIODIC_STATISTICS, 0);
  timer_nrf_heartbeat = itti_inst->timer_setup(1, 0, TASK_AMF_APP,
		          TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT,
			        0); 
  Logger::amf_app().startup("Started timer(%d)", tid);
}

//------------------------------------------------------------------------------
void amf_app::allRegistredModulesInit(const amf_modules& modules) {
  Logger::amf_app().info("Initiating all registered modules");
}

//------------------------------------------------------------------------------
void amf_app_task(void*) {
  const task_id_t task_id = TASK_AMF_APP;
  itti_inst->notify_task_ready(task_id);
    // nlohmann::json udsf_response2;
    // std::string record_id = "RECORD_ID=\'" + to_string(1221) + "\'";
    // std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/"+ std::string("nas_context/records/") +  record_id;
    // while(1)
    // {amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response2);}
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    timer_id_t tid;
    switch (msg->msg_type) {
      case NAS_SIG_ESTAB_REQ: {
        Logger::amf_app().debug("Received NAS_SIG_ESTAB_REQ");
        itti_nas_signalling_establishment_request* m =
            dynamic_cast<itti_nas_signalling_establishment_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;

      case N1N2_MESSAGE_TRANSFER_REQ: {
        Logger::amf_app().debug("Received N1N2_MESSAGE_TRANSFER_REQ");
        itti_n1n2_message_transfer_request* m =
            dynamic_cast<itti_n1n2_message_transfer_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;

      case TEST_SIGNALLING_PAGING: {
        Logger::amf_app().debug("Received TEST_SIGNALLING_PAGING");
        itti_test_signalling_paging* m =
            dynamic_cast<itti_test_signalling_paging*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;

      case TEST_SIGNALLING_NETWORK_INITIATED_DEREGISTRAATION: {
        Logger::amf_app().debug("Received TEST_SIGNALLING_NETWORK_INITIATED_DEREGISTRAATION");
        itti_test_signalling_network_initiated_deregistration* m =
            dynamic_cast<itti_test_signalling_network_initiated_deregistration*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;
      case N11_REGISTER_NF_INSTANCE_RESPONSE:{
        itti_n11_register_nf_instance_response* m =
            dynamic_cast<itti_n11_register_nf_instance_response*>(msg);
        amf_app_inst->handle_itti_msg(std::ref(*m));
      } break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch (to->arg1_user) {
            case TASK_AMF_APP_PERIODIC_STATISTICS:
              tid = itti_inst->timer_setup(
                  amf_cfg.statistics_interval, 0, TASK_AMF_APP,
                  TASK_AMF_APP_PERIODIC_STATISTICS, 0);
              stacs.display();
              break;
              case TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT:
                amf_app_inst->timer_nrf_heartbeat_timeout(
                    to->timer_id, to->arg2_user);
              break;
            default:
              Logger::amf_app().info(
                  "No handler for timer(%d) with arg1_user(%d) ", to->timer_id,
                  to->arg1_user);
          }
        }
        break;
      default:
        Logger::amf_app().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
long amf_app::generate_amf_ue_ngap_id() {
  long tmp = 0;
   tmp      = __sync_fetch_and_add(&amf_app_ue_ngap_id_generator, 1);
  //srand(time(NULL));
  //tmp = rand()%10000 + 1;
  //return tmp & 0xffffffffff;
  return tmp;
}

//------------------------------------------------------------------------------
bool amf_app::is_amf_ue_id_2_ue_context(const long& amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return bool{amf_ue_ngap_id2ue_ctx.count(amf_ue_ngap_id) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::amf_ue_id_2_ue_context(
    const long& amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return amf_ue_ngap_id2ue_ctx.at(amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
void amf_app::set_amf_ue_ngap_id_2_ue_context(
    const long& amf_ue_ngap_id, std::shared_ptr<ue_context> uc) {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  amf_ue_ngap_id2ue_ctx[amf_ue_ngap_id] = uc;
}

//------------------------------------------------------------------------------
bool amf_app::is_ran_amf_id_2_ue_context(const string& ue_context_key) const {
  std::shared_lock lock(m_ue_ctx_key);
  return bool{ue_ctx_key.count(ue_context_key) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::ran_amf_id_2_ue_context(
    const string& ue_context_key) const {
  std::shared_lock lock(m_ue_ctx_key);
  return ue_ctx_key.at(ue_context_key);
}

//------------------------------------------------------------------------------
void amf_app::set_ran_amf_id_2_ue_context(
    const string& ue_context_key, std::shared_ptr<ue_context> uc) {
  std::shared_lock lock(m_ue_ctx_key);
  ue_ctx_key[ue_context_key] = uc;
}

// ITTI handlers
//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_test_signalling_paging& itti_msg) {

  itti_paging* paging_msg = new itti_paging(TASK_AMF_APP, TASK_AMF_N2);

  paging_msg->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
  paging_msg->amf_ue_ngap_id = itti_msg.amf_ue_ngap_id;

  std::shared_ptr<itti_paging> i =
      std::shared_ptr<itti_paging>(paging_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_test_signalling_network_initiated_deregistration& itti_msg) {

  amf_n1_inst->network_initiate_de_registration_handle(itti_msg.ran_ue_ngap_id, itti_msg.amf_ue_ngap_id);

}



//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_n1n2_message_transfer_request& itti_msg) {
  // 1. encode DL NAS TRANSPORT message(NAS message)
  DLNASTransport* dl = new DLNASTransport();
  dl->setHeader(PLAIN_5GS_MSG);
  dl->setPayload_Container_Type(N1_SM_INFORMATION);
  dl->setPayload_Container(
      (uint8_t*) bdata(itti_msg.n1sm), blength(itti_msg.n1sm));
  dl->setPDUSessionId(itti_msg.pdu_session_id);
  uint8_t nas[1024];
  int encoded_size = dl->encode2buffer(nas, 1024);
  print_buffer("amf_app", "n1n2 transfer", nas, encoded_size);
  bstring dl_nas = blk2bstr(nas, encoded_size);

  itti_downlink_nas_transfer* dl_msg =
      new itti_downlink_nas_transfer(TASK_AMF_APP, TASK_AMF_N1);
  dl_msg->dl_nas = dl_nas;
  if (!itti_msg.is_n2sm_set) {
    dl_msg->is_n2sm_set = false;
  } else {
    dl_msg->n2sm           = itti_msg.n2sm;
    dl_msg->pdu_session_id = itti_msg.pdu_session_id;
    dl_msg->is_n2sm_set    = true;
    dl_msg->n2sm_info_type = itti_msg.n2sm_info_type;
  }
  dl_msg->amf_ue_ngap_id = itti_msg.amf_ue_ngap_id;//amf_n1_inst->supi2amfId.at(itti_msg.supi);
  dl_msg->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;//amf_n1_inst->supi2ranId.at(itti_msg.supi);
  std::shared_ptr<itti_downlink_nas_transfer> i =
      std::shared_ptr<itti_downlink_nas_transfer>(dl_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        i->get_msg_name());
  }
}
void amf_app::handle_itti_msg(itti_n11_register_nf_instance_response& r){
  Logger::amf_app().debug("Handle NF Instance Registration response");
 // nf_instance_profile = itti_msg.profile;
  // Set heartbeat timer
  Logger::amf_app().debug(
      "Set value of NRF Heartbeat timer to %d",
      r.profile.get_nf_heartBeat_timer());
  /*timer_nrf_heartbeat = itti_inst->timer_setup(
      r.profile.get_nf_heartBeat_timer(), 0, TASK_AMF_APP,
      TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT,
      0); */
}
//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_nas_signalling_establishment_request& itti_msg) {

  // get ue_context
  Logger::amf_app().debug("Try to get ue_context from UDSF");
  long amf_ue_ngap_id = 0;
  if ((amf_ue_ngap_id = itti_msg.amf_ue_ngap_id) == -1) {
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  }else{
    amf_ue_ngap_id = itti_msg.amf_ue_ngap_id;
  }
  nlohmann::json udsf_response;
  std::shared_ptr<ue_context> uc = std::shared_ptr<ue_context>(new ue_context());
  string ue_context_key = "app_ue_ranid_" + to_string(itti_msg.ran_ue_ngap_id) + "-amfid_" + to_string(amf_ue_ngap_id);
  std::string record_id = "amf_ue_ngap_id=\'" + to_string(amf_ue_ngap_id) + "\'";
  //std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + record_id ;
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + "RECORD_ID = \'" + ue_context_key + "\'";
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing ue_context with ue_context_key ...");
  }else if(udsf_response.dump().length()<8){
    Logger::amf_n2().error("No existing ue_context with ue_context_key .....");
  }else{
    Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
    uc.get()->ue_context_from_json(udsf_response);
  }
  set_ran_amf_id_2_ue_context(ue_context_key, uc);
  // Update ue_context
  uc.get()->cgi = itti_msg.cgi;
  uc.get()->tai = itti_msg.tai;
  if (itti_msg.rrc_cause != -1)
    uc.get()->rrc_estb_cause =
        (e_Ngap_RRCEstablishmentCause) itti_msg.rrc_cause;
  if (itti_msg.ueCtxReq == -1)
    uc.get()->isUeContextRequest = false;
  else
    uc.get()->isUeContextRequest = true;
  uc.get()->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
  uc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  //Update ue_context to UDSF
  Logger::amf_app().debug("Update ue_context to UDSF");
  record_id = "amf_ue_ngap_id=\'" + to_string(amf_ue_ngap_id) + "\'";
  //udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + record_id ;
  udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + "RECORD_ID = \'" + ue_context_key + "\'";
  nlohmann::json udsf_ue_context;
  nlohmann::json  cgi;
  cgi["Content-ID"] = "cgi";
  cgi["Content-Type"] = "JSON";
  cgi["content"]["mcc"] = uc.get()->cgi.mcc;
  cgi["content"]["mnc"] = uc.get()->cgi.mnc; 
  cgi["content"]["nrCellID"] = to_string(uc.get()->cgi.nrCellID);
  nlohmann::json  tai;
  tai["Content-ID"] = "tai";
  tai["Content-Type"] = "JSON";
  tai["content"]["mcc"] = uc.get()->tai.mcc;
  tai["content"]["mnc"] = uc.get()->tai.mnc; 
  tai["content"]["tac"] = to_string(uc.get()->tai.tac);
  udsf_ue_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({ue_context_key})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;
  udsf_ue_context["blocks"] = nlohmann::json::array({
                                               {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->ran_ue_ngap_id)}},
                                               {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->amf_ue_ngap_id)}},
                                               {{"Content-ID", "rrc_estb_cause"},{"Content-Type", "varchar(32)"},{"content",to_string(uc.get()->rrc_estb_cause)}},
                                               {{"Content-ID", "isUeContextRequest"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->isUeContextRequest)}}
                                          });              
  udsf_ue_context["blocks"].push_back(cgi);    
  udsf_ue_context["blocks"].push_back(tai);  
  std::string json_part = udsf_ue_context.dump();
  amf_n2_inst->curl_http_client_udsf(udsf_url,json_part,"PUT",udsf_response);
  Logger::amf_app().debug("Update ue_context to UDSF finished");
  // send to TASK_AMF_N1
  std::string guti;
  bool is_guti_valid = false;
  if (itti_msg.is_5g_s_tmsi_present) {
    guti = itti_msg.tai.mcc + itti_msg.tai.mnc + amf_cfg.guami.regionID +
           itti_msg._5g_s_tmsi;
    is_guti_valid = true;
    Logger::amf_app().debug("Receiving GUTI %s", guti.c_str());
  }
  itti_uplink_nas_data_ind* itti_n1_msg =
      new itti_uplink_nas_data_ind(TASK_AMF_APP, TASK_AMF_N1);
  itti_n1_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
  itti_n1_msg->ran_ue_ngap_id              = itti_msg.ran_ue_ngap_id;
  itti_n1_msg->is_nas_signalling_estab_req = true;
  itti_n1_msg->nas_msg                     = itti_msg.nas_buf;
  itti_n1_msg->mcc                         = itti_msg.tai.mcc;
  itti_n1_msg->mnc                         = itti_msg.tai.mnc;
  itti_n1_msg->is_guti_valid               = is_guti_valid;
  if (is_guti_valid) {
    itti_n1_msg->guti = guti;
  }
  std::shared_ptr<itti_uplink_nas_data_ind> i =
      std::shared_ptr<itti_uplink_nas_data_ind>(itti_n1_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        i->get_msg_name());
  }

#if 0

  // 1. generate amf_ue_ngap_id
  // 2. establish ue_context associated with amf_ue_ngap_id
  // 3. store ue-reated core information
  // 4. send nas-pdu to task_amf_n1
  long amf_ue_ngap_id = 0;
  std::shared_ptr<ue_context> uc;
  // check ue context with 5g-s-tmsi

  if ((amf_ue_ngap_id = itti_msg.amf_ue_ngap_id) == -1) {
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  }
  string ue_context_key = "app_ue_ranid_" + to_string(itti_msg.ran_ue_ngap_id) +
                          "-amfid_" + to_string(amf_ue_ngap_id);
  // if(!is_amf_ue_id_2_ue_context(amf_ue_ngap_id)){
  if (!is_ran_amf_id_2_ue_context(ue_context_key)) {
    Logger::amf_app().debug(
        "No existing UE Context, Create a new one with ran_amf_id %s",
        ue_context_key.c_str());
    uc = std::shared_ptr<ue_context>(new ue_context());
    // set_amf_ue_ngap_id_2_ue_context(amf_ue_ngap_id, uc);
    set_ran_amf_id_2_ue_context(ue_context_key, uc);
  }
  if (uc.get() == nullptr) {
    Logger::amf_app().error(
        "Failed to create ue_context with ran_amf_id %s",
        ue_context_key.c_str());
  } else {
    uc.get()->cgi = itti_msg.cgi;
    uc.get()->tai = itti_msg.tai;
    if (itti_msg.rrc_cause != -1)
      uc.get()->rrc_estb_cause =
          (e_Ngap_RRCEstablishmentCause) itti_msg.rrc_cause;
    if (itti_msg.ueCtxReq == -1)
      uc.get()->isUeContextRequest = false;
    else
      uc.get()->isUeContextRequest = true;
    uc.get()->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
    uc.get()->amf_ue_ngap_id = amf_ue_ngap_id;

    std::string guti;
    bool is_guti_valid = false;
    if (itti_msg.is_5g_s_tmsi_present) {
      guti = itti_msg.tai.mcc + itti_msg.tai.mnc + amf_cfg.guami.regionID +
             itti_msg._5g_s_tmsi;
      is_guti_valid = true;
      Logger::amf_app().debug("Receiving GUTI %s", guti.c_str());
    }

    itti_uplink_nas_data_ind* itti_n1_msg =
        new itti_uplink_nas_data_ind(TASK_AMF_APP, TASK_AMF_N1);
    itti_n1_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
    itti_n1_msg->ran_ue_ngap_id              = itti_msg.ran_ue_ngap_id;
    itti_n1_msg->is_nas_signalling_estab_req = true;
    itti_n1_msg->nas_msg                     = itti_msg.nas_buf;
    itti_n1_msg->mcc                         = itti_msg.tai.mcc;
    itti_n1_msg->mnc                         = itti_msg.tai.mnc;
    itti_n1_msg->is_guti_valid               = is_guti_valid;
    if (is_guti_valid) {
      itti_n1_msg->guti = guti;
    }
    
  /**********************   hxs add *******************************/

  std::string record_id = "amf_ue_ngap_id=\'" + to_string(amf_ue_ngap_id) + "\'";
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + record_id ;
  nlohmann::json udsf_ue_context;
  nlohmann::json  cgi;
  cgi["Content-ID"] = "cgi";
  cgi["Content-Type"] = "JSON";
  cgi["content"]["mcc"] = uc.get()->cgi.mcc;
  cgi["content"]["mnc"] = uc.get()->cgi.mnc; 
  cgi["content"]["nrCellID"] = to_string(uc.get()->cgi.nrCellID);
  nlohmann::json  tai;
  tai["Content-ID"] = "tai";
  tai["Content-Type"] = "JSON";
  tai["content"]["mcc"] = uc.get()->tai.mcc;
  tai["content"]["mnc"] = uc.get()->tai.mnc; 
  tai["content"]["tac"] = to_string(uc.get()->tai.tac);
  udsf_ue_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({ue_context_key})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;
  udsf_ue_context["blocks"] = nlohmann::json::array({
                                               {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->ran_ue_ngap_id)}},
                                               {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->amf_ue_ngap_id)}},
                                               {{"Content-ID", "rrc_estb_cause"},{"Content-Type", "varchar(32)"},{"content",to_string(uc.get()->rrc_estb_cause)}},
                                               {{"Content-ID", "isUeContextRequest"},{"Content-Type", "varchar(32)"},{"content", to_string(uc.get()->isUeContextRequest)}}
                                          });              
  udsf_ue_context["blocks"].push_back(cgi);    
  udsf_ue_context["blocks"].push_back(tai);  
  std::string json_part = udsf_ue_context.dump();
  //amf_n2_inst->curl_http_client_udsf(udsf_url,json_part,"PUT",udsf_ue_context);
  nlohmann::json udsf_response;
  amf_n2_inst->curl_http_client_udsf(udsf_url,json_part,"PUT",udsf_response);
  // udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("gnb_context/records/") + std::to_string(itti_msg.assoc_id) ;
  // if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
  //   Logger::amf_n2().error("No existing gNG context with assoc_id (%d)", itti_msg.assoc_id);
  //   return;
  // }
  // Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());\
  // gnb_context *gc1 = new gnb_context();
  // std::shared_ptr<gnb_context> gc2;
  // gc1->gnb_context_from_json(udsf_response);
  // gc2 = std::shared_ptr<gnb_context>(gc1);

  // udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + std::to_string(uc.get()->ran_ue_ngap_id) ;
  // if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
  //   Logger::amf_n2().error("No existing gNG context with assoc_id (%d)", uc.get()->ran_ue_ngap_id);
  //   return;
  // }
  // Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());


  // ue_context *uc1 = new ue_context();
  // std::shared_ptr<ue_context> uc2;
  // uc1->ue_context_from_json(udsf_response);
  // uc2 = std::shared_ptr<ue_context>(uc1);

//printf("ran_ue_ngap_id-%d\n",uc2.get()->ran_ue_ngap_id);
//printf("ng_ue_state-%d\n",unc2.get()->ng_ue_state);




  /**********************   hxs add *******************************/  
    std::shared_ptr<itti_uplink_nas_data_ind> i =
        std::shared_ptr<itti_uplink_nas_data_ind>(itti_n1_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N1",
          i->get_msg_name());
    }
  }
#endif
}

// SMF Client response handlers
//------------------------------------------------------------------------------
void amf_app::handle_post_sm_context_response_error_400() {
  Logger::amf_app().error("Post SM context response error 400");
}

bool amf_app::generate_5g_guti(
    uint32_t ranid, long amfid, string& mcc, string& mnc, uint32_t& tmsi) {
  string ue_context_key =
      "app_ue_ranid_" + to_string(ranid) + "-amfid_" + to_string(amfid);
  // if (!is_ran_amf_id_2_ue_context(ue_context_key)) {
  //   Logger::amf_app().error(
  //       "No UE context for ran_amf_id %s, exit", ue_context_key.c_str());
  //   return false;
  // }
  // std::shared_ptr<ue_context> uc;
  // uc   = ran_amf_id_2_ue_context(ue_context_key);
   std::shared_ptr<ue_context> uc = std::shared_ptr<ue_context>(new ue_context());
  nlohmann::json udsf_response;
  std::string record_id = "RECORD_ID=\'" + to_string(amfid) + "\'";
  //std::string record_id = "RECORD_ID=\'" + ue_context_key + "\'";
  //std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + record_id;
  std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("ue_context/records/") + "RECORD_ID = \'" + ue_context_key + "\'";
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing gNG context with assoc_id");
    return false;
  }
  Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
  uc.get()->ue_context_from_json(udsf_response);



  mcc  = uc.get()->tai.mcc;
  mnc  = uc.get()->tai.mnc;
  tmsi = golbal_tmsi;
  golbal_tmsi++;
  return true;
}

void amf_app::register_to_nrf() {
  // create a NF profile to this instance
  generate_amf_profile();
  // send request to N11 to send NF registration to NRF
  trigger_nf_registration_request();
}
void amf_app::trigger_nf_registration_request() {
  Logger::amf_app().debug(
      "Send ITTI msg to N11 task to trigger the registration request to NRF");

  std::shared_ptr<itti_n11_register_nf_instance_request> itti_msg =
      std::make_shared<itti_n11_register_nf_instance_request>(
          TASK_AMF_APP, TASK_AMF_N11);
  itti_msg->profile = nf_instance_profile;

  amf_n11_inst->register_nf_instance(itti_msg);
  /*

  int ret           = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N11",
        itti_msg->get_msg_name());
  }
  */
}
//------------------------------------------------------------------------------
void amf_app::generate_uuid() {
  amf_instance_id = to_string(boost::uuids::random_generator()());
}

//---------------------------------------------------------------------------------------------
void amf_app::generate_amf_profile() {
  // generate UUID
  generate_uuid();
  nf_instance_profile.set_nf_instance_id(amf_instance_id);
  nf_instance_profile.set_nf_instance_name("OAI-AMF");
  nf_instance_profile.set_nf_type("AMF");
  nf_instance_profile.set_nf_status("REGISTERED");
  nf_instance_profile.set_nf_heartBeat_timer(1);
  nf_instance_profile.set_nf_priority(1);
  nf_instance_profile.set_nf_capacity(100);
  nf_instance_profile.add_nf_ipv4_addresses(amf_cfg.n11.addr4);

  // NF services
  nf_service_t nf_service        = {};
  nf_service.service_instance_id = "namf_communication";
  nf_service.service_name        = "namf_communication";
  nf_service_version_t version   = {};
  version.api_version_in_uri     = "v1";
  version.api_full_version       = "1.0.0";  // TODO: to be updated
  nf_service.versions.push_back(version);
  nf_service.scheme            = "http";
  nf_service.nf_service_status = "REGISTERED";
  // IP Endpoint
  ip_endpoint_t endpoint = {};
  endpoint.ipv4_address  = amf_cfg.n11.addr4;
  endpoint.transport     = "TCP";
  endpoint.port          = amf_cfg.n11.port;
  nf_service.ip_endpoints.push_back(endpoint);

  nf_instance_profile.add_nf_service(nf_service);

  // TODO: custom info
  // AMF info
  amf_info_t info    = {};
  info.amf_region_id = amf_cfg.guami.regionID;
  info.amf_set_id    = amf_cfg.guami.AmfSetID;
  for (auto g : amf_cfg.guami_list) {
    _3gpp_23003::guami_5g_t guami = {};
    guami.amf_id =
        g.regionID + ":" + g.AmfSetID + ":" + g.AmfPointer;  // TODO verify??
    guami.plmn.mcc = g.mcc;
    guami.plmn.mnc = g.mnc;
    info.guami_list.push_back(guami);
  }

  nf_instance_profile.set_amf_info(info);

  // Display the profile
  nf_instance_profile.display();
}

//------------------------------------------------------------------------------
void amf_app::timer_nrf_heartbeat_timeout(
    timer_id_t timer_id, uint64_t arg2_user) {
  Logger::amf_app().debug("Send ITTI msg to N11 task to trigger NRF Heartbeat");

  std::shared_ptr<itti_n11_update_nf_instance_request> itti_msg =
      std::make_shared<itti_n11_update_nf_instance_request>(
          TASK_AMF_APP, TASK_AMF_N11);
  oai::amf::model::PatchItem patch_item = {};
  //{"op":"replace","path":"/nfStatus", "value": "REGISTERED"}
  patch_item.setOp("replace");
  patch_item.setPath("/nfStatus");
  patch_item.setValue("REGISTERED");
  itti_msg->patch_items.push_back(patch_item);
  itti_msg->amf_instance_id = amf_instance_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N11",
        itti_msg->get_msg_name());
  } else {
    Logger::amf_app().debug(
        "Set a timer to the next Heart-beat (%d)",
        nf_instance_profile.get_nf_heartBeat_timer());
    timer_nrf_heartbeat = itti_inst->timer_setup(
        nf_instance_profile.get_nf_heartBeat_timer(), 0, TASK_AMF_APP,
        TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT,
        0);  // TODO arg2_user
  }
}
