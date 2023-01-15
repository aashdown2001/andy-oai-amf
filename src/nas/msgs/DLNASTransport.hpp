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

#ifndef _DL_NAS_TRANSPORT_H_
#define _DL_NAS_TRANSPORT_H_

#include "NasIeHeader.hpp"

namespace nas {

class DLNASTransport {
 public:
  DLNASTransport();
  ~DLNASTransport();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(NasMmPlainHeader* header, uint8_t* buf, int len);

  void SetPayloadContainerType(uint8_t value);

  void SetPayloadContainer(std::vector<PayloadContainerEntry> content);
  void SetPayloadContainer(uint8_t* buf, int len);

  void SetPduSessionId(uint8_t value);
  void SetAdditionalInformation(const bstring& value);
  void Set5gmmCause(uint8_t value);
  void SetBackOffTimerValue(uint8_t unit, uint8_t value);

 public:
  NasMmPlainHeader* plain_header;
  PayloadContainerType* ie_payload_container_type;
  Payload_Container* ie_payload_container;
  PduSessionIdentity2* ie_pdu_session_identity_2;
  AdditionalInformation* ie_additional_information;
  _5GMM_Cause* ie_5gmm_cause;
  GprsTimer3* ie_back_off_timer_value;
};

}  // namespace nas

#endif
