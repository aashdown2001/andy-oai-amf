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
#include "conversions.hpp"
using namespace std;
using namespace oai::amf::model;
extern unsigned char *format_string_as_hex(std::string str);
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
  is_current_security_available                         = false;
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
    Logger::amf_server().error("get_value nas_context_ran_ue_ngap_id_from_json from json  is error");
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
    Logger::amf_server().error("get_value nas_context_amf_ue_ngap_id_from_json from json  is error");
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
    Logger::amf_server().error("get_value nas_context_amf_ue_ngap_id_from_json from json  is error");
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
    Logger::amf_server().error("get_value nas_context_nas_status_from_json from json  is error");
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
    Logger::amf_server().error("get_value nas_context_serving_network_from_json from json  is error");
}
std::string nas_context::nas_context_imsi_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "imsi")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context_serving_network_from_json from json  is error");
}


void nas_context::nas_context_security_ctx_from_json(nlohmann::json j)
{
    printf("------------nas_context_security_ctx_from_json----\n");
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "security_ctx")
        { 
// [
//     {"content": "0", "Content-ID": "vector_pointer", "Content-Type": "varchar(32)"}, 
//     {"content": "2", "Content-ID": "sc_type", "Content-Type": "varchar(32)"}, 
//     {"content": "3", "Content-ID": "ngksi", "Content-Type": "varchar(32)"}, 
//     {"content": "167", "Content-ID": "knas_enc", "Content-Type": "varchar(32)"},
//      {"content": "110", "Content-ID": "knas_int", "Content-Type": "varchar(32)"}, 


//      {"spare": "0", "seq_num": "1", "overflow": "0", "Content-ID": "ul_count", "Content-Type": "JSON"},
//       {"spare": "0", "seq_num": "2", "overflow": "0", "Content-ID": "dl_count", "Content-Type": "JSON"},
//        {"overflow": "0", "Content-ID": "ue_algorithms", "Content-Type": "JSON", "_5gs_encryption": "0"},
//         {"integrity": "1", "Content-ID": "nas_algs", "encryption": "0", "Content-Type": "JSON"}]
            nas_secu_ctx security_ctx_udsf;
            //string s;
            nlohmann::json j;
            int i=0;
            std::set<nlohmann::json> security_ctx_set ;
            if(it_block->find("content")!=it_block->end())
            {
                it_block->at("content").get_to(security_ctx_set);
            }
        
            std::set<nlohmann::json>::iterator it_security_ctx;  
            for(it_security_ctx=security_ctx_set.begin();it_security_ctx!=security_ctx_set.end();)
            {   
                string s;
                 if(it_security_ctx->at("Content-ID") == "vector_pointer")
                 {
                      s = it_security_ctx->at("content");
                      security_ctx_udsf.vector_pointer = atoi(s.c_str());
                      i++;
                      //printf("-----------security_ctx_udsf->vector_pointer-------%d\n",security_ctx_udsf.vector_pointer);
                 }
                if(it_security_ctx->at("Content-ID") == "sc_type")
                 {
                      s = it_security_ctx->at("content");
                      i++;
                      security_ctx_udsf.sc_type = nas_sc_type_t(atoi(s.c_str()));
                 }
                 if(it_security_ctx->at("Content-ID") == "ngksi")
                 {
                      s = it_security_ctx->at("content");
                      security_ctx_udsf.ngksi = atoi(s.c_str());
                      i++;
                 }
                if(it_security_ctx->at("Content-ID") == "ul_count")
                {
                    s = it_security_ctx->at("seq_num");
                    security_ctx_udsf.ul_count.seq_num = atoi(s.c_str());
                    s = it_security_ctx->at("overflow");
                    security_ctx_udsf.ul_count.overflow = atoi(s.c_str());
                    s = it_security_ctx->at("spare");
                    security_ctx_udsf.ul_count.spare = atoi(s.c_str());
                    i++;
                   // printf("---------security_ctx_udsf-------%x\n",security_ctx_udsf.ul_count.seq_num);
                }
                if(it_security_ctx->at("Content-ID") == "dl_count")
                {
                    s = it_security_ctx->at("seq_num");
                    security_ctx_udsf.dl_count.seq_num = atoi(s.c_str());
                    s = it_security_ctx->at("overflow");
                    security_ctx_udsf.dl_count.overflow = atoi(s.c_str());
                    s = it_security_ctx->at("spare");
                    security_ctx_udsf.dl_count.spare = atoi(s.c_str());
                    i++;
                    printf("---------security_ctx_udsf-------%x\n",security_ctx_udsf.dl_count.seq_num);
                }
                if(it_security_ctx->at("Content-ID") == "nas_algs")
                {
                    s = it_security_ctx->at("encryption");
                    security_ctx_udsf.nas_algs.encryption = atoi(s.c_str());
                    s = it_security_ctx->at("integrity");
                    security_ctx_udsf.nas_algs.integrity = atoi(s.c_str());
                    i++;
                }
                if(it_security_ctx->at("Content-ID") == "ue_algorithms")
                {
                    s = it_security_ctx->at("_5gs_encryption");
                    security_ctx_udsf.ue_algorithms._5gs_encryption = atoi(s.c_str());
                    s = it_security_ctx->at("_5gs_integrity");
                    security_ctx_udsf.ue_algorithms._5gs_integrity = atoi(s.c_str()); 
                    i++;
                }
                if(it_security_ctx->at("Content-ID") == "knas_int")
                {
                    s = it_security_ctx->at("content");
                    printf("------------knas_int-- string -----%s\n",s.c_str()); 
                    std::memcpy(security_ctx_udsf.knas_int, format_string_as_hex(s),s.length()/2+1);  
                    for(int xx=0;xx<AUTH_KNAS_INT_SIZE;xx++)   
                        printf("------------knas_int----uint8---%x\n",security_ctx_udsf.knas_int[xx]);
                    // j = s.at("knas_int");
                    // std::memcpy(security_ctx_udsf->knas_int, format_string_as_hex(j),j.length()/2+1);
                    i++;
                }
                if(it_security_ctx->at("Content-ID") == "knas_enc")
                {
                    s = it_security_ctx->at("content");
                    printf("------------knas_enc-- string -----%s\n",s.c_str());
                    std::memcpy(security_ctx_udsf.knas_enc, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<AUTH_KNAS_ENC_SIZE;uu++)   
                        printf("------------knas_enc----uint8---%x\n",security_ctx_udsf.knas_enc[uu]);
                   
                    i++;
                }
                 if(i==9)
                 {   security_ctx = new nas_secu_ctx;
                     *security_ctx = security_ctx_udsf;
                     for(int m=0;m<AUTH_KNAS_ENC_SIZE;m++)   
                        printf("------------knas_enc----uint8---%x\n",security_ctx->knas_int[m]);

                     printf("---------security_ctx_udsf.ul_count.seq_num------%x\n",security_ctx_udsf.ul_count.seq_num);
                     return ;
                 }
                    
                security_ctx_set.erase(it_security_ctx++); 

            }
            Logger::amf_server().error("get_value from json  is error 000000000");

          

           // s1 = s.at("ul_count");
           // j = s1.at("seq_num");
           // security_ctx_udsf->ul_count.seq_num = atoi(j.c_str());
           // j = s1.at("overflow");
           // security_ctx_udsf->ul_count.overflow = atoi(j.c_str());
           // j = s1.at("spare");
           // security_ctx_udsf->ul_count.spare = atoi(j.c_str());

            //s1 = s.at("dl_count");
           // j = s1.at("seq_num");
           // security_ctx_udsf->dl_count.seq_num = atoi(j.c_str());
           // j = s1.at("overflow");
           // security_ctx_udsf->dl_count.overflow = atoi(j.c_str());
           // j = s1.at("spare");
           // security_ctx_udsf->dl_count.spare = atoi(j.c_str());

            //string -> uint8
           // j = s.at("knas_enc");
            //uint8_t buffer_knas_enc[AUTH_KNAS_ENC_SIZE];
            //std::memcpy(security_ctx_udsf->knas_enc, format_string_as_hex(j),j.length()/2+1);       
            //for(int i=0;i<AUTH_KNAS_ENC_SIZE;i++)   
           //     printf("------------knas_int-------%x\n",security_ctx_udsf->knas_int[i]);
                
          //  j = s.at("knas_int");
         //   std::memcpy(security_ctx_udsf->knas_int, format_string_as_hex(j),j.length()/2+1);


          //  s1 = s.at("nas_algs");
           // j = s1.at("encryption");
           // security_ctx_udsf->nas_algs.encryption = atoi(j.c_str());
           // j = s1.at("integrity");
           // security_ctx_udsf->nas_algs.integrity = atoi(j.c_str());

            //j = s.at("ngksi");
           // security_ctx_udsf->ngksi = atoi(j.c_str());

         //   j = s.at("sc_type");
          //  security_ctx_udsf->sc_type= nas_sc_type_t(atoi(j.c_str()));

          //  s1 = s.at("ue_algorithms");
          //  j = s1.at("_5gs_encryption");
          //  security_ctx_udsf->ue_algorithms._5gs_encryption = atoi(j.c_str());
          //  j = s1.at("_5gs_integrity");
          //  security_ctx_udsf->ue_algorithms._5gs_integrity = atoi(j.c_str()); 

          //  security_ctx = security_ctx_udsf;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context_security_ctx_from_json from json  is error");
}
void nas_context::nas_context_kamf_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {         
        if(it_block->at("Content-ID") == "kamf")
        { 
            string  s  = it_block->at("content");
            printf("---------------kamf from udsf %s-----------\n",s.c_str());
            std::memcpy(kamf[0], format_string_as_hex(s),s.length()/2+1);  
            for(int uu=0;uu<32;uu++)   
                 printf("------------kamf----uint8---%x\n",kamf[0][uu]);
                   
            return  ;

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context_kamf_from_jsons from json  kamf is error");
}

bool nas_context::nas_context_is_stacs_available_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_stacs_available")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context_is_stacs_available_from_json from json  is error");
}

int nas_context::nas_context__5gmm_state_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "_5gmm_state")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get  value nas_context__5gmm_state_from_json from json  is error");
}


uint8_t nas_context::nas_context_registration_type_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "kamf")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_registration_type_from_json from json  is error");
}




bool nas_context::nas_context_follow_on_req_pending_ind_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "follow_on_req_pending_ind")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_follow_on_req_pending_ind_from_json from json  is error");
}

uint8_t nas_context::nas_context_ngKsi_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ngKsi")
        { 
            string  s  = it_block->at("content");
           
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ngKsi_from_json from json  is error");
}


void nas_context::nas_context_auts_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "auts")
        { 
            string  s  = it_block->at("content");
            unsigned int msg_len = s.length();
            char* data           = (char*) malloc(msg_len + 1);
            memset(data, 0, msg_len + 1);
            memcpy((void*) data, (void*) s.c_str(), msg_len);
            //printf("data: %s\n", data);
            uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
            conv::ascii_to_hex(msg_hex, (const char*) data);
            auts = blk2bstr(msg_hex, (msg_len / 2));


            //  string ssaa;
            //  octet_stream_2_hex_stream((uint8_t*) bdata(n2sm), blength(n2sm), ssaa);
            // printf("-----bstring n2sm  pdu _session----------------(%s)\n",ssaa.c_str());
            return ;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context_auts_from_json n2sm from json  is error");
}





bool nas_context::nas_context_is_specific_procedure_for_registration_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_specific_procedure_for_registration_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_specific_procedure_for_registration_running_from_json from json  is error");
}


bool nas_context::nas_context_is_specific_procedure_for_deregistration_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_specific_procedure_for_deregistration_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_specific_procedure_for_deregistration_running_from_json from json  is error");
}

 bool nas_context::nas_context_is_specific_procedure_for_eCell_inactivity_running_from_json(nlohmann::json j)
 {
     Record record;
     nlohmann::json::parse(j.dump()).get_to(record);
     std::set<nlohmann::json> block_set = record.getBlocks();   
     std::set<nlohmann::json>::iterator it_block;  
     for(it_block=block_set.begin();it_block!=block_set.end();)
     {  
        
         if(it_block->at("Content-ID") == "is_specific_procedure_for_eCell_inactivity_running")
         { 
             string  s  = it_block->at("content");
             return atoi(s.c_str());
         }
         block_set.erase(it_block++); 
     } 
     Logger::amf_server().error("get value nas_context_is_specific_procedure_for_eCell_inactivity_running_from_json from json  is error");
 }



bool nas_context::nas_context_is_common_procedure_for_authentication_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_common_procedure_for_authentication_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_common_procedure_for_authentication_running_from_json from json  is error");
}


bool nas_context::nas_context_is_common_procedure_for_identification_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_common_procedure_for_identification_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value from nas_context_is_common_procedure_for_authentication_running_from_json json  is error");
}



bool nas_context::nas_context_is_common_procedure_for_security_mode_control_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_common_procedure_for_security_mode_control_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_common_procedure_for_security_mode_control_running_from_json from json  is error");
}


bool nas_context::nas_context_is_common_procedure_for_nas_transport_running_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_common_procedure_for_nas_transport_running")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_common_procedure_for_nas_transport_running_from_json from json  is error");
}

std::string nas_context::nas_context_Href_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "Href")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value  nas_context_Href_from_json from json  is error");
}


bool nas_context::nas_context_is_current_security_available_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_current_security_available")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value  nas_context_is_current_security_available_from_json from json  is error");
}



int nas_context::nas_context_registration_attempt_counter_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "registration_attempt_counter")
        { 
            string  s  = it_block->at("content");
           
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_registration_attempt_counter_from_json from json  is error");
}

bool nas_context::nas_context_is_imsi_present_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_imsi_present")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_imsi_present_from_json from json  is error");
}
uint8_t nas_context::nas_context_mmCapability_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "mmCapability")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_mmCapability_from_json from json  is error");
}
uint8_t nas_context::nas_context_ueSecurityCaplen_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ueSecurityCaplen")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ueSecurityCaplen_from_json from json  is error");
}
uint8_t nas_context::nas_context_ueSecurityCapEnc_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ueSecurityCapEnc")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ueSecurityCapEnc_from_json from json  is error");
}

uint8_t nas_context::nas_context_ueSecurityCapInt_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ueSecurityCapInt")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ueSecurityCapInt_from_json from json  is error");
}

uint8_t nas_context::nas_context_ueSecurityCapEEnc_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ueSecurityCapEEnc")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ueSecurityCapEEnc_from_json from json  is error");
}

uint8_t nas_context::nas_context_ueSecurityCapEInt_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  

        
        if(it_block->at("Content-ID") == "ueSecurityCapEInt")
        { 
            string  s  = it_block->at("content");
          
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_ueSecurityCapEInt_from_json from json  is error");
}

bool nas_context::nas_context_is_5g_guti_present_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_5g_guti_present")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_5g_guti_present_from_json from json  is error");
}


bool nas_context::nas_context_is_auth_vectors_present_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "is_auth_vectors_present")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_is_auth_vectors_present_from_json from json  is error");
}


bool nas_context::nas_context_to_be_register_by_new_suci_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "to_be_register_by_new_suci")
        { 
            string  s  = it_block->at("content");
            return atoi(s.c_str());
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_to_be_register_by_new_suci_from_json from json  is error");
}


std::string nas_context::nas_context_guti_from_json(nlohmann::json j)
{
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "guti")
        { 
            string  s  = it_block->at("content");
            return s;
        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get value nas_context_guti_from_json from json  is error");
}


void nas_context::nas_context__vector_from_json(nlohmann::json j)
{
    printf("------------nas_context__vector_from_json----\n");
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "_vector")
        { 
            auc_vector_t _vector_udsf;
            //string s;
            nlohmann::json j;
            int i=0;
            std::set<nlohmann::json> _vector_set ;
            if(it_block->find("content")!=it_block->end())
            {
                it_block->at("content").get_to(_vector_set);
            }
            std::set<nlohmann::json>::iterator it_security_ctx;  
            for(it_security_ctx=_vector_set.begin();it_security_ctx!=_vector_set.end();)
            {   
                string s;
                s = it_security_ctx->at("rand_new");
                _vector_udsf.rand_new = atoi(s.c_str());
                i++;
                s = it_security_ctx->at("rand");
                std::memcpy(_vector_udsf.rand, format_string_as_hex(s),s.length()/2+1);  
                for(int uu=0;uu<16;uu++)   
                    printf("-----------rand----uint8---%x\n",_vector_udsf.rand[uu]);
                i++;
                s = it_security_ctx->at("xres");
                std::memcpy(_vector_udsf.xres, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<16;uu++)   
                        printf("-----------xres----uint8---%x\n",_vector_udsf.xres[uu]);
                i++;
                s = it_security_ctx->at("autn");
                std::memcpy(_vector_udsf.autn, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<16;uu++)   
                        printf("-----------autn----uint8---%x\n",_vector_udsf.autn[uu]);
                i++;
                s = it_security_ctx->at("kasme");
                std::memcpy(_vector_udsf.kasme, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<32;uu++)   
                        printf("-----------kasme----uint8---%x\n",_vector_udsf.kasme[uu]);
                i++;
                
                if(i==5)
                {  
                     return ;
                }
                _vector_set.erase(it_security_ctx++); 

            }
            Logger::amf_server().error("get_value from json  is error 000000000");

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context__vector_from_json from json  is error");
}
 // void nas_context__5g_he_av_from_json(nlohmann::json j);

void nas_context::nas_context__5g_av_from_json(nlohmann::json j)
{
    printf("------------nas_context__5g_av_from_json----\n");
    Record record;
    nlohmann::json::parse(j.dump()).get_to(record);
    std::set<nlohmann::json> block_set = record.getBlocks();   
    std::set<nlohmann::json>::iterator it_block;  
    for(it_block=block_set.begin();it_block!=block_set.end();)
    {  
        
        if(it_block->at("Content-ID") == "_5g_av")
        { 
            _5G_AV_t _5g_av_udsf;
            //string s;
            nlohmann::json j;
            int i=0;
            std::set<nlohmann::json> _5g_av_set ;
            if(it_block->find("content")!=it_block->end())
            {
                it_block->at("content").get_to(_5g_av_set);
            }
            std::set<nlohmann::json>::iterator it_security_ctx;  
            for(it_security_ctx=_5g_av_set.begin();it_security_ctx!=_5g_av_set.end();)
            {   
                string s;
                s = it_security_ctx->at("avType");
                _5g_av_udsf.avType = atoi(s.c_str());
                i++;
                s = it_security_ctx->at("rand");
                std::memcpy(_5g_av_udsf.rand, format_string_as_hex(s),s.length()/2+1);  
                for(int uu=0;uu<16;uu++)   
                    printf("-----------rand----uint8---%x\n",_5g_av_udsf.rand[uu]);
                i++;
                s = it_security_ctx->at("hxres");
                std::memcpy(_5g_av_udsf.hxres, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<16;uu++)   
                        printf("-----------hxres----uint8---%x\n",_5g_av_udsf.hxres[uu]);
                i++;
                s = it_security_ctx->at("hxresStar");
                std::memcpy(_5g_av_udsf.hxresStar, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<16;uu++)   
                        printf("-----------hxresStar----uint8---%x\n",_5g_av_udsf.hxresStar[uu]);
                i++;
                s = it_security_ctx->at("autn");
                std::memcpy(_5g_av_udsf.autn, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<16;uu++)   
                        printf("-----------autn----uint8---%x\n",_5g_av_udsf.autn[uu]);
                i++;
                s = it_security_ctx->at("kseaf");
                std::memcpy(_5g_av_udsf.kseaf, format_string_as_hex(s),s.length()/2+1);  
                    for(int uu=0;uu<32;uu++)   
                        printf("-----------kseaf----uint8---%x\n",_5g_av_udsf.kseaf[uu]);
                i++;
                
                if(i==6)
                {  
                     return ;
                }
                _5g_av_set.erase(it_security_ctx++); 

            }
            Logger::amf_server().error("get_value from json  is error 000000000");

        }
        block_set.erase(it_block++); 
    } 
    Logger::amf_server().error("get_value nas_context__5g_av_from_json from json  is error");
}
 // void nas_context__security_from_json(nlohmann::json j);
 // void nas_context_security_ctx_from_json(nlohmann::json j);
 // void nas_context_requestedNssai_from_json(nlohmann::json j);




bool nas_context::nas_context_from_json(nlohmann::json j)
{
   ctx_avaliability_ind = nas_context_ctx_avaliability_ind_from_json(j);
   is_stacs_available = nas_context_is_stacs_available_from_json(j);
   amf_ue_ngap_id = nas_context_amf_ue_ngap_id_from_json(j);
   ran_ue_ngap_id = nas_context_ran_ue_ngap_id_from_json(j);
   nas_status = nas_context_nas_status_from_json(j);
   //_5gmm_state = nas_context__5gmm_state_from_json( j);

   //registration_type = nas_context_registration_type_from_json(j);
   follow_on_req_pending_ind = nas_context_follow_on_req_pending_ind_from_json(j);
   ngKsi= nas_context_ngKsi_from_json(j);

   imsi = nas_context_imsi_from_json(j);
   mmCapability = nas_context_mmCapability_from_json(j);
   ueSecurityCaplen = nas_context_ueSecurityCaplen_from_json(j);
   ueSecurityCapEnc = nas_context_ueSecurityCapEnc_from_json(j);
   ueSecurityCapInt = nas_context_ueSecurityCapInt_from_json(j);
   ueSecurityCapEEnc = nas_context_ueSecurityCapEEnc_from_json(j);
   ueSecurityCapEInt = nas_context_ueSecurityCapEInt_from_json(j);
   //nas_context_requestedNssai_from_json( j);
   serving_network = nas_context_serving_network_from_json(j);
   nas_context_auts_from_json(j);
   Logger::amf_n1().debug("------------------------1 --------------------------");
   is_specific_procedure_for_registration_running = nas_context_is_specific_procedure_for_registration_running_from_json(j);
   Logger::amf_n1().debug("------------------------2 --------------------------");
   is_specific_procedure_for_deregistration_running = nas_context_is_specific_procedure_for_deregistration_running_from_json(j);
   Logger::amf_n1().debug("------------------------3 --------------------------");
   is_specific_procedure_for_eCell_inactivity_running = nas_context_is_specific_procedure_for_eCell_inactivity_running_from_json(j);
   Logger::amf_n1().debug("------------------------4 --------------------------");
   is_common_procedure_for_authentication_running = nas_context_is_common_procedure_for_authentication_running_from_json(j);
   Logger::amf_n1().debug("------------------------5 --------------------------");
   is_common_procedure_for_identification_running = nas_context_is_common_procedure_for_identification_running_from_json(j);
   Logger::amf_n1().debug("------------------------6 --------------------------");
   //is_common_procedure_for_security_mode_control_running = nas_context_is_common_procedure_for_security_mode_control_running_from_json(j);
   Logger::amf_n1().debug("------------------------7 --------------------------");
   is_common_procedure_for_nas_transport_running = nas_context_is_common_procedure_for_nas_transport_running_from_json(j);
   Logger::amf_n1().debug("------------------------8 --------------------------");
  
   Logger::amf_n1().debug("------------------------9 --------------------------");
   Href = nas_context_Href_from_json(j);
   Logger::amf_n1().debug("------------------------10 --------------------------");
   //kamf = nas_context_kamf_from_json(j); 
   Logger::amf_n1().debug("------------------------11 --------------------------");
   is_current_security_available = nas_context_is_current_security_available_from_json(j);
   Logger::amf_n1().debug("------------------------12 --------------------------");
  
   nas_context__vector_from_json(j);
   Logger::amf_n1().debug("------------------------13 --------------------------");
   nas_context__5g_av_from_json(j);
   Logger::amf_n1().debug("------------------------14 --------------------------");
   nas_context_security_ctx_from_json(j);
   Logger::amf_n1().debug("------------------------15 --------------------------");
   nas_context_kamf_from_json(j);
  


   registration_attempt_counter = nas_context_registration_attempt_counter_from_json(j);
   is_imsi_present = nas_context_is_imsi_present_from_json(j);
  return true;
}
