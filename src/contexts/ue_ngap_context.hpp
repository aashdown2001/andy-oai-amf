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

#ifndef _UE_NGAP_CONTEXT_H_
#define _UE_NGAP_CONTEXT_H_

#include <stdint.h>

#include <map>

#include "amf.hpp"
#include "gNB_context.hpp"

using namespace sctp;
typedef enum {
  NGAP_UE_INVALID_STATE,
  NGAP_UE_WAITING_CSR,  // Context Setup Response(CSR)
  NGAP_UE_HANDOVER,
  NGAP_UE_CONNECTED,
  NGAP_UE_WAITING_CRR
} ng_ue_state_t;

class ue_ngap_context {
 public:
  ue_ngap_context() {
    ran_ue_ngap_id        = 0;
    amf_ue_ngap_id        = 0;
    target_ran_ue_ngap_id = 0;

    sctp_stream_recv = {};
    sctp_stream_send = {};

    gnb_assoc_id        = {};
    target_gnb_assoc_id = {};
    ueContextRequest    = false;
    s_tmsi_5g           = {};
    s_setid             = {};
    s_pointer           = {};
    s_tmsi              = {};
    tai                 = {};
    ng_ue_state         = NGAP_UE_INVALID_STATE;
    ncc                 = 0;
    initialUEMsg.buf    = new uint8_t[BUFFER_SIZE_1024];
    initialUEMsg.size   = 0;
  }
  virtual ~ue_ngap_context() {
    delete[] initialUEMsg.buf;
    initialUEMsg.buf  = nullptr;
    initialUEMsg.size = 0;
  }

  uint32_t ran_ue_ngap_id;         // 32bits
  long amf_ue_ngap_id : 40;        // 40bits
  uint32_t target_ran_ue_ngap_id;  // 32bits, for HO

  sctp_stream_id_t sctp_stream_recv;  // used to decide which ue in gNB
  sctp_stream_id_t sctp_stream_send;  // used to decide which ue in gNB

  sctp_assoc_id_t gnb_assoc_id;         // to find which gnb this UE belongs to
  sctp_assoc_id_t target_gnb_assoc_id;  // for HO

  bool ueContextRequest;
  uint32_t s_tmsi_5g;

  std ::string s_setid;
  std ::string s_pointer;
  std ::string s_tmsi;

  Tai_t tai;

  // state management, ue status over the air
  ng_ue_state_t ng_ue_state;
  uint8_t ncc;  // Next Hop Chaining Counter

  OCTET_STRING_t initialUEMsg;
};

#endif
