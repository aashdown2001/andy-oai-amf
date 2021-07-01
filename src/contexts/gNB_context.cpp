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

#include "gNB_context.hpp"
#include <nlohmann/json.hpp>
#include "logger.hpp"
#include "Record.h"
#include "conversions.hpp"
using namespace std;
using namespace oai::amf::model;

extern void msg_str_2_msg_hex(std::string msg, bstring& b);
bool get_value_string(const nlohmann::json &j, const string & nm, string &value){

  Record record;
  nlohmann::json::parse(j.dump()).get_to(record);
  std::set<nlohmann::json> block_set = record.getBlocks();   
  std::set<nlohmann::json>::iterator it_block;  
  for(it_block=block_set.begin();it_block!=block_set.end();)
  {  
      
      if(it_block->at("Content-ID") == nm)
      {
        //if((it_block->at("Content-Type") == "varhcar")
        value = it_block->at("content");
        return true;
      }
      block_set.erase(it_block++); 
  } 
  return false;
}
// bool get_value_int(const nlohmann::json &j, const string & nm, long &value){
//    Record record;
//   nlohmann::json::parse(j.dump()).get_to(record);
//   std::set<nlohmann::json> block_set = record.getBlocks();   
//   std::set<nlohmann::json>::iterator it_block;  
//   for(it_block=block_set.begin();it_block!=block_set.end();)
//   {  
      
//       if(it_block->at("Content-ID") == nm)
//       { 
//         string  s  = it_block->at("content");
//         value = atoi(s.c_str());
//         return true;
//       }
//       block_set.erase(it_block++); 
//   } 
//   return false;
// }
long gnb_context::gnb_context_globalRanNodeId_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "globalRanNodeId")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
    // long a;
    // if(!get_value_int(j,"globalRanNodeId",a))
    //     Logger::amf_server().error("get_value from json  is error");
    // return a;
}
std::string gnb_context::gnb_context_gnb_name_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "gnb_name")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}
int gnb_context::gnb_context_ng_state_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "ng_state")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}
sctp_stream_id_t gnb_context::gnb_context_next_sctp_stream_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "next_sctp_stream")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}
uint32_t gnb_context::gnb_context_sctp_assoc_id_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "sctp_assoc_id")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}
sctp_stream_id_t gnb_context::gnb_context_instreams_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "instreams")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value from json  is error");
}
void gnb_context::gnb_context_ue_radio_cap_ind_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "ue_radio_cap_ind")
        { 
            string  s  = it_block->at("content");
            //msg_str_2_msg_hex(s.substr(0, s.length()),ue_radio_cap_ind);
            unsigned int msg_len = s.length();
            char* data           = (char*) malloc(msg_len + 1);
            memset(data, 0, msg_len + 1);
            memcpy((void*) data, (void*) s.c_str(), msg_len);
            //printf("data: %s\n", data);
            uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
            conv::ascii_to_hex(msg_hex, (const char*) data);
            ue_radio_cap_ind = blk2bstr(msg_hex, (msg_len / 2));
            return ;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value n2sm from json  is error");
}

bool gnb_context::gnb_context_from_json(nlohmann::json j)
{
    //std::shared_ptr<gnb_context> gc1;
    sctp_assoc_id = gnb_context_sctp_assoc_id_from_json( j);
    globalRanNodeId = gnb_context_globalRanNodeId_from_json(j);
    gnb_name = gnb_context_gnb_name_from_json(j);
    ng_state = amf_ng_gnb_state_s(gnb_context_ng_state_from_json(j));
    next_sctp_stream = gnb_context_next_sctp_stream_from_json(j);
    next_sctp_stream = gnb_context_instreams_from_json(j);
    gnb_context_ue_radio_cap_ind_from_json(j);
    //     printf("gnbname-%s\n",gnb_name.c_str());
    // printf("globalgnbid-%d\n",globalRanNodeId);
    // printf("next stream-%d\n",next_sctp_stream);

    return true;
}
