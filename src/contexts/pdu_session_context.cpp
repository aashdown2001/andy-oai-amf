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

/*! \file pdu_session_context.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "pdu_session_context.hpp"
#include <nlohmann/json.hpp>
#include "logger.hpp"
#include "Record.h"
#include "conversions.hpp"
using namespace std;
using namespace oai::amf::model;

extern void msg_str_2_msg_hex(std::string msg, bstring& b);
extern void octet_stream_2_hex_stream(uint8_t *buf, int len, std::string &out);

//------------------------------------------------------------------------------
pdu_session_context::pdu_session_context() {
  smf_available    = false;
  isn2sm_avaliable = false;
  isn1sm_avaliable = false;
}

//------------------------------------------------------------------------------
pdu_session_context::~pdu_session_context() {


    // bdestroy(n1sm);
    // bdestroy(n2sm);
    // if(!n2sm)
    // printf("null");
    // else
    // printf("11");
}


uint32_t pdu_session_context::pdu_session_context_ran_ue_ngap_id_from_json(nlohmann::json j)
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
    Logger::amf_server().error("get_value ran ue ngap id from json  is error");
}

long pdu_session_context::pdu_session_context_amf_ue_ngap_id_from_json(nlohmann::json j)
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
    Logger::amf_server().error("get_value maf ue ngap id from json  is error");
}

uint8_t pdu_session_context::pdu_session_context_req_type_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "req_type")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value req type from json  is error");
}


uint8_t pdu_session_context::pdu_session_context_pdu_session_id_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "pdu_session_id")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value pdu session id from json  is error");
}


bool pdu_session_context::pdu_session_context_isn2sm_avaliable_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "isn2sm_avaliable")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value n2sm avaliable from json  is error");
}

bool pdu_session_context::pdu_session_context_isn1sm_avaliable_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "isn1sm_avaliable")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value n1sm avaliable from json  is error");
}

std::string pdu_session_context::pdu_session_context_dnn_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "dnn")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value dnn from json  is error");
}


std::string pdu_session_context::pdu_session_context_remote_smf_addr_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "remote_smf_addr")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value remot smf addr from json  is error");
}

bool pdu_session_context::pdu_session_context_smf_available_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "smf_available")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value smf acailablefrom json  is error");
}


std::string pdu_session_context::pdu_session_context_location_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "location")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value location from json  is error");
}

std::string pdu_session_context::pdu_session_context_smf_context_location_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "smf_context_location")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value smf contexxt location from json  is error");
}

void pdu_session_context::pdu_session_context_snssai_from_json(nlohmann::json j,snssai_t &snssai_json)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "snssai")
        { 
            nlohmann::json  s;
            string j;
            s = it_block->at("content");
            snssai_json.sD = s.at("sD") ;
            j = s.at("sST");
            snssai_json.sST =  atoi(j.c_str());
            //printf("ue_context_tai_from_json %s---mcc %s---mnc %s--\n",s.dump().c_str(),tai_json.mcc,tai_json.mnc);
            return ; 
            // s  = nlohmann::json::parse(j);
            // s.at("mnc").get_to(tai_json.mnc);
            // s.at("mcc").get_to(tai_json.mcc);

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value snassai from json  is error");
}

void pdu_session_context::pdu_session_context_plmn_from_json(nlohmann::json j,plmn_t &plmn_json)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "plmn")
        { 
            nlohmann::json  s;
            string j;
            s = it_block->at("content");
            
            plmn_json.mcc = s.at("mcc");
            plmn_json.mnc = s.at("mnc") ;
            j = s.at("tac");
            plmn_json.tac =  atoi(j.c_str());
            //printf("pdu_session_context_plmn_from_json %s---mcc %s---mnc %s--\n",s.dump().c_str(),plmn_json.mcc,plmn_json.mnc);
            return ; 
            // s  = nlohmann::json::parse(j);
            // s.at("mnc").get_to(tai_json.mnc);
            // s.at("mcc").get_to(tai_json.mcc);

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value plmn from json  is error");
}


void pdu_session_context::pdu_session_context_n1sm_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "n1sm")
        { 
            string  s  = it_block->at("content");
            // msg_str_2_msg_hex(s.substr(0, s.length()),n1sm);
            unsigned int msg_len = s.length();
            char* data           = (char*) malloc(msg_len + 1);
            memset(data, 0, msg_len + 1);
            memcpy((void*) data, (void*) s.c_str(), msg_len);
            //printf("data: %s\n", data);
            uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
            conv::ascii_to_hex(msg_hex, (const char*) data);
            n1sm = blk2bstr(msg_hex, (msg_len / 2));
            return ;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value n1sm from json  is error");
}

void pdu_session_context::pdu_session_context_n2sm_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "n2sm")
        { 
            string  s  = it_block->at("content");
           // msg_str_2_msg_hex(s.substr(0, s.length()),n2sm);
           //printf("----string n2sm  pdu _session----------------(%s)\n",s.c_str());
          //  msg_str_2_msg_hex(s.c_str(), n2sm);
            
            unsigned int msg_len = s.length();
            char* data           = (char*) malloc(msg_len + 1);
            memset(data, 0, msg_len + 1);
            memcpy((void*) data, (void*) s.c_str(), msg_len);
            //printf("data: %s\n", data);
            uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
            conv::ascii_to_hex(msg_hex, (const char*) data);
            n2sm = blk2bstr(msg_hex, (msg_len / 2));


            //  string ssaa;
            //  octet_stream_2_hex_stream((uint8_t*) bdata(n2sm), blength(n2sm), ssaa);
            // printf("-----bstring n2sm  pdu _session----------------(%s)\n",ssaa.c_str());
            return ;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value n2sm from json  is error");
}




bool pdu_session_context::pdu_session_context_from_json(nlohmann::json j)
{
    //std::shared_ptr<gnb_context> gc1;
    //     printf("gnbname-%s\n",gnb_name.c_str());
    // printf("globalgnbid-%d\n",globalRanNodeId);
    // printf("next stream-%d\n",next_sctp_stream);
  pdu_session_context_n1sm_from_json(j);
  pdu_session_context_n2sm_from_json(j);

  ran_ue_ngap_id = pdu_session_context_ran_ue_ngap_id_from_json(j);
  amf_ue_ngap_id = pdu_session_context_amf_ue_ngap_id_from_json(j);
  req_type = pdu_session_context_req_type_from_json(j);
  pdu_session_id = pdu_session_context_pdu_session_id_from_json(j);
  isn2sm_avaliable = pdu_session_context_isn2sm_avaliable_from_json(j);

  isn1sm_avaliable = pdu_session_context_isn1sm_avaliable_from_json(j);
  dnn = pdu_session_context_dnn_from_json(j);
  //remote_smf_addr[0] = pdu_session_context_remote_smf_addr_from_json(j);
  smf_available = pdu_session_context_smf_available_from_json(j);
  location = pdu_session_context_location_from_json(j);

  pdu_session_context_snssai_from_json(j,snssai);
  pdu_session_context_plmn_from_json(j,plmn);

  smf_context_location = pdu_session_context_smf_context_location_from_json(j);



    return true;
}