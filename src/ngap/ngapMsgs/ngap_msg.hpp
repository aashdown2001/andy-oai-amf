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

/*! \file
 \brief
 \author
 \date 2021
 \email: contact@openairinterface.org
 */

#ifndef _NGAP_MSG_H_
#define _NGAP_MSG_H_

#include "MessageType.hpp"
#include "NAS-PDU.hpp"
#include "NgapIEsStruct.hpp"
#include "AMF-UE-NGAP-ID.hpp"
#include "RAN-UE-NGAP-ID.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_Criticality.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProcedureCode.h"
#include "Ngap_SuccessfulOutcome.h"
#include "Ngap_UnsuccessfulOutcome.h"
}

namespace ngap {

class ngap_msg {
 public:
  ngap_msg(){};
  virtual ~ngap_msg(){};

  void setMessageType(
      Ngap_NGAP_PDU_PR typeOfMessage, Ngap_ProcedureCode_t procedureCode,
      Ngap_Criticality criticality);
  void setAmfUeNgapId(unsigned long id);
  void setRanUeNgapId(uint32_t id);
  // virtual bool decodefrompdu(Ngap_NGAP_PDU_t *ngap_msg_pdu);
  unsigned long getAmfUeNgapId();
  uint32_t getRanUeNgapId();

 protected:
  Ngap_NGAP_PDU_t ngap_pdu;

  AMF_UE_NGAP_ID amfUeNgapId;
  RAN_UE_NGAP_ID ranUeNgapId;
};

}  // namespace ngap
#endif
