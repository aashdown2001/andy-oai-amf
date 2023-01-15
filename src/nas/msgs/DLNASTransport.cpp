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

#include "DLNASTransport.hpp"

#include "3gpp_24.501.hpp"
#include "bstrlib.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
DLNASTransport::DLNASTransport()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, DL_NAS_TRANSPORT) {
  ie_pdu_session_identity_2 = std::nullopt;
  ie_additional_information = std::nullopt;
  ie_5gmm_cause             = std::nullopt;
  ie_back_off_timer_value   = std::nullopt;
}

//------------------------------------------------------------------------------
DLNASTransport::~DLNASTransport() {}

//------------------------------------------------------------------------------
void DLNASTransport::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetPayloadContainerType(uint8_t value) {
  ie_payload_container_type.SetValue(value);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetPayloadContainer(
    std::vector<PayloadContainerEntry> content) {
  ie_payload_container.SetValue(content);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetPayloadContainer(uint8_t* buf, int len) {
  bstring b = blk2bstr(buf, len);
  ie_payload_container.SetValue(b);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetPduSessionId(uint8_t value) {
  ie_pdu_session_identity_2 =
      std::make_optional<PduSessionIdentity2>(0x12, value);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetAdditionalInformation(const bstring& value) {
  ie_additional_information = std::make_optional<AdditionalInformation>(value);
}

//------------------------------------------------------------------------------
void DLNASTransport::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause = std::make_optional<_5gmmCause>(0x58, value);
}

//------------------------------------------------------------------------------
void DLNASTransport::SetBackOffTimerValue(uint8_t unit, uint8_t value) {
  ie_back_off_timer_value = std::make_optional<GprsTimer3>(0x37, unit, value);
}

//------------------------------------------------------------------------------
int DLNASTransport::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DLNASTransport message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // Payload container type
  int size =
      ie_payload_container_type.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding ie_payload_container_type error");
    return KEncodeDecodeError;
  }
  if (size == 0)
    size++;  // 1/2 octet for  ie_payload_container_type, 1/2 octet for spare
  encoded_size += size;

  // Payload container
  size = ie_payload_container.Encode(
      buf + encoded_size, len - encoded_size,
      ie_payload_container_type.GetValue());

  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_payload_container error");
    return KEncodeDecodeError;
  }

  // PDU session ID
  if (!ie_pdu_session_identity_2.has_value()) {
    Logger::nas_mm().warn("IE ie_pdu_session_identity_2 is not available");
  } else {
    size = ie_pdu_session_identity_2->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_pdu_session_identity_2 error");
      return KEncodeDecodeError;
    }
  }

  // Additional information
  if (!ie_additional_information.has_value()) {
    Logger::nas_mm().warn("IE ie_additional_information is not available");
  } else {
    size = ie_additional_information->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_additional_information error");
      return KEncodeDecodeError;
    }
  }

  // 5GMM cause
  if (!ie_5gmm_cause.has_value()) {
    Logger::nas_mm().warn("IE ie_5gmm_cause is not available");
  } else {
    size = ie_5gmm_cause->Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_5gmm_cause error");
      return KEncodeDecodeError;
    }
  }

  // Back-off timer value
  if (!ie_back_off_timer_value.has_value()) {
    Logger::nas_mm().warn("IE ie_back_off_timer_value is not available");
  } else {
    size =
        ie_back_off_timer_value->Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_back_off_timer_value error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded DLNASTransport message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DLNASTransport::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DLNASTransport message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Payload container type
  decoded_size += ie_payload_container_type.Decode(
      buf + decoded_size, len - decoded_size, false);
  decoded_size++;  // 1/2 octet for PayloadContainerType, 1/2 octet for spare

  // Payload container
  decoded_size += ie_payload_container.Decode(
      buf + decoded_size, len - decoded_size, false,
      N1_SM_INFORMATION);  // TODO: verified Typeb of Payload Container
  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI (0x12)");
        PduSessionIdentity2 ie_pdu_session_identity_2_tmp = {};
        if ((decoded_result = ie_pdu_session_identity_2_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_pdu_session_identity_2 =
            std::optional<PduSessionIdentity2>(ie_pdu_session_identity_2_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiAdditionalInformation: {
        Logger::nas_mm().debug("Decoding IEI (0x24)");
        AdditionalInformation ie_additional_information_tmp = {};
        if ((decoded_result = ie_additional_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_additional_information =
            std::optional<AdditionalInformation>(ie_additional_information_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIei5gmmCause: {
        Logger::nas_mm().debug("Decoding IEI (0x58)");
        _5gmmCause ie_5gmm_cause_tmp = {};
        if ((decoded_result = ie_5gmm_cause_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_5gmm_cause = std::optional<_5gmmCause>(ie_5gmm_cause_tmp);
        octet         = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer3BackOffTimer: {
        Logger::nas_mm().debug("Decoding IEI (0x37)");
        GprsTimer3 ie_back_off_timer_value_tmp(kIeiGprsTimer3BackOffTimer);
        if ((decoded_result = ie_back_off_timer_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_back_off_timer_value =
            std::optional<GprsTimer3>(ie_back_off_timer_value_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded DLNASTransport message len (%d)", decoded_size);
  return decoded_size;
}
