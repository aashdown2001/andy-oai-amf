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

/*! \file nas_context.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "nas_context.hpp"
#include <nlohmann/json.hpp>
#include "logger.hpp"
#include "Record.h"
using namespace std;
using namespace oai::amf::model;
//------------------------------------------------------------------------------
nas_context::nas_context() {
  security_ctx                                          = NULL;
  is_imsi_present                                       = false;
  is_stacs_available                                    = false;
  is_auth_vectors_present                               = false;
  is_specific_procedure_for_registration_running        = false;
  is_specific_procedure_for_deregistration_running      = false;
  is_specific_procedure_for_eCell_inactivity_running    = false;
  is_common_procedure_for_authentication_running        = false;
  is_common_procedure_for_identification_running        = false;
  is_common_procedure_for_security_mode_control_running = false;
  is_common_procedure_for_nas_transport_running         = false;
  auts                                                  = NULL;
  ueSecurityCaplen = 2;
  ueSecurityCapEEnc = 0;
  ueSecurityCapEInt = 0;
}

//------------------------------------------------------------------------------
nas_context::~nas_context() {}

uint32_t nas_context::nas_context_ran_ue_ngap_id_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "ran_ue_ngap_id")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}

long nas_context::nas_context_amf_ue_ngap_id_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "amf_ue_ngap_id")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}

bool nas_context::nas_context_ctx_avaliability_ind_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "ctx_avaliability_ind")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}

std::string nas_context::nas_context_nas_status_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "nas_status")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}

std::string nas_context::nas_context_serving_network_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "serving_network")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}

bool nas_context::nas_context_from_json(nlohmann::json j)
{
  
  ran_ue_ngap_id = nas_context_ran_ue_ngap_id_from_json(j);
  amf_ue_ngap_id = nas_context_amf_ue_ngap_id_from_json(j);
  ctx_avaliability_ind = nas_context_ctx_avaliability_ind_from_json(j);
  nas_status = nas_context_nas_status_from_json(j);
  serving_network = nas_context_serving_network_from_json(j);
    return true;
}