/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
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

/*! \file amf_statistics.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <string>


typedef struct {
  uint32_t gnb_id;
  std::string mcc;
  std::string mnc;
  std::string gnb_name;
  uint32_t tac;
  //long nrCellId;
} gnb_infos;

typedef struct {
  std::string connStatus;
  std::string registerStatus;
  uint32_t ranid;
  long amfid;
  std::string imsi;
  std::string guti;
  std::string mcc;
  std::string mnc;
  uint32_t cellId;
} ue_infos;

class statistics {
 public:
  void display();
  statistics();
  ~statistics();
 public:
  uint32_t gNB_connected;
  uint32_t UE_connected;
  uint32_t UE_registred;
  //uint32_t        system_pdu_sessions;
  std::vector<gnb_infos> gnbs;
  std::vector<ue_infos> ues;

};

#endif
