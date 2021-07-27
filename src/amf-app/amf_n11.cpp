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

/*! \file amf_n11.cpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_n11.hpp"
#include "amf_n2.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "amf.hpp"
#include "amf_app.hpp"
#include "3gpp_ts24501.hpp"
#include "amf_config.hpp"
#include "amf_n1.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "nas_context.hpp"
// For smf_client
#include "ApiClient.h"
#include "ApiConfiguration.h"
#include "SMContextsCollectionApi.h"
#include "SmContextCreateData.h"
#include "mime_parser.hpp"
#include "ue_context.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace oai::smf::model;
using namespace oai::smf::api;
using namespace web;
using namespace web::http;
// Common features like URIs.
using namespace web::http::client;
using namespace config;
using namespace amf_application;
extern itti_mw* itti_inst;
extern amf_config amf_cfg;
extern amf_n11* amf_n11_inst;
extern amf_n1* amf_n1_inst;
extern amf_n2* amf_n2_inst;
extern amf_app* amf_app_inst;
extern statistics stacs;

extern void msg_str_2_msg_hex(std::string msg, bstring& b);
extern void convert_string_2_hex(std::string& input, std::string& output);
extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern bool multipart_parser(
    std::string input, std::string& jsonData, std::string& n1sm,
    std::string& n2sm);
extern unsigned char* format_string_as_hex(std::string str);
extern char* bstring2charString(bstring b);

//------------------------------------------------------------------------------
std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
void octet_stream_2_hex_stream(uint8_t* buf, int len, std::string& out) {
  out       = "";
  char* tmp = (char*) calloc(1, 2 * len * sizeof(uint8_t) + 1);
  for (int i = 0; i < len; i++) {
    sprintf(tmp + 2 * i, "%02x", buf[i]);
  }
  tmp[2 * len] = '\0';
  out          = tmp;
  printf("n1sm buffer: %s\n", out.c_str());
}

/****************************************************/
/**  used to run NF(s) consumer, like smf_client ****/
/***************************************************/

void amf_n11_task(void*);
//------------------------------------------------------------------------------
void amf_n11_task(void*) {
  const task_id_t task_id = TASK_AMF_N11;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case SMF_SERVICES_CONSUMER: {
        Logger::amf_n11().info("Running SMF_SERVICES_CONSUMER");
        itti_smf_services_consumer* m =
            dynamic_cast<itti_smf_services_consumer*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      case NSMF_PDU_SESSION_UPDATE_SM_CTX: {
        Logger::amf_n11().info(
            "Receive Nsmf_PDUSessionUpdateSMContext, handling ...");
        itti_nsmf_pdusession_update_sm_context* m =
            dynamic_cast<itti_nsmf_pdusession_update_sm_context*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      case PDU_SESS_RES_SET_RESP: {
        Logger::amf_n11().info(
            "Receive PDU Session Resource Setup Response, handling ...");
        itti_pdu_session_resource_setup_response* m =
            dynamic_cast<itti_pdu_session_resource_setup_response*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      case N11_REGISTER_NF_INSTANCE_REQUEST: {
        Logger::amf_n11().info(
            "Receive Register NF Instance Request, handling ...");
        itti_n11_register_nf_instance_request* m =
            dynamic_cast<itti_n11_register_nf_instance_request*>(msg);
        // TODO: Handle ITTI
      } break;
       case N11_UPDATE_NF_INSTANCE_REQUEST:{
        Logger::amf_n11().info(
            "Receive Update NF Instance Request, handling ...");
        amf_n11_inst->update_nf_instance(
            std::static_pointer_cast<itti_n11_update_nf_instance_request>(
                shared_msg));
      } break;
      default: {
        Logger::amf_n11().info(
            "Receive unknown message type %d", msg->msg_type);
      }
    }
  } while (true);
}

//------------------------------------------------------------------------------
amf_n11::amf_n11() {
  if (itti_inst->create_task(TASK_AMF_N11, amf_n11_task, nullptr)) {
    Logger::amf_n11().error("Cannot create task TASK_AMF_N1");
    throw std::runtime_error("Cannot create task TASK_AMF_N1");
  }
  Logger::amf_n11().startup("Started");
  Logger::amf_n11().debug("Construct amf_n1 successfully");
}

//------------------------------------------------------------------------------
amf_n11::~amf_n11() {}

//------------------------------------------------------------------------------

void amf_n11::handle_itti_message(
    itti_pdu_session_resource_setup_response& itti_msg) {}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(
    itti_nsmf_pdusession_update_sm_context& itti_msg) {



  //std::string supi = pduid2supi.at(itti_msg.pdu_session_id);
  // Logger::amf_n11().debug(
  //     "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
  //     "ID %d)",
  //     supi.c_str(), itti_msg.pdu_session_id);
  //std::shared_ptr<pdu_session_context> psc;
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //***************************stateless
  nlohmann::json udsf_response;
  std::shared_ptr<nas_context> nc = std::shared_ptr<nas_context>(new nas_context());
  std::string record_id_nas = "amf_ue_ngap_id=\'" + to_string(itti_msg.amf_ue_ngap_id) + "\'";
  std::string udsf_url_nas = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("nas_context/records/") +record_id_nas ;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url_nas,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
    return ;
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      nc.get()->nas_context_from_json(udsf_response);
  }
  std::string supi ="imsi-" + nc.get()->imsi ;
  Logger::amf_n11().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d)",
      supi.c_str(), itti_msg.pdu_session_id);

  std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
 if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
return ;
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      psc.get()->pdu_session_context_from_json(udsf_response);
  }

  // std::shared_ptr<nas_context> nc = std::shared_ptr<nas_context>(new nas_context());
  // if (amf_n1_inst->is_amf_ue_id_2_nas_context_in_udsf(psc.get()->amf_ue_ngap_id))
  //     nc = amf_n1_inst->amf_ue_id_2_nas_context_in_udsf(psc.get()->amf_ue_ngap_id);

  
   
  //***************************stateless

  // if (is_supi_to_pdu_ctx(supi)) {
   //  psc = supi_to_pdu_ctx(supi);
  // } else {
  //   Logger::amf_n11().error(
  //       "Could not find psu_session_context with SUPI %s, Failed",
  //       supi.c_str());
  //   return;
  // }

  /*string ue_context_key = "app_ue_ranid_" + to_string(itti_msg.ran_ue_ngap_id) +
	                            ":amfid_" + to_string(itti_msg.amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc;
  uc = amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
  std::string supi_2;
  if (uc.get() != nullptr) {
    supi_2 = uc->supi;
  }
  Logger::amf_n11().debug(
		        "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
			      "ID %d)",
			            supi_2.c_str(), itti_msg.pdu_session_id);
  std::shared_ptr<pdu_session_context> psc_2 = {};
    if (!uc.get()->find_pdu_session_context(itti_msg.pdu_session_id, psc_2)) {
	 Logger::amf_n11().error(
		"Could not find psu_session_context with SUPI %s, Failed",
			supi_2.c_str());
        return;
   }*/
  std::string smf_addr;
  if (!psc.get()->smf_available) {
    /*if (!smf_selection_from_configuration(smf_addr)) {
      Logger::amf_n11().error("No SMF candidate is available");
     return;
    }*/
    Logger::amf_n11().error("No SMF is available for this PDU session");
  } else {
    //smf_selection_from_context(smf_addr);
    smf_addr        = psc->smf_addr;
  }
 
  smf_addr        = psc->smf_addr;
  std::string smf_ip_addr, remote_uri;
  std::shared_ptr<pdu_session_context> context = std::shared_ptr<pdu_session_context>(new pdu_session_context());
   context.get()->pdu_session_context_from_json(udsf_response);
  //context = supi_to_pdu_ctx(supi);
  // remove http port from the URI if existed
  std::size_t found_port = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port - 1);
  else
    smf_ip_addr = smf_addr;

  std::size_t found = psc.get()->smf_context_location.find(smf_ip_addr);
  if (found != std::string::npos)
    remote_uri = context.get()->smf_context_location + "/modify";
  else
    remote_uri = smf_addr + context.get()->smf_context_location + "/modify";

  Logger::amf_n11().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_update_request = {};
  // if (itti_msg.is_n2sm_set){
  pdu_session_update_request["n2SmInfoType"]          = itti_msg.n2sm_info_type;
  pdu_session_update_request["n2SmInfo"]["contentId"] = "n2msg";
  std::string json_part = pdu_session_update_request.dump();
  std::string n2SmMsg;
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(itti_msg.n2sm), blength(itti_msg.n2sm), n2SmMsg);
  curl_http_client(
      remote_uri, json_part, "", n2SmMsg, supi, itti_msg.pdu_session_id);
}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(itti_smf_services_consumer& smf) {
  Logger::amf_n11().debug("Handle ITTI_SMF_SERVICES_CONSUMER");
  std::shared_ptr<nas_context> nc;
  nc               = amf_n1_inst->amf_ue_id_2_nas_context(smf.amf_ue_ngap_id);
  std::string supi = "imsi-" + nc.get()->imsi;

  //std::shared_ptr<pdu_session_context> psc;
  std::shared_ptr<pdu_session_context> psc;
    //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;



  std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response) ){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
    psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  }
  else if(udsf_response.dump().length() < 8)
  {
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id----length = %d ",udsf_response.dump().length());
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id 2222");
     psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
     psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
      psc.get()->pdu_session_context_from_json(udsf_response);
      if(psc.get()->isn2sm_avaliable == false)
      {
	 psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
      }
      //psc = std::shared_ptr<pdu_session_context>(psc1);
  }
 
  //***************************stateless

  // if (is_supi_to_pdu_ctx(supi)) {
  //   psc = supi_to_pdu_ctx(supi);
  // } else {
  //   psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
  //   set_supi_to_pdu_ctx(supi, psc);
  // }

  //pduid2supi[smf.pdu_sess_id] = supi;
  psc.get()->amf_ue_ngap_id   = nc.get()->amf_ue_ngap_id;
  psc.get()->ran_ue_ngap_id   = nc.get()->ran_ue_ngap_id;
  psc.get()->req_type         = smf.req_type;
  psc.get()->pdu_session_id   = smf.pdu_sess_id;
  psc.get()->snssai.sST       = smf.snssai.sST;
  psc.get()->snssai.sD        = smf.snssai.sD;
  psc.get()->plmn.mcc         = smf.plmn.mcc;
  psc.get()->plmn.mnc         = smf.plmn.mnc;
  psc.get()->plmn.tac         = smf.plmn.tac;

  // parse binary dnn and store
  std::string dnn = "default";
  if ((smf.dnn != nullptr) && (blength(smf.dnn) > 0)) {
    char* tmp = bstring2charString(smf.dnn);
    dnn       = tmp;
    free_wrapper((void**) &tmp);
  }

  Logger::amf_n11().debug("Requested DNN: %s", dnn.c_str());
  psc.get()->dnn = dnn;

  std::string smf_addr;
  std::string smf_api_version;
  if (!psc.get()->smf_available) {
    if (amf_cfg.enable_smf_selection) {
      // use NRF to find suitable SMF based on snssai, plmn and dnn
      if (!discover_smf(
              smf_addr, smf_api_version, psc.get()->snssai, psc.get()->plmn,
              psc.get()->dnn)) {
        Logger::amf_n11().error("SMF Selection, no SMF candidate is available");
      return;
    }
   } else if (!smf_selection_from_configuration(smf_addr)) {
      Logger::amf_n11().error(
          "No SMF candidate is available (from configuration file)");
      return;
    }
    // store smf info to be used with this PDU session
    psc.get()->smf_available = true;
    psc->smf_addr            = smf_addr;
    psc->smf_api_version     = smf_api_version;
  } else {
    smf_addr        = psc->smf_addr;
    smf_api_version = psc->smf_api_version;
  }

  switch (smf.req_type & 0x07) {
    case PDU_SESSION_INITIAL_REQUEST: {
      // get pti
      uint8_t* sm_msg = (uint8_t*) bdata(smf.sm_msg);
      uint8_t pti     = sm_msg[2];
      Logger::amf_n11().debug(
          "Decoded PTI for PDUSessionEstablishmentRequest(0x%x)", pti);
      psc.get()->isn2sm_avaliable = false;
      handle_pdu_session_initial_request(
          supi, psc, smf_addr, smf_api_version, smf.sm_msg, dnn);
      /*
      if (psc.get()->isn1sm_avaliable && psc.get()->isn2sm_avaliable) {
        itti_n1n2_message_transfer_request* itti_msg =
            new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
        itti_msg->supi = supi;

        uint8_t accept_len = blength(psc.get()->n1sm);
        uint8_t* accept    = (uint8_t*) calloc(1, accept_len);
        memcpy(accept, (uint8_t*) bdata(psc.get()->n1sm), accept_len);
        accept[2]      = pti;
        itti_msg->n1sm = blk2bstr(accept, accept_len);
        free(accept);
        itti_msg->is_n1sm_set    = true;
        itti_msg->n2sm           = psc.get()->n2sm;
        itti_msg->is_n2sm_set    = true;
        itti_msg->pdu_session_id = psc.get()->pdu_session_id;
        std::shared_ptr<itti_n1n2_message_transfer_request> i =
            std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_n11().error(
              "Could not send ITTI message %s to task TASK_AMF_APP",
              i->get_msg_name());
        }
      } else {
        psc.get()->isn2sm_avaliable = false;
        handle_pdu_session_initial_request(
            supi, psc, smf_addr, smf.sm_msg, dnn);
      }
      */
    } break;
    case EXISTING_PDU_SESSION: {
      // TODO:
    } break;
    case PDU_SESSION_TYPE_MODIFICATION_REQUEST: {
      // TODO:
    } break;
    default: {
      // send Nsmf_PDUSession_UpdateSM_Context to SMF e.g., for PDU Session
      // release request
      send_pdu_session_update_sm_context_request(
          supi, psc, smf_addr, smf.sm_msg, dnn);
    }
  }
}

//------------------------------------------------------------------------------
void amf_n11::send_pdu_session_update_sm_context_request(
    std::string supi, std::shared_ptr<pdu_session_context> psc,
    std::string smf_addr, bstring sm_msg, std::string dnn) {
  Logger::amf_n11().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d)",
      supi.c_str(), psc.get()->pdu_session_id);

  std::string smf_ip_addr, remote_uri;
  // remove http port from the URI if existed
  std::size_t found_port = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port - 1);
  else
    smf_ip_addr = smf_addr;

  std::size_t found = psc.get()->smf_context_location.find(smf_ip_addr);
  if (found != std::string::npos)
    remote_uri = psc.get()->smf_context_location + "/modify";
  else
    remote_uri = smf_addr + psc.get()->smf_context_location + "/modify";

  Logger::amf_n11().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_update_request          = {};
  pdu_session_update_request["n1SmMsg"]["contentId"] = "n1SmMsg";
  std::string json_part = pdu_session_update_request.dump();

  std::string n1SmMsg;
  octet_stream_2_hex_stream((uint8_t*) bdata(sm_msg), blength(sm_msg), n1SmMsg);
  curl_http_client(
      remote_uri, json_part, n1SmMsg, "", supi, psc.get()->pdu_session_id);
}

//------------------------------------------------------------------------------
void amf_n11::handle_pdu_session_initial_request(
    std::string supi, std::shared_ptr<pdu_session_context> psc,
    std::string smf_addr, std::string smf_api_version, bstring sm_msg,
    std::string dnn) {
  Logger::amf_n11().debug(
      "Handle PDU Session Establishment Request (SUPI %s, PDU Session ID %d)",
      supi.c_str(), psc.get()->pdu_session_id);

  /***************************hxs add**************/
  //send udsf to storage us_ngap_context recordid=ran+ue_ngap_id

   std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_put_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
  nlohmann::json udsf_pdu_session_context;
 // nlohmann::json udsf_response;
  udsf_pdu_session_context["meta"] ["tags"] = {
                                       {"RECORD_ID",nlohmann::json::array({supi})},
                                       {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                       } ;
  udsf_pdu_session_context["blocks"] = nlohmann::json::array({
                                               {{"Content-ID", "ran_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(psc.get()->ran_ue_ngap_id)}},
                                               {{"Content-ID", "amf_ue_ngap_id"},{"Content-Type", "varchar(32)"},{"content", to_string(psc.get()->amf_ue_ngap_id)}},
                                               {{"Content-ID", "req_type"},{"Content-Type", "varchar(32)"},{"content",to_string(psc.get()->req_type)}},
                                               {{"Content-ID", "pdu_session_id"},{"Content-Type", "varchar(32)"},{"content", to_string(psc.get()->pdu_session_id)}},
                                               {{"Content-ID", "supi"},{"Content-Type", "varchar(32)"},{"content", supi}},
                                               {{"Content-ID", "dnn"},{"Content-Type", "varchar(32)"},{"content", psc.get()->dnn}},
                                               {{"Content-ID", "n2sm"},{"Content-Type", "varchar(1024)"},{"content", " "}},
                                               {{"Content-ID", "isn2sm_avaliable"},{"Content-Type", "varchar(32)"},{"content"," "}},
                                               {{"Content-ID", "n1sm"},{"Content-Type", "varchar(1024)"},{"content", " "}},
                                               {{"Content-ID", "isn1sm_avaliable"},{"Content-Type", "varchar(32)"},{"content", " "}},
                                               {{"Content-ID", "remote_smf_addr"},{"Content-Type", "varchar(32)"},{"content", " "}},
                                               {{"Content-ID", "smf_available"},{"Content-Type", "varchar(32)"},{"content", ""}},
                                               {{"Content-ID", "location"},{"Content-Type", "varchar(32)"},{"content",""}},
                                               {{"Content-ID", "smf_context_location"},{"Content-Type", "varchar(128)"},{"content","" }}   
                                          });
  nlohmann::json snssai = {};
  nlohmann::json plmn = {};
  nlohmann::json udsf_response;
  snssai["Content-ID"] ="snssai";
  snssai["Content-Type"] = "JSON" ;
  snssai["content"]["sST"] = to_string(psc.get()->snssai.sST) ;
  snssai["content"]["sD"] = psc.get()->snssai.sD ;

  plmn["Content-ID"] ="plmn";
  plmn["Content-Type"] = "JSON" ;
  plmn["content"]["mcc"] = psc.get()->plmn.mcc ;
  plmn["content"]["mnc"] = psc.get()->plmn.mnc ;
  plmn["content"]["tac"] = to_string(psc.get()->plmn.tac) ;

  udsf_pdu_session_context["blocks"].push_back(snssai); 
  udsf_pdu_session_context["blocks"].push_back(plmn);
 
  amf_n2_inst->curl_http_client_udsf(udsf_put_url,udsf_pdu_session_context.dump(),"PUT",udsf_response);

  
  /************************hxs add****************/



  // TODO: Remove hardcoded values
   std::string remote_uri =
      smf_addr  + ":8889/nsmf-pdusession/v2/sm-contexts";
  nlohmann::json pdu_session_establishment_request;
  pdu_session_establishment_request["supi"]          = supi.c_str();
  pdu_session_establishment_request["pei"]           = "imei-200000000000001";
  pdu_session_establishment_request["gpsi"]          = "msisdn-200000000001";
  pdu_session_establishment_request["dnn"]           = dnn.c_str();
  pdu_session_establishment_request["sNssai"]["sst"] = 1;
  pdu_session_establishment_request["sNssai"]["sd"]  = psc.get()->snssai.sD;
  pdu_session_establishment_request["pduSessionId"] = psc.get()->pdu_session_id;
  pdu_session_establishment_request["requestType"] =
      "INITIAL_REQUEST";  // TODO: from SM_MSG
  pdu_session_establishment_request["servingNfId"] = "servingNfId";
  pdu_session_establishment_request["servingNetwork"]["mcc"] =
      psc.get()->plmn.mcc;
  pdu_session_establishment_request["servingNetwork"]["mnc"] =
      psc.get()->plmn.mnc;
  pdu_session_establishment_request["anType"] = "3GPP_ACCESS";  // TODO
  pdu_session_establishment_request["smContextStatusUri"] =
      "http://" +
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.n11.addr4))) +
      "/nsmf-pdusession/callback/" + supi + "/" +
      std::to_string(psc.get()->pdu_session_id);

  pdu_session_establishment_request["n1MessageContainer"]["n1MessageClass"] =
      "SM";
  pdu_session_establishment_request["n1MessageContainer"]["n1MessageContent"]
                                   ["contentId"] = "n1SmMsg";

  std::string json_part = pdu_session_establishment_request.dump();
  std::string n1SmMsg;
  octet_stream_2_hex_stream((uint8_t*) bdata(sm_msg), blength(sm_msg), n1SmMsg);
  curl_http_client(
      remote_uri, json_part, n1SmMsg, "", supi, psc.get()->pdu_session_id);
}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(
    itti_nsmf_pdusession_release_sm_context& itti_msg) {
  //std::shared_ptr<pdu_session_context> psc ;
  //= supi_to_pdu_ctx(itti_msg.supi);
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
    //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;

   std::string record_id = "RECORD_ID=\'" +itti_msg.supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
  }
  else{
     Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      psc.get()->pdu_session_context_from_json(udsf_response);
      //psc = std::shared_ptr<pdu_session_context>(psc1);
  }
  //***************************stateless
  
  string smf_addr;
  if (!psc.get()->smf_available) {
    if (!smf_selection_from_configuration(smf_addr)) {
      Logger::amf_n11().error("No candidate smf is avaliable");
      return;
    }
  } else {
    smf_selection_from_context(smf_addr);
  }
  string remote_uri = psc.get()->location + "release";
  nlohmann::json pdu_session_release_request;
  pdu_session_release_request["supi"]          = itti_msg.supi.c_str();
  pdu_session_release_request["dnn"]           = psc.get()->dnn.c_str();
  pdu_session_release_request["sNssai"]["sst"] = 1;
  pdu_session_release_request["sNssai"]["sd"]  = "0";
  pdu_session_release_request["pduSessionId"]  = psc.get()->pdu_session_id;
  pdu_session_release_request["cause"]         = "REL_DUE_TO_REACTIVATION";
  pdu_session_release_request["ngApCause"]     = "radioNetwork";
  std::string json_part = pdu_session_release_request.dump();
  curl_http_client(
      remote_uri, json_part, "", "", itti_msg.supi, psc.get()->pdu_session_id);
}

// Context management functions
//------------------------------------------------------------------------------
bool amf_n11::is_supi_to_pdu_ctx(const std::string& supi) const {
  std::shared_lock lock(m_supi2pdu);
  return bool{supi2pdu.count(supi) > 0};
}

std::shared_ptr<pdu_session_context> amf_n11::supi_to_pdu_ctx(
    const std::string& supi) const {
  std::shared_lock lock(m_supi2pdu);
  return supi2pdu.at(supi);
}

//------------------------------------------------------------------------------
void amf_n11::set_supi_to_pdu_ctx(
    const string& supi, std::shared_ptr<pdu_session_context> psc) {
  std::shared_lock lock(m_supi2pdu);
  supi2pdu[supi] = psc;
}

// SMF selection
//------------------------------------------------------------------------------
bool amf_n11::smf_selection_from_configuration(std::string& smf_addr) {
  for (int i = 0; i < amf_cfg.smf_pool.size(); i++) {
    if (amf_cfg.smf_pool[i].selected) {
      // smf_addr = "http://" + amf_cfg.smf_pool[i].ipv4 + ":" +
      // amf_cfg.smf_pool[i].port;
      smf_addr = amf_cfg.smf_pool[i].ipv4 + ":" + amf_cfg.smf_pool[i].port;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool amf_n11::smf_selection_from_context(std::string& smf_addr) {
  // TODO:
}

// handlers for smf client response
//------------------------------------------------------------------------------
void amf_n11::handle_post_sm_context_response_error_400() {}

//------------------------------------------------------------------------------
void amf_n11::handle_post_sm_context_response_error(
    long code, std::string cause, bstring n1sm, std::string supi,
    uint8_t pdu_session_id) {
  print_buffer("amf_n11", "n1 sm", (uint8_t*) bdata(n1sm), blength(n1sm));
  itti_n1n2_message_transfer_request* itti_msg =
      new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
  itti_msg->n1sm           = n1sm;
  itti_msg->is_n2sm_set    = false;
  itti_msg->supi           = supi;
  itti_msg->pdu_session_id = pdu_session_id;
  std::shared_ptr<itti_n1n2_message_transfer_request> i =
      std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n11().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n11::curl_http_client(
    std::string remoteUri, std::string jsonData, std::string n1SmMsg,
    std::string n2SmMsg, std::string supi, uint8_t pdu_session_id) {
  Logger::amf_n11().debug("Call SMF service: %s", remoteUri.c_str());

  uint8_t number_parts = 0;
  mime_parser parser   = {};
  std::string body;
  std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());

  //***************************stateless
  nlohmann::json udsf_response;
  std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
  if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
    Logger::amf_n2().error("No existing pdu_session_context with assoc_id ");
    return;
  }
  Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
  //psc1->pdu_session_context_from_json(udsf_response);
  //psc = std::shared_ptr<pdu_session_context>(psc1);
  psc.get()->pdu_session_context_from_json(udsf_response);
  //***************************stateless

  // if (is_supi_to_pdu_ctx(supi)) {
  //   psc = supi_to_pdu_ctx(supi);
  // } else {
  //   Logger::amf_n11().warn(
  //       "PDU Session context for SUPI %s doesn't exit!", supi.c_str());
  //   // TODO:
  // }

  if ((n1SmMsg.size() > 0) and (n2SmMsg.size() > 0)) {
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n1SmMsg, n2SmMsg);
  } else if (n1SmMsg.size() > 0) {  // only N1 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n1SmMsg,
        multipart_related_content_part_e::NAS);
  } else if (n2SmMsg.size() > 0) {  // only N2 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n2SmMsg,
        multipart_related_content_part_e::NGAP);
  }

  Logger::amf_n11().debug(
      "Send HTTP message to SMF with body %s", body.c_str());

  uint32_t str_len = body.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) body.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;

    std::string content_type = "content-type: multipart/related; boundary=" +
                               std::string(CURL_MIME_BOUNDARY);
    headers = curl_slist_append(headers, content_type.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remoteUri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, SMF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get cause from the response
    std::string response           = *httpData.get();
    std::string json_data_response = "";
    std::string n1sm               = "";
    std::string n2sm               = "";
    nlohmann::json response_data   = {};
    bstring n1sm_hex, n2sm_hex;

    Logger::amf_n11().debug("Get response with HTTP code (%d)", httpCode);
    if (static_cast<http_response_codes_e>(httpCode) ==
        http_response_codes_e::HTTP_RESPONSE_CODE_0) {
      Logger::amf_n11().error(
          "Cannot get response when calling %s", remoteUri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      psc.get()->smf_context_location =
          "/nsmf-pdusession/v2/sm-contexts/1";  // try to fix bugs for
                                                // no-response from SMF when
                                                // requesting
                                                // /nsmf-pdusession/v2/sm-contexts
                                                // (first pdu session
                                                // establishment request)
      return;
    }
    if (response.size() > 0) {
      number_parts = multipart_parser(response, json_data_response, n1sm, n2sm);
    }

    if ((static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_204_UPDATED)) {
      // ERROR
      if (response.size() < 1) {
        Logger::amf_n11().error("There's no content in the response");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        // TODO: send context response error
        return;
      }

      try {
        response_data = nlohmann::json::parse(json_data_response);
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n11().warn("Could not get Json content from the response");
        // Set the default Cause
        response_data["error"]["cause"] = "504 Gateway Timeout";
      }

      Logger::amf_n11().debug(
          "Get response with jsonData: %s", json_data_response.c_str());
      msg_str_2_msg_hex(
          n1sm.substr(0, n1sm.length() - 2),
          n1sm_hex);  // TODO: pdu session establishment reject bugs from SMF
      print_buffer(
          "amf_n11", "Get response with n1sm:", (uint8_t*) bdata(n1sm_hex),
          blength(n1sm_hex));

      std::string cause = response_data["error"]["cause"];
      Logger::amf_n11().warn(
          "Call Network Function services failure (with cause %s)",
          cause.c_str());
      if (!cause.compare("DNN_DENIED"))
        handle_post_sm_context_response_error(
            httpCode, cause, n1sm_hex, supi, pdu_session_id);
    } else {  // Response with success code
      // Store location of the created context in case of PDU Session
      // Establishment
      std::string header_response = *httpHeaderData.get();
      std::string CRLF            = "\r\n";
      std::size_t location_pos    = header_response.find("Location");

      if (location_pos != std::string::npos) {
        std::size_t crlf_pos = header_response.find(CRLF, location_pos);
        if (crlf_pos != std::string::npos) {
          std::string location = header_response.substr(
              location_pos + 10, crlf_pos - (location_pos + 10));
          Logger::amf_n11().info(
              "Location of the created SMF context: %s", location.c_str());
          psc.get()->smf_context_location = location;


        std::string record_id = "RECORD_ID=\'" +supi  + "\'";
        std::string udsf_put_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
        nlohmann::json udsf_put_pdu_session_context;
        udsf_put_pdu_session_context["meta"] ["tags"] = {
                                            {"RECORD_ID",nlohmann::json::array({supi})},
                                            {"from_nf_ID",nlohmann::json::array({"AMF_1234"})}
                                            } ;
        udsf_put_pdu_session_context["blocks"] = nlohmann::json::array({
          {{"Content-ID", "smf_context_location"},{"Content-Type", "varchar(128)"},{"content",psc.get()->smf_context_location}},
          });
        std::string json_part = udsf_put_pdu_session_context.dump();
        amf_n2_inst->curl_http_client_udsf(udsf_put_url,json_part,"PUT",udsf_response);

        }
      }

      // Transfer N1/N2 to gNB/UE if available
      if (number_parts > 1) {
        try {
          response_data = nlohmann::json::parse(json_data_response);
        } catch (nlohmann::json::exception& e) {
          Logger::amf_n11().warn(
              "Could not get Json content from the response");
          // TODO:
        }

        itti_n1n2_message_transfer_request* itti_msg =
            new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);

        if (n1sm.size() > 0) {
          msg_str_2_msg_hex(n1sm, n1sm_hex);
          print_buffer(
              "amf_n11", "Get response n1sm:", (uint8_t*) bdata(n1sm_hex),
              blength(n1sm_hex));
          itti_msg->n1sm        = n1sm_hex;
          itti_msg->is_n1sm_set = true;
        }
        if (n2sm.size() > 0) {
          msg_str_2_msg_hex(n2sm, n2sm_hex);
          print_buffer(
              "amf_n11", "Get response n2sm:", (uint8_t*) bdata(n2sm_hex),
              blength(n2sm_hex));
          itti_msg->n2sm           = n2sm_hex;
          itti_msg->is_n2sm_set    = true;
          itti_msg->n2sm_info_type = response_data
              ["n2SmInfoType"];  // response_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"];
        }

        itti_msg->supi           = supi;
        itti_msg->pdu_session_id = pdu_session_id;

       



        std::shared_ptr<itti_n1n2_message_transfer_request> i =
            std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_n11().error(
              "Could not send ITTI message %s to task TASK_AMF_APP",
              i->get_msg_name());
        }
      }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  free_wrapper((void**) &body_data);
}

bool amf_n11::discover_smf(
    std::string& smf_addr, std::string& smf_api_version, const snssai_t snssai,
    const plmn_t plmn, const std::string dnn) {
  Logger::amf_n11().debug(
      "Send NFDiscovery to NRF to discover the available SMFs");
  bool result = true;

  nlohmann::json json_data = {};
  // TODO: remove hardcoded values
  std::string url =
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(amf_cfg.nrf_addr.port) + "/nnrf-disc/" +
      amf_cfg.nrf_addr.api_version +
      "/nf-instances?target-nf-type=SMF&requester-nf-type=AMF";

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::amf_n11().debug(
        "NFDiscovery, response from NRF, HTTP Code: %d", httpCode);

    if (httpCode == 200) {
      Logger::amf_n11().debug("NFDiscovery, got successful response from NRF");

      nlohmann::json response_data = {};
      try {
        response_data = nlohmann::json::parse(*httpData.get());
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n11().warn(
            "NFDiscovery, could not parse json from the NRF "
            "response");
      }
      Logger::amf_n11().debug(
          "NFDiscovery, response from NRF, json data: \n %s",
          response_data.dump().c_str());

      // Process data to obtain SMF info
      if (response_data.find("nfInstances") != response_data.end()) {
        for (auto& it : response_data["nfInstances"].items()) {
          nlohmann::json instance_json = it.value();
          // TODO: convert instance_json to SMF profile
          // TODO: add SMF to the list of available SMF
          // TODO: check with sNSSAI and DNN
          // TODO: PLMN (need to add plmnList into NRF profile, SMF profile)
          // for now, just IP addr of SMF of the first NF instance
          if (instance_json.find("ipv4Addresses") != instance_json.end()) {
            if (instance_json["ipv4Addresses"].size() > 0)
              smf_addr =
                  instance_json["ipv4Addresses"].at(0).get<std::string>();
            // break;
          }
          if (instance_json.find("nfServices") != instance_json.end()) {
            if (instance_json["nfServices"].size() > 0) {
              nlohmann::json nf_service = instance_json["nfServices"].at(0);
              if (nf_service.find("versions") != nf_service.end()) {
                nlohmann::json nf_version = nf_service["versions"].at(0);
                if (nf_version.find("apiVersionInUri") != nf_version.end()) {
                  smf_api_version =
                      nf_version["apiVersionInUri"].get<std::string>();
                }
              }
            }
            break;
          }
        }
      }
    } else {
      Logger::amf_n11().warn("NFDiscovery, could not get response from NRF");
      result = false;
    }

    Logger::amf_n11().debug(
        "NFDiscovery, SMF Addr: %s, SMF Api Version: %s", smf_addr.c_str(),
        smf_api_version.c_str());

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return result;
}

//-----------------------------------------------------------------------------------------------------
void amf_n11::register_nf_instance(
    std::shared_ptr<itti_n11_register_nf_instance_request> msg) {
  Logger::amf_n11().debug(
      "Send NF Instance Registration to NRF (HTTP version %d)",
      msg->http_version);
  nlohmann::json json_data = {};
  msg->profile.to_json(json_data);

  std::string url =
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(amf_cfg.nrf_addr.port) + "/nnrf-nfm/" +
      amf_cfg.nrf_addr.api_version + "/nf-instances/" +
      msg->profile.get_nf_instance_id();

  Logger::amf_n11().debug(
      "Send NF Instance Registration to NRF, NRF URL %s", url.c_str());

  std::string body = json_data.dump();
  Logger::amf_n11().debug(
      "Send NF Instance Registration to NRF, msg body: \n %s", body.c_str());

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::amf_n11().debug(
        "NFDiscovery, response from NRF, HTTP Code: %d", httpCode);
    nlohmann::json response_data = {};
    if (httpCode == 200) {
      Logger::amf_n11().debug(
          "NFRegistration, got successful response from NRF");

      //nlohmann::json response_data = {};
      try {
        response_data = nlohmann::json::parse(*httpData.get());
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n11().warn(
            "NFDiscovery, could not parse json from the NRF "
            "response");
      }
      Logger::amf_n11().debug(
          "NFDiscovery, response from NRF, json data: \n %s",
          response_data.dump().c_str());
    }
    // send response to APP to process
    if (httpCode == 201) {
      Logger::amf_n11().debug("NRF Response\n");
      try{
	   response_data =nlohmann::json::parse(*httpData.get());
	   Logger::amf_n11().warn(
	         "register from NRF,json data:\n %s",
		 response_data.dump().c_str());
      }catch (nlohmann::json::exception& e) {
	   Logger::amf_n11().debug("NF Instance Registration, could not parse json from the NRF "
				  "response");
      }
      Logger::amf_n11().warn("register-2 from NRF,json data:\n %s", response_data.dump().c_str());
      itti_n11_register_nf_instance_response* itti_msg = new itti_n11_register_nf_instance_response(TASK_AMF_N11, TASK_AMF_APP);
      itti_msg->http_response_code = httpCode;
      itti_msg->http_version       = msg->http_version;
      Logger::amf_app().debug("Registered AMF profile (from NRF) version is %d",msg->http_version);
      itti_msg->profile.from_json(response_data);
      //itti_msg->profile.display();
      //amf_app_inst->handle_itti_msg(itti_msg);
      std::shared_ptr<itti_n11_register_nf_instance_response> i =
	            std::shared_ptr<itti_n11_register_nf_instance_response>(itti_msg);
      int ret = itti_inst->send_msg(i);
      if (RETURNok != ret) {
        Logger::amf_n11().error(
            "Could not send ITTI message %s to task TASK_AMF_APP",
            itti_msg->get_msg_name());
      }
    } else {
      Logger::amf_n11().warn(
          "NF Instance Registration, could not get response from NRF");
    }

      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void amf_n11::update_nf_instance(
    std::shared_ptr<itti_n11_update_nf_instance_request> msg) {
  Logger::amf_n11().debug(
      "Send NF Update to NRF (HTTP version %d)", msg->http_version);

  nlohmann::json json_data = nlohmann::json::array();
  for (auto i : msg->patch_items) {
    nlohmann::json item = {};
    to_json(item, i);
    json_data.push_back(item);
  }
  std::string body = json_data.dump();
  Logger::amf_n11().debug("Send NF Update to NRF, Msg body %s", body.c_str());

  std::string url =
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(amf_cfg.nrf_addr.port) + "/nnrf-nfm/" +
      amf_cfg.nrf_addr.api_version + "/nf-instances/" +
      msg->amf_instance_id;

  Logger::amf_n11().debug("Send NF Update to NRF, NRF URL %s", url.c_str());

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(
        headers, "content-type: application/json");  // TODO: json-patch+json
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);

    if (msg->http_version == 2) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      // we use a self-signed test server, skip verification during debugging
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(
          curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    }

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::amf_n11().debug(
        "NF Update, response from NRF, HTTP Code: %d", httpCode);

    if ((static_cast<http_response_codes_e>(httpCode) ==
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) or
        (static_cast<http_response_codes_e>(httpCode) ==
         http_response_codes_e::HTTP_RESPONSE_CODE_204_UPDATED)) {
      Logger::amf_n11().debug("NF Update, got successful response from NRF");

      // TODO: in case of response containing NF profile
      // send response to APP to process
      std::shared_ptr<itti_n11_update_nf_instance_response> itti_msg =
          std::make_shared<itti_n11_update_nf_instance_response>(
              TASK_AMF_N11, TASK_AMF_APP);
      itti_msg->http_response_code = httpCode;
      itti_msg->http_version       = msg->http_version;
      itti_msg->amf_instance_id    = msg->amf_instance_id;

      int ret = itti_inst->send_msg(itti_msg);
      if (RETURNok != ret) {
        Logger::amf_n11().error(
            "Could not send ITTI message %s to task TASK_AMF_APP",
            itti_msg->get_msg_name());
      }
    } else {
      Logger::amf_n11().warn("NF Update, could not get response from NRF");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}
