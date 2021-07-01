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

/*! \file gNB_context.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "ue_context.hpp"
#include <nlohmann/json.hpp>
#include "logger.hpp"
#include "Record.h"
using namespace std;
using namespace oai::amf::model;


uint32_t ue_context::ue_context_ran_ue_ngap_id_from_json(nlohmann::json j)
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
void ue_context::ue_context_tai_from_json(nlohmann::json j,Tai_t &Tai_json)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "tai")
        { 
            nlohmann::json  s;
            string j;
            s = it_block->at("content");
            
            Tai_json.mcc = s.at("mcc");
            Tai_json.mnc = s.at("mnc") ;
            j = s.at("tac");
            Tai_json.tac =  atoi(j.c_str());
            //printf("ue_context_tai_from_json %s---mcc %s---mnc %s--\n",s.dump().c_str(),tai_json.mcc,tai_json.mnc);
            return ; 
            // s  = nlohmann::json::parse(j);
            // s.at("mnc").get_to(tai_json.mnc);
            // s.at("mcc").get_to(tai_json.mcc);

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}



bool ue_context::ue_context_from_json(nlohmann::json j)
{
    //std::shared_ptr<gnb_context> gc1;
    // globalRanNodeId = gnb_context_globalRanNodeId_from_json(j);
    // gnb_name = gnb_context_gnb_name_from_json(j);
    // ng_state = amf_ng_gnb_state_s(gnb_context_ng_state_from_json(j));
    // next_sctp_stream = gnb_context_next_sctp_stream_from_json(j);
    // next_sctp_stream = gnb_context_instreams_from_json(j);
  ue_context_tai_from_json(j,tai);
  ran_ue_ngap_id = ue_context_ran_ue_ngap_id_from_json(j);
    //     printf("gnbname-%s\n",gnb_name.c_str());
    // printf("globalgnbid-%d\n",globalRanNodeId);
    // printf("next stream-%d\n",next_sctp_stream);

    return true;
}
