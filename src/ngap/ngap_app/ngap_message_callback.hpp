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

#ifndef _NGAP_MESSAGE_CALLBACK_H_
#define _NGAP_MESSAGE_CALLBACK_H_

#include "InitialContextSetupResponse.hpp"
#include "NGReset.hpp"
#include "NGResetAck.hpp"
#include "NGSetupRequest.hpp"
#include "PDUSessionResourceSetupUnsuccessfulTransfer.hpp"
#include "PduSessionResourceReleaseResponse.hpp"
#include "PduSessionResourceSetupResponse.hpp"
#include "PduSessionResourceModifyResponse.hpp"
#include "amf_app.hpp"
#include "amf_n1.hpp"
#include "itti_msg_sbi.hpp"
#include "itti_msg_n2.hpp"
#include "logger.hpp"
#include "nas_context.hpp"
#include "pdu_session_context.hpp"

using namespace sctp;
using namespace ngap;
using namespace amf_application;

extern itti_mw* itti_inst;
extern amf_n1* amf_n1_inst;
extern amf_app* amf_app_inst;

typedef int (*ngap_message_decoded_callback)(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

typedef void (*ngap_event_callback)(const sctp_assoc_id_t assoc_id);

constexpr uint8_t NGAP_PROCEDURE_CODE_MAX_VALUE = 55;
constexpr uint8_t NGAP_PRESENT_MAX_VALUE        = 3;

//------------------------------------------------------------------------------
int ngap_amf_handle_ng_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI NG Setup Request message to TASK_AMF_N2");
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  NGSetupRequestMsg* ngSetupReq = new NGSetupRequestMsg();
  if (!ngSetupReq->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding NGSetupRequest message error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_ng_setup_request>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id   = assoc_id;
  itti_msg->stream     = stream;
  itti_msg->ngSetupReq = ngSetupReq;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_ng_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling NG Setup Response (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_ng_setup_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling NG Setup Failure (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_ue_message(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Initial UE Message to TASK_AMF_N2");

  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  InitialUEMessageMsg* initUeMsg = new InitialUEMessageMsg();
  if (!initUeMsg->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding InitialUEMessage error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_initial_ue_message>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id  = assoc_id;
  itti_msg->stream    = stream;
  itti_msg->initUeMsg = initUeMsg;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_uplink_nas_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Uplink NAS Transport message to TASK_AMF_N2");
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  UplinkNASTransportMsg* uplinkNasT = new UplinkNASTransportMsg();
  if (!uplinkNasT->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding UplinkNasTransport message error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_ul_nas_transport>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id = assoc_id;
  itti_msg->stream   = stream;
  itti_msg->ulNas    = uplinkNasT;
  int ret            = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_initial_context_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Initial Context Setup Request (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_context_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Initial Context Setup Response");

  InitialContextSetupResponseMsg* initCtxResp =
      new InitialContextSetupResponseMsg();
  if (!initCtxResp->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding InitialContextSetupResponse message error");
    return RETURNerror;
  }

  std::vector<PDUSessionResourceSetupResponseItem_t> list;
  if (!initCtxResp->getPduSessionResourceSetupResponseList(list)) {
    Logger::ngap().debug(
        "Decode PduSessionResourceSetupResponseList IE error or this IE is not "
        "available");
    return 0;
  }

  uint8_t transferIe[BUFFER_SIZE_512];
  memcpy(
      transferIe, list[0].pduSessionResourceSetupResponseTransfer.buf,
      list[0].pduSessionResourceSetupResponseTransfer.size);
  bstring n2sm = blk2bstr(
      transferIe, list[0].pduSessionResourceSetupResponseTransfer.size);

  Logger::ngap().debug(
      "Sending ITTI Initial Context Setup Response to TASK_AMF_SBI");
  auto itti_msg = std::make_shared<itti_nsmf_pdusession_update_sm_context>(
      TASK_NGAP, TASK_AMF_SBI);
  itti_msg->pdu_session_id = list[0].pduSessionId;
  itti_msg->n2sm           = n2sm;
  itti_msg->is_n2sm_set    = true;
  itti_msg->n2sm_info_type = "PDU_RES_SETUP_RSP";
  itti_msg->amf_ue_ngap_id = initCtxResp->getAmfUeNgapId();
  itti_msg->ran_ue_ngap_id = initCtxResp->getRanUeNgapId();

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_context_setup_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Initial Context Setup Failure to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_radio_cap_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Radio Capability Indication to TASK_AMF_N2");

  UeRadioCapabilityInfoIndicationMsg* ueRadioCap =
      new UeRadioCapabilityInfoIndicationMsg();
  if (!ueRadioCap->decodeFromPdu(message_p)) {
    Logger::ngap().error(
        "Decoding UE Radio Capability Indication message error");
    return RETURNerror;
  }

  auto itti_msg = std::make_shared<itti_ue_radio_capability_indication>(
      TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id   = assoc_id;
  itti_msg->stream     = stream;
  itti_msg->ueRadioCap = ueRadioCap;
  int ret              = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_context_release_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Context Release Request to TASK_AMF_N2");
  UEContextReleaseRequestMsg* ueCtxRelReq = new UEContextReleaseRequestMsg();
  if (!ueCtxRelReq->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding UEContextReleaseRequest message error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_ue_context_release_request>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id = assoc_id;
  itti_msg->stream   = stream;
  itti_msg->ueCtxRel = ueCtxRelReq;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_ue_context_release_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling UE Context Release Command (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_context_release_complete(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Context Release Complete to TASK_AMF_N2");

  UEContextReleaseCompleteMsg* ueCtxRelCmpl = new UEContextReleaseCompleteMsg();
  if (!ueCtxRelCmpl->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding UEContextReleaseComplete message error");
    return RETURNerror;
  }

  auto itti_msg = std::make_shared<itti_ue_context_release_complete>(
      TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id     = assoc_id;
  itti_msg->stream       = stream;
  itti_msg->ueCtxRelCmpl = ueCtxRelCmpl;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_release_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Handling PDU Session Resource Release Command (AMF->AN)");
  // TODO
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_release_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handle PDU Session Resource Release Response");

  PduSessionResourceReleaseResponseMsg* pduresp =
      new PduSessionResourceReleaseResponseMsg();
  if (!pduresp->decodeFromPdu(message_p)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceReleaseResponseMsg message error");
    return RETURNerror;
  }
  // TODO: process User Location Information if this IE is available

  // Transfer pduSessionResourceReleaseResponseTransfer to SMF
  std::vector<PDUSessionResourceReleasedItem_t> list;
  if (!pduresp->getPduSessionResourceReleasedList(list)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceReleaseResponseMsg "
        "getPduSessionResourceReleasedList IE error");
    return RETURNerror;
  }

  // TODO: add the full list
  uint8_t transferIe[BUFFER_SIZE_512];
  memcpy(
      transferIe, list[0].pduSessionResourceReleaseResponseTransfer.buf,
      list[0].pduSessionResourceReleaseResponseTransfer.size);
  bstring n2sm = blk2bstr(
      transferIe, list[0].pduSessionResourceReleaseResponseTransfer.size);
  Logger::ngap().debug(
      "Sending ITTI PDUSessionResourceReleaseResponse to TASK_AMF_SBI");

  auto itti_msg = std::make_shared<itti_nsmf_pdusession_update_sm_context>(
      TASK_NGAP, TASK_AMF_SBI);

  itti_msg->pdu_session_id = list[0].pduSessionId;
  itti_msg->n2sm           = n2sm;
  itti_msg->is_n2sm_set    = true;
  itti_msg->n2sm_info_type = "PDU_RES_REL_RSP";
  itti_msg->amf_ue_ngap_id = pduresp->getAmfUeNgapId();
  itti_msg->ran_ue_ngap_id = pduresp->getRanUeNgapId();

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling PDU Session Resource Setup Request");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handle PDU Session Resource Setup Response");

  PduSessionResourceSetupResponseMsg* pduresp =
      new PduSessionResourceSetupResponseMsg();
  if (!pduresp->decodeFromPdu(message_p)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceSetupResponseMsg message error");
    return RETURNerror;
  }

  std::vector<PDUSessionResourceSetupResponseItem_t> list;
  if (!pduresp->getPduSessionResourceSetupResponseList(list)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceSetupResponseMsg "
        "getPduSessionResourceSetupResponseList IE error");
    // return RETURNerror;
  } else {
    // TODO: for multiple PDU Sessions
    uint8_t transferIe[BUFFER_SIZE_512];
    memcpy(
        transferIe, list[0].pduSessionResourceSetupResponseTransfer.buf,
        list[0].pduSessionResourceSetupResponseTransfer.size);
    bstring n2sm = blk2bstr(
        transferIe, list[0].pduSessionResourceSetupResponseTransfer.size);
    itti_nsmf_pdusession_update_sm_context* itti_msg =
        new itti_nsmf_pdusession_update_sm_context(TASK_NGAP, TASK_AMF_SBI);
    long amf_ue_ngap_id              = pduresp->getAmfUeNgapId();
    std::shared_ptr<nas_context> nct = {};
    if (!amf_n1_inst->is_amf_ue_id_2_nas_context(amf_ue_ngap_id, nct)) {
      Logger::ngap().error(
          "No UE NAS context with amf_ue_ngap_id (0x%x)", amf_ue_ngap_id);
      return RETURNerror;
    }
    itti_msg->supi           = "imsi-" + nct.get()->imsi;
    itti_msg->pdu_session_id = list[0].pduSessionId;
    itti_msg->n2sm           = n2sm;
    itti_msg->is_n2sm_set    = true;
    itti_msg->n2sm_info_type = "PDU_RES_SETUP_RSP";
    itti_msg->amf_ue_ngap_id = pduresp->getAmfUeNgapId();
    itti_msg->ran_ue_ngap_id = pduresp->getRanUeNgapId();

    std::shared_ptr<itti_nsmf_pdusession_update_sm_context> i =
        std::shared_ptr<itti_nsmf_pdusession_update_sm_context>(itti_msg);

    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::ngap().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          i->get_msg_name());
    }
    return 0;
  }

  // TTN: Should be removed
  std::vector<PDUSessionResourceFailedToSetupItem_t> list_fail;
  if (!pduresp->getPduSessionResourceFailedToSetupList(list_fail)) {
    Logger::ngap().error(
        "decoding PduSessionResourceSetupResponseMsg "
        "getPduSessionResourceFailedToSetupList IE  error");
  } else {
    PduSessionResourceSetupUnSuccessfulTransferIE* UnSuccessfultransfer =
        new PduSessionResourceSetupUnSuccessfulTransferIE();
    uint8_t buffer[BUFFER_SIZE_512];
    memcpy(
        buffer, list_fail[0].pduSessionResourceSetupUnsuccessfulTransfer.buf,
        list_fail[0].pduSessionResourceSetupUnsuccessfulTransfer.size);
    UnSuccessfultransfer->decodefromIE(
        buffer, list_fail[0].pduSessionResourceSetupUnsuccessfulTransfer.size);
    Logger::ngap().debug(
        "UnSuccessfultransfer->getChoiceOfCause%d      "
        "UnSuccessfultransfer->getCause%d",
        UnSuccessfultransfer->getChoiceOfCause(),
        UnSuccessfultransfer->getCause());
    if ((UnSuccessfultransfer->getChoiceOfCause() ==
         Ngap_Cause_PR_radioNetwork) &&
        (UnSuccessfultransfer->getCause() ==
         Ngap_CauseRadioNetwork_multiple_PDU_session_ID_instances)) {
      // TODO:
      /*Logger::ngap().debug("Sending itti pdu session resource release command
       to TASK_AMF_N2"); itti_pdu_session_resource_release_command * itti_msg =
       new itti_pdu_session_resource_release_command(TASK_NGAP, TASK_AMF_N2);
       itti_msg->amf_ue_ngap_id = pduresp->getAmfUeNgapId();
       itti_msg->ran_ue_ngap_id = pduresp->getRanUeNgapId();
       std::shared_ptr<itti_pdu_session_resource_release_command> i =
       std::shared_ptr<itti_pdu_session_resource_release_command>(itti_msg); int
       ret = itti_inst->send_msg(i); if (0 != ret) { Logger::ngap().error("Could
       not send ITTI message %s to task TASK_AMF_N2", i->get_msg_name());
       }*/
      long amf_ue_ngap_id = pduresp->getAmfUeNgapId();

      std::shared_ptr<nas_context> nct = {};
      if (!amf_n1_inst->is_amf_ue_id_2_nas_context(amf_ue_ngap_id, nct)) {
        Logger::ngap().error(
            "No UE NAS context with amf_ue_ngap_id (0x%x)", amf_ue_ngap_id);
        return RETURNerror;
      }

      string supi                              = "imsi-" + nct.get()->imsi;
      std::shared_ptr<pdu_session_context> psc = {};
      if (amf_app_inst->find_pdu_session_context(
              supi, list_fail[0].pduSessionId, psc)) {
        if (psc.get() == nullptr) {
          Logger::ngap().error("Cannot get pdu_session_context");
          return RETURNerror;
        }
      }
      psc.get()->isn2sm_avaliable = false;
      Logger::ngap().debug(
          "Receive pdu session resource setup response fail (multi pdu session "
          "id),set pdu session context isn2sm_avaliable = false");
      // TODO:
      /*Logger::ngap().debug("Sending itti ue context release command to
       TASK_AMF_N2"); itti_ue_context_release_command * itti_msg = new
       itti_ue_context_release_command(TASK_AMF_N1, TASK_AMF_N2);
       itti_msg->amf_ue_ngap_id = pduresp->getAmfUeNgapId();
       itti_msg->ran_ue_ngap_id = pduresp->getRanUeNgapId();
       itti_msg->cause.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
       itti_msg->cause.setValue(28);
       std::shared_ptr<itti_ue_context_release_command> i =
       std::shared_ptr<itti_ue_context_release_command>(itti_msg); int ret =
       itti_inst->send_msg(i); if (0 != ret) { Logger::ngap().error("Could not
       send ITTI message %s to task TASK_AMF_N2", i->get_msg_name());
       }*/
      return 0;
    }
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_modify_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling PDU Session Resource Modify Request");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_modify_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handle PDU Session Resource Modify Response");

  std::unique_ptr<PduSessionResourceModifyResponseMsg> response_msg =
      std::make_unique<PduSessionResourceModifyResponseMsg>();

  if (!response_msg->decodeFromPdu(message_p)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceModifyResponseMsg message error");
    return RETURNerror;
  }

  // Transfer pduSessionResourceModifyResponseTransfer to SMF
  std::vector<PDUSessionResourceModifyResponseItem_t> list;
  if (!response_msg->getPduSessionResourceModifyResponseList(list)) {
    Logger::ngap().error(
        "Decoding PduSessionResourceModifyResponseMsg "
        "getPduSessionResourceModifyResponseList IE error");
    return RETURNerror;
  }

  for (auto response_item : list) {
    uint8_t buf[BUFFER_SIZE_512];
    memcpy(
        buf, response_item.pduSessionResourceModifyResponseTransfer.buf,
        response_item.pduSessionResourceModifyResponseTransfer.size);
    bstring n2sm = blk2bstr(
        buf, response_item.pduSessionResourceModifyResponseTransfer.size);
    Logger::ngap().debug(
        "Sending ITTI PDUSessionResourceModifyResponseTransfer to "
        "TASK_AMF_SBI");

    auto itti_msg = std::make_shared<itti_nsmf_pdusession_update_sm_context>(
        TASK_NGAP, TASK_AMF_SBI);
    itti_msg->pdu_session_id = response_item.pduSessionId;
    itti_msg->n2sm           = n2sm;
    itti_msg->is_n2sm_set    = true;
    itti_msg->n2sm_info_type = "PDU_RES_MOD_RSP";
    itti_msg->amf_ue_ngap_id = response_msg->getAmfUeNgapId();
    itti_msg->ran_ue_ngap_id = response_msg->getRanUeNgapId();

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::ngap().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }

  // TODO:for PDUSessionResourceFailedToModifyListModRes
  // TODO: process User Location Information if available
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_error_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI NGAP Error Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_configuration_update(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling AMF Configuration Update");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_configuration_update_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI AMF Configuration Update ACK to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_configuration_update_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI AMF Configuration Update Failure to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int amf_status_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI AMF Status Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int cell_traffic_trace(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Cell Traffic Trace to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int deactivate_trace(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Deactivate Trace to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int downlink_nas_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Downlink NAS Transport to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int downlink_non_UEassociated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Downlink Non UEAssociated NRPPA Transport to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int downlink_ran_configuration_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Downlink RAN Configuration Transfer to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int downlink_ran_status_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Downlink RAN Status Transfer to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int downlink_ue_associated_nappa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Downlink UE Associated NAPPA Transport to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_cancel(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Handover Cancel to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_cancel_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Handover Cancel Ack (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_preparation(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Handover Preparation to TASK_AMF_N2");

  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  HandoverRequiredMsg* handover_required = new HandoverRequiredMsg();
  if (!handover_required->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding HandoverRequired message error");
    return RETURNerror;
  }
  auto itti_msg =
      std::make_shared<itti_handover_required>(TASK_NGAP, TASK_AMF_N2);

  itti_msg->assoc_id    = assoc_id;
  itti_msg->stream      = stream;
  itti_msg->handoverReq = handover_required;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Handover Command (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_preparation_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Handover Preparation failure (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_notification(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Handover Notification to TASK_AMF_N2");
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  HandoverNotifyMsg* handoverNotify = new HandoverNotifyMsg();
  if (!handoverNotify->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding HandoverNotify message error");
    return RETURNerror;
  }
  auto itti_msg =
      std::make_shared<itti_handover_notify>(TASK_NGAP, TASK_AMF_N2);

  itti_msg->assoc_id       = assoc_id;
  itti_msg->stream         = stream;
  itti_msg->handovernotify = handoverNotify;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Handover Resource Allocation (HandoverRequest) to "
      "TASK_AMF_N2");

  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  HandoverRequestAck* handoverRequestAck = new HandoverRequestAck();
  if (!handoverRequestAck->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding Handover Request Acknowledge message error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_handover_request_Ack>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id           = assoc_id;
  itti_msg->stream             = stream;
  itti_msg->handoverrequestAck = handoverRequestAck;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_request_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Handover Request Ack (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int handover_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Handover Failure (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int location_reporting_control(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Location Reporting Control to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int location_reporting_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Location Reporting Failure Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int location_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Location Report to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int nas_non_delivery_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI NAS Non Delivery Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ng_reset(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI NG Reset to TASK_AMF_N2");

  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  NGResetMsg* ngReset = new NGResetMsg();
  if (!ngReset->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding NGReset message error");
    return RETURNerror;
  }

  auto itti_msg      = std::make_shared<itti_ng_reset>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id = assoc_id;
  itti_msg->stream   = stream;
  itti_msg->ngReset  = ngReset;
  int ret            = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
int overload_start(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Overload Start to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int overload_stop(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Overload Stop to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int paging(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Paging to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_amf_handle_path_switch_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Path Switch Request to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_path_switch_request_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Path Switch Request Ack (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ngap_handle_path_switch_request_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Path Switch Request Failure (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pdu_session_resource_modify_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI PDU Session Resource Modify Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pdu_session_resource_modify_confirm(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Handling PDU Session Resource Modify Confirm (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pdu_session_resource_notify(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI PDU Session Resource Notify to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int private_message(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Private Message to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pws_cancel_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling PWS Cancel Request (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pws_cancel_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI PWS Cancel Response to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pws_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI PWS Failure Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int pws_restart_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI PWS Restart Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ran_configuration_update(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI RAN Configuration Update to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ran_configuration_update_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling RAN Configuration Update Ack (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ran_configuration_update_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling RAN Configuration Update Failure (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int reroute_nas_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Reroute NAS Request to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int rrc_inactive_transition_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI RRC Inactive Transition Report to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int trace_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Trace Failure Indication to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int trace_start(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI Trace Start to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_context_modification_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling UE Context Modification Request (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_context_modification_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Context Modification Response to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_context_modification_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Context Modification Failure to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_radio_capability_check_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling UE Radio Capability Check Request (AMF->AN)");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_radio_capability_check_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI UE Radio Capability Check Response to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int ue_tnla_binding_release(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Sending ITTI UE TNLA Binding Release to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int uplink_non_ue_associated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Uplink Non UE Associated NRPPA Transport to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int uplink_ran_configuration_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Uplink RAN Configuration Transfer to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int uplink_ran_status_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Uplink RAN Status Transfer to TASK_AMF_N2");

  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, message_p);
  UplinkRANStatusTransfer* Uplinkranstatustransfer =
      new UplinkRANStatusTransfer();
  if (!Uplinkranstatustransfer->decodeFromPdu(message_p)) {
    Logger::ngap().error("Decoding Uplink RAN Status Transfer message error");
    return RETURNerror;
  }

  auto itti_msg =
      std::make_shared<itti_uplink_ran_status_transfer>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id          = assoc_id;
  itti_msg->stream            = stream;
  itti_msg->uplinkrantransfer = Uplinkranstatustransfer;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int uplink_ue_associated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug(
      "Sending ITTI Uplink UE Associated NRPPA Transport to TASK_AMF_N2");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
int secondary_rat_data_usage_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p) {
  Logger::ngap().debug("Handling Secondary RAT Data Usage Report");
  // TODO:
  return RETURNok;
}

//------------------------------------------------------------------------------
ngap_message_decoded_callback
    messages_callback[NGAP_PROCEDURE_CODE_MAX_VALUE][NGAP_PRESENT_MAX_VALUE] = {
        {ngap_amf_configuration_update, ngap_amf_configuration_update_ack,
         ngap_amf_configuration_update_failure}, /*0 AMFConfigurationUpdate*/
        {amf_status_indication, 0, 0},           /*1 AMFStatusIndication*/
        {cell_traffic_trace, 0, 0},              /*2 CellTrafficTrace*/
        {deactivate_trace, 0, 0},                /*3 DeactivateTrace*/
        {downlink_nas_transport, 0, 0},          /*4 DownlinkNASTransport*/
        {downlink_non_UEassociated_nrppa_transport, 0,
         0}, /*5 DownlinkNonUEAssociatedNRPPaTransport*/
        {downlink_ran_configuration_transfer, 0,
         0}, /*6 DownlinkRANConfigurationTransfer*/
        {downlink_ran_status_transfer, 0, 0}, /*7 DownlinkRANStatusTransfer*/
        {downlink_ue_associated_nappa_transport, 0,
         0}, /*8 DownlinkUEAssociatedNRPPaTransport*/
        {ngap_amf_handle_error_indication, 0, 0},  /*9 ErrorIndication*/
        {handover_cancel, handover_cancel_ack, 0}, /*10 HandoverCancel*/
        {handover_notification, 0, 0},             /*11 HandoverNotification*/
        {handover_preparation, handover_command,
         handover_preparation_failure}, /*12 HandoverPreparation*/
        {handover_request, handover_request_ack,
         handover_failure}, /*13 HandoverResourceAllocation*/
        {ngap_handle_initial_context_setup_request,
         ngap_amf_handle_initial_context_setup_response,
         ngap_amf_handle_initial_context_setup_failure}, /*14
                                                            InitialContextSetup*/
        {ngap_amf_handle_initial_ue_message, 0, 0},      /*15 InitialUEMessage*/
        {location_reporting_control, 0, 0}, /*16 LocationReportingControl*/
        {location_reporting_failure_indication, 0,
         0},                     /*17 LocationReportingFailureIndication*/
        {location_report, 0, 0}, /*18 LocationReport*/
        {nas_non_delivery_indication, 0, 0}, /*19 NASNonDeliveryIndication*/
        {ng_reset, 0, 0},                    /*20 NGReset*/
        {ngap_amf_handle_ng_setup_request, ngap_handle_ng_setup_response,
         ngap_handle_ng_setup_failure}, /*21 NGSetup*/
        {overload_start, 0, 0},         /*22 OverloadStart*/
        {overload_stop, 0, 0},          /*23 OverloadStop*/
        {paging, 0, 0},                 /*24 Paging*/
        {ngap_amf_handle_path_switch_request,
         ngap_handle_path_switch_request_ack,
         ngap_handle_path_switch_request_failure}, /*25 PathSwitchRequest*/
        {ngap_handle_pdu_session_resource_modify_request,
         ngap_amf_handle_pdu_session_resource_modify_response,
         0}, /*26 PDUSessionResourceModify*/
        {pdu_session_resource_modify_indication,
         pdu_session_resource_modify_confirm,
         0}, /*27 PDUSessionResourceModifyIndication*/
        {ngap_handle_pdu_session_resource_release_command,
         ngap_amf_handle_pdu_session_resource_release_response,
         0}, /*28 PDUSessionResourceRelease*/
        {ngap_handle_pdu_session_resource_setup_request,
         ngap_amf_handle_pdu_session_resource_setup_response,
         0},                                 /*29 PDUSessionResourceSetup*/
        {pdu_session_resource_notify, 0, 0}, /*30 PDUSessionResourceNotify*/
        {private_message, 0, 0},             /*31 PrivateMessage*/
        {pws_cancel_request, pws_cancel_response, 0}, /*32 PWSCancel*/
        {pws_failure_indication, 0, 0}, /*33 PWSFailureIndication*/
        {pws_restart_indication, 0, 0}, /*34 PWSRestartIndication*/
        {ran_configuration_update, ran_configuration_update_ack,
         ran_configuration_update_failure}, /*35 RANConfigurationUpdate*/
        {reroute_nas_request, 0, 0},        /*36 RerouteNASRequest*/
        {rrc_inactive_transition_report, 0,
         0},                              /*37 RRCInactiveTransitionReport*/
        {trace_failure_indication, 0, 0}, /*38 TraceFailureIndication*/
        {trace_start, 0, 0},              /*39 TraceStart*/
        {ue_context_modification_request, ue_context_modification_response,
         ue_context_modification_failure}, /*40 UEContextModification*/
        {ngap_handle_ue_context_release_command,
         ngap_amf_handle_ue_context_release_complete,
         0}, /*41 UEContextRelease*/
        {ngap_amf_handle_ue_context_release_request, 0,
         0}, /*42 UEContextReleaseRequest*/
        {ue_radio_capability_check_request, ue_radio_capability_check_response,
         0}, /*43 UERadioCapabilityCheck*/
        {ngap_amf_handle_ue_radio_cap_indication, 0,
         0}, /*44 UERadioCapabilityInfoIndication*/
        {ue_tnla_binding_release, ue_tnla_binding_release,
         ue_tnla_binding_release}, /*45 UETNLABindingRelease*/
        {ngap_amf_handle_uplink_nas_transport, 0, 0}, /*46 UplinkNASTransport*/
        {uplink_non_ue_associated_nrppa_transport, 0,
         0}, /*47 UplinkNonUEAssociatedNRPPaTransport*/
        {uplink_ran_configuration_transfer, 0,
         0}, /*48 UplinkRANConfigurationTransfer*/
        {uplink_ran_status_transfer, 0, 0}, /*49 UplinkRANStatusTransfer*/
        {uplink_ue_associated_nrppa_transport, 0,
         0},       /*50 UplinkUEAssociatedNRPPaTransport*/
        {0, 0, 0}, /*51 WriteReplaceWarning*/
        {secondary_rat_data_usage_report, 0,
         0},       /*52 SecondaryRATDataUsageReport*/
        {0, 0, 0}, /*53 UplinkRIMInformationTransfer*/
        {0, 0, 0}  /*54 DownlinkRIMInformationTransfer*/
};

//------------------------------------------------------------------------------
void ngap_sctp_shutdown(const sctp_assoc_id_t assoc_id) {
  Logger::ngap().debug("Sending ITTI SCTP Shutdown event to TASK_AMF_N2");

  auto itti_msg = std::make_shared<itti_ng_shutdown>(TASK_NGAP, TASK_AMF_N2);
  itti_msg->assoc_id = assoc_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::ngap().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
  return;
}

//------------------------------------------------------------------------------
ngap_event_callback events_callback[][1] = {
    {ngap_sctp_shutdown},
    {0} /*reserved*/
};

#endif
