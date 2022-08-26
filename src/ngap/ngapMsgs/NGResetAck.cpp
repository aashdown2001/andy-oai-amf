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

#include "NGResetAck.hpp"
#include "logger.hpp"

#include <vector>

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
NGResetAckMsg::NGResetAckMsg() {
  ngResetAckIEs                       = nullptr;
  ueAssociatedLogicalNGConnectionList = nullptr;
  CriticalityDiagnostics              = nullptr;
  NgapMessage::setMessageType(NgapMessageType::NG_RESET_ACKNOWLEDGE);
  initialize();
}
//------------------------------------------------------------------------------
NGResetAckMsg::~NGResetAckMsg() {
  if (ueAssociatedLogicalNGConnectionList)
    free_wrapper((void**) &ueAssociatedLogicalNGConnectionList);
  if (CriticalityDiagnostics) free_wrapper((void**) &CriticalityDiagnostics);
}

//------------------------------------------------------------------------------
void NGResetAckMsg::initialize() {
  ngResetAckIEs =
      &(ngapPdu->choice.successfulOutcome->value.choice.NGResetAcknowledge);
}

//------------------------------------------------------------------------------
void NGResetAckMsg::setUE_associatedLogicalNG_connectionList(
    std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  if (!ueAssociatedLogicalNGConnectionList) {
    ueAssociatedLogicalNGConnectionList =
        (UEAssociatedLogicalNGConnectionList*) calloc(
            1, sizeof(UEAssociatedLogicalNGConnectionList));
  }
  ueAssociatedLogicalNGConnectionList->setUEAssociatedLogicalNGConnectionItem(
      list);
  addUE_associatedLogicalNG_connectionList();
}

//------------------------------------------------------------------------------
void NGResetAckMsg::getUE_associatedLogicalNG_connectionList(
    std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  if (ueAssociatedLogicalNGConnectionList) {
    ueAssociatedLogicalNGConnectionList->getUEAssociatedLogicalNGConnectionItem(
        list);
  }
}

//------------------------------------------------------------------------------
void NGResetAckMsg::addUE_associatedLogicalNG_connectionList() {
  Ngap_NGResetAcknowledgeIEs_t* ie = (Ngap_NGResetAcknowledgeIEs_t*) calloc(
      1, sizeof(Ngap_NGResetAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList;

  if (!ueAssociatedLogicalNGConnectionList->encode(
          &ie->value.choice.UE_associatedLogicalNG_connectionList)) {
    Logger::ngap().error(
        "Encode NGAP UE_associatedLogicalNG_connectionList IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngResetAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP UE_associatedLogicalNG_connectionList IE error");
}

//------------------------------------------------------------------------------
bool NGResetAckMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGReset &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGResetAcknowledge) {
      ngResetAckIEs =
          &ngapPdu->choice.successfulOutcome->value.choice.NGResetAcknowledge;
      for (int i = 0; i < ngResetAckIEs->protocolIEs.list.count; i++) {
        switch (ngResetAckIEs->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList: {
            if (ngResetAckIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngResetAckIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList) {
              ueAssociatedLogicalNGConnectionList =
                  new UEAssociatedLogicalNGConnectionList();
              if (!ueAssociatedLogicalNGConnectionList->decode(
                      &ngResetAckIEs->protocolIEs.list.array[i]
                           ->value.choice
                           .UE_associatedLogicalNG_connectionList)) {
                Logger::ngap().error(
                    "Decoded NGAP UE_associatedLogicalNG_connectionList IE "
                    "error");
                return false;
              }

            } else {
              Logger::ngap().error(
                  "Decoded NGAP UE_associatedLogicalNG_connectionList IE "
                  "error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
            // TODO:
          } break;
          default: {
            Logger::ngap().error(
                "Decoded NGAP NGResetAck message PDU IE error");
            return false;
          }
        }
      }
    } else {
      Logger::ngap().error("Check NGResetAck message error!");
      return false;
    }
  } else {
    Logger::ngap().error("Check NGResetAck message error!");
    return false;
  }
  return true;
}

}  // namespace ngap
