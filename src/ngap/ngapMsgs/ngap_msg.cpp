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

#include "ngap_msg.hpp"
#include "logger.hpp"

#include <iostream>

namespace ngap {

//------------------------------------------------------------------------------
void ngap_msg::setMessageType(
    Ngap_NGAP_PDU_PR typeOfMessage, Ngap_ProcedureCode_t procedureCode,
    Ngap_Criticality criticality) {
  ngap_pdu.present = typeOfMessage;
  switch (typeOfMessage) {
    case Ngap_NGAP_PDU_PR_initiatingMessage: {
      ngap_pdu.choice.initiatingMessage = (Ngap_InitiatingMessage_t*) calloc(
          1, sizeof(Ngap_InitiatingMessage_t));
      ngap_pdu.choice.initiatingMessage->procedureCode = procedureCode;
      ngap_pdu.choice.initiatingMessage->criticality   = criticality;
      // ngap_pdu.choice.initiatingMessage->value.present =
      // initiatingMsgValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_successfulOutcome: {
      ngap_pdu.choice.successfulOutcome = (Ngap_SuccessfulOutcome_t*) calloc(
          1, sizeof(Ngap_SuccessfulOutcome_t));
      ngap_pdu.choice.successfulOutcome->procedureCode = procedureCode;
      ngap_pdu.choice.successfulOutcome->criticality   = criticality;
      // ngap_pdu.choice.successfulOutcome->value.present =
      //   successfulOutcomeValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_unsuccessfulOutcome: {
      ngap_pdu.choice.unsuccessfulOutcome =
          (Ngap_UnsuccessfulOutcome_t*) calloc(
              1, sizeof(Ngap_UnsuccessfulOutcome_t));
      ngap_pdu.choice.unsuccessfulOutcome->procedureCode = procedureCode;
      ngap_pdu.choice.unsuccessfulOutcome->criticality   = criticality;
      // ngap_pdu.choice.unsuccessfulOutcome->value.present =
      //    unsuccessfulOutcomeValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_NOTHING: {
      std::cout << "Ngap_NGAP_PDU_PR_NOTHING"
                << "(messageType encode error)" << std::endl;
    }
  }
}

//------------------------------------------------------------------------------
void ngap_msg::setAmfUeNgapId(unsigned long id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);
}

//------------------------------------------------------------------------------
void ngap_msg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);
}

//------------------------------------------------------------------------------
unsigned long ngap_msg::getAmfUeNgapId() {
  return amfUeNgapId.getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t ngap_msg::getRanUeNgapId() {
  return ranUeNgapId.getRanUeNgapId();
}

}  // namespace ngap
