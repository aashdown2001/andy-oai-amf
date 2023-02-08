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

#ifndef _SERVICE_ACCEPT_H_
#define _SERVICE_ACCEPT_H_

#include <stdint.h>

#include "NasIeHeader.hpp"

namespace nas {

class ServiceAccept : public NasMmPlainHeader {
 public:
  ServiceAccept();
  ~ServiceAccept();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetPduSessionStatus(uint16_t value);
  // TODO: Get

  void SetPduSessionReactivationResult(uint16_t);
  // TODO: Get

  void SetPduSessionReactivationResultErrorCause(
      uint8_t session_id, uint8_t value);
  // TODO: Get

  void SetEapMessage(const bstring& eap);
  // TODO: Get

  void SetT3448Value(uint8_t unit, uint8_t value);
  // TODO: Get

 private:
  std::optional<PDUSessionStatus> ie_pdu_session_status;  // Optional
  std::optional<PDU_Session_Reactivation_Result>
      ie_pdu_session_reactivation_result;  // Optional
  std::optional<PDU_Session_Reactivation_Result_Error_Cause>
      ie_pdu_session_reactivation_result_error_cause;  // Optional
  std::optional<EapMessage> ie_eap_message;            // Optional
  std::optional<GprsTimer3> ie_t3448_value;            // Optional
};

}  // namespace nas

#endif
