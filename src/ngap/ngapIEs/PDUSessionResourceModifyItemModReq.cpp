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

#include "PDUSessionResourceModifyItemModReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceModifyItemModReq::PDUSessionResourceModifyItemModReq() {
  nas_pdu_ = std::nullopt;
  s_nssai_ = std::nullopt;
}

//------------------------------------------------------------------------------
PDUSessionResourceModifyItemModReq::~PDUSessionResourceModifyItemModReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceModifyItemModReq::set(
    const PDUSessionID& pdu_session_id, const std::optional<NAS_PDU>& nas_pdu,
    const OCTET_STRING_t& pdu_session_resource_modify_request_transfer,
    const std::optional<S_NSSAI>& s_nssai) {
  pdu_session_id_ = pdu_session_id;
  if (nas_pdu.has_value()) {
    uint8_t* nas_buf = nullptr;
    size_t nas_len   = 0;
    nas_pdu.value().get(nas_buf, nas_len);
    NAS_PDU tmp = {};
    tmp.set(nas_buf, nas_len);
    nas_pdu_ = std::optional<NAS_PDU>(tmp);
  }

  pdu_session_resource_modify_request_transfer_ =
      pdu_session_resource_modify_request_transfer;
  s_nssai_ = s_nssai;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceModifyItemModReq::encode(
    Ngap_PDUSessionResourceModifyItemModReq_t&
        pduSessionResourceModifyItemModReq) {
  if (!pdu_session_id_.encode(pduSessionResourceModifyItemModReq.pDUSessionID))
    return false;
  if (nas_pdu_.has_value()) {
    pduSessionResourceModifyItemModReq.nAS_PDU =
        (Ngap_NAS_PDU_t*) calloc(1, sizeof(Ngap_NAS_PDU_t));
    if (!pduSessionResourceModifyItemModReq.nAS_PDU) return false;
    if (!nas_pdu_.value().encode(*pduSessionResourceModifyItemModReq.nAS_PDU)) {
      if (pduSessionResourceModifyItemModReq.nAS_PDU != nullptr)
        free(pduSessionResourceModifyItemModReq.nAS_PDU);
      return false;
    }
  }

  pduSessionResourceModifyItemModReq.pDUSessionResourceModifyRequestTransfer =
      pdu_session_resource_modify_request_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceModifyItemModReq::decode(
    Ngap_PDUSessionResourceModifyItemModReq_t&
        pduSessionResourceModifyItemModReq) {
  if (!pdu_session_id_.decode(pduSessionResourceModifyItemModReq.pDUSessionID))
    return false;

  if (pduSessionResourceModifyItemModReq.nAS_PDU) {
    NAS_PDU tmp = {};
    if (!tmp.decode(*pduSessionResourceModifyItemModReq.nAS_PDU)) return false;
    nas_pdu_ = std::optional<NAS_PDU>(tmp);
  }

  pdu_session_resource_modify_request_transfer_ =
      pduSessionResourceModifyItemModReq
          .pDUSessionResourceModifyRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceModifyItemModReq::get(
    PDUSessionID& pdu_session_id, std::optional<NAS_PDU>& nas_pdu,
    OCTET_STRING_t& pdu_session_resource_modify_request_transfer,
    std::optional<S_NSSAI>& s_nssai) {
  pdu_session_id = pdu_session_id_;
  nas_pdu        = *nas_pdu_;
  if (nas_pdu_.has_value()) {
    NAS_PDU tmp      = {};
    uint8_t* nas_buf = nullptr;
    size_t nas_len   = 0;
    nas_pdu_.value().get(nas_buf, nas_len);
    tmp.set(nas_buf, nas_len);
    nas_pdu = std::optional<NAS_PDU>(tmp);
  }

  pdu_session_resource_modify_request_transfer =
      pdu_session_resource_modify_request_transfer_;
  s_nssai = s_nssai_;
}

}  // namespace ngap
