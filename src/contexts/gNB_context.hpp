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

#ifndef _GNB_CONTEXT_H_
#define _GNB_CONTEXT_H_

#include <stdint.h>

#include <string>
#include <vector>

#include "NgapIEsStruct.hpp"
#include "sctp_server.hpp"
#include "3gpp_23.003.h"

extern "C" {
#include "Ngap_PagingDRX.h"
#include "bstrlib.h"
}

using namespace sctp;
using namespace ngap;

typedef enum {
  NGAP_INIT,
  NGAP_RESETING,
  NGAP_READY,
  NGAP_SHUTDOWN
} ng_gnb_state_t;

static const std::vector<std::string> ng_gnb_state_str = {
    "NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};

class gnb_context {
 public:
  ng_gnb_state_t ng_state;

  std::string gnb_name;
  uint32_t globalRanNodeId;
  plmn_t plmn;
  e_Ngap_PagingDRX default_paging_drx;  // v32, v64, v128, v256
  std::vector<SupportedTaItem_t> s_ta_list;
  bstring ue_radio_cap_ind;

  sctp_assoc_id_t sctp_assoc_id;
  sctp_stream_id_t next_sctp_stream;
  sctp_stream_id_t instreams;
  sctp_stream_id_t outstreams;
};

#endif
