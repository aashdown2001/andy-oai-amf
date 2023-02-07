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

#include "ServiceRequest.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ServiceRequest::ServiceRequest()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SERVICE_REQUEST) {
  ie_uplink_data_status         = std::nullopt;
  ie_pdu_session_status         = std::nullopt;
  ie_allowed_pdu_session_status = std::nullopt;
  ie_nas_message_container      = std::nullopt;
}

//------------------------------------------------------------------------------
ServiceRequest::~ServiceRequest() {}

//------------------------------------------------------------------------------
void ServiceRequest::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void ServiceRequest::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ng_ksi.Set(false);  // 4 lower bits
  ie_ng_ksi.SetNasKeyIdentifier(key_set_id);
  ie_ng_ksi.SetTypeOfSecurityContext(tsc);
}

//------------------------------------------------------------------------------
void ServiceRequest::GetNgKsi(uint8_t& ng_ksi) const {
  ng_ksi =
      (ie_ng_ksi.GetTypeOfSecurityContext()) | ie_ng_ksi.GetNasKeyIdentifier();
}

//------------------------------------------------------------------------------
void ServiceRequest::SetServiceType(uint8_t value) {
  ie_service_type.Set(true, value);  // 4 higher bit
}

//------------------------------------------------------------------------------
void ServiceRequest::GetServiceType(uint8_t& value) {
  ie_service_type.GetValue(value);
}

//------------------------------------------------------------------------------
void ServiceRequest::Set5gSTmsi(
    uint16_t amf_set_id, uint8_t amf_pointer, std::string tmsi) {
  ie_5g_s_tmsi.Set5gSTmsi(amf_set_id, amf_pointer, tmsi);
}

//------------------------------------------------------------------------------
bool ServiceRequest::Get5gSTmsi(
    uint16_t& amf_set_id, uint8_t& amf_pointer, string& tmsi) const {
  return ie_5g_s_tmsi.Get5gSTmsi(amf_set_id, amf_pointer, tmsi);
}

//------------------------------------------------------------------------------
void ServiceRequest::SetUplinkDataStatus(uint16_t value) {
  ie_uplink_data_status = std::make_optional<UplinkDataStatus>(value);
}

//------------------------------------------------------------------------------
bool ServiceRequest::GetUplinkDataStatus(uint16_t& value) const {
  if (ie_uplink_data_status.has_value()) {
    value = ie_uplink_data_status.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void ServiceRequest::SetPduSessionStatus(uint16_t value) {
  ie_pdu_session_status = std::make_optional<PDUSessionStatus>(value);
}

//------------------------------------------------------------------------------
bool ServiceRequest::GetPduSessionStatus(uint16_t& value) const {
  if (ie_pdu_session_status.has_value()) {
    value = ie_pdu_session_status.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
std::optional<uint16_t> ServiceRequest::GetPduSessionStatus() const {
  if (ie_pdu_session_status.has_value()) {
    return std::optional<uint16_t>(ie_pdu_session_status.value().GetValue());
  }
  return std::nullopt;
}

//------------------------------------------------------------------------------
void ServiceRequest::SetAllowedPduSessionStatus(uint16_t value) {
  ie_allowed_pdu_session_status =
      std::make_optional<AllowedPDUSessionStatus>(value);
}

//------------------------------------------------------------------------------
bool ServiceRequest::GetAllowedPduSessionStatus(uint16_t& value) const {
  if (ie_allowed_pdu_session_status.has_value()) {
    value = ie_allowed_pdu_session_status.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
std::optional<uint16_t> ServiceRequest::GetAllowedPduSessionStatus() const {
  if (ie_allowed_pdu_session_status.has_value()) {
    return std::optional<uint16_t>(
        ie_allowed_pdu_session_status.value().GetValue());
  }
  return std::nullopt;
}

//------------------------------------------------------------------------------
void ServiceRequest::SetNasMessageContainer(bstring value) {
  ie_nas_message_container = std::make_optional<NasMessageContainer>(value);
}

//------------------------------------------------------------------------------
bool ServiceRequest::GetNasMessageContainer(bstring& nas) const {
  if (ie_nas_message_container.has_value()) {
    ie_nas_message_container.value().GetValue(nas);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
int ServiceRequest::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding ServiceRequest message...");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // ngKSI and Service Type
  int size = ie_ng_ksi.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NgKSI error");
    return KEncodeDecodeError;
  }
  if (size != 0) {
    Logger::nas_mm().error("Encoding NgKSI error");
    return KEncodeDecodeError;
  }

  size = ie_service_type.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size++;  // 1/2 octet for ngKSI, 1/2 for Service Type
  } else {
    Logger::nas_mm().error("Encoding ie_service_type error");
    return KEncodeDecodeError;
  }

  // 5G-S-TMSI
  size = ie_5g_s_tmsi.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding IE ie_5g_s_tmsi error");
    return KEncodeDecodeError;
  }

  // Uplink data status
  if (!ie_uplink_data_status.has_value()) {
    Logger::nas_mm().warn("IE ie_uplink_data_status is not available");
  } else {
    size = ie_uplink_data_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_uplink_data_status error");
      return KEncodeDecodeError;
    }
  }

  // PDU session status
  if (!ie_pdu_session_status.has_value()) {
    Logger::nas_mm().warn("IE ie_pdu_session_status is not available");
  } else {
    size = ie_pdu_session_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_pdu_session_status error");
      return KEncodeDecodeError;
    }
  }

  // Allowed PDU session status
  if (!ie_allowed_pdu_session_status.has_value()) {
    Logger::nas_mm().warn("IE ie_allowed_pdu_session_status is not available");
  } else {
    size = ie_allowed_pdu_session_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_allowed_pdu_session_status error");
      return KEncodeDecodeError;
    }
  }

  // NAS message container
  if (!ie_nas_message_container.has_value()) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    size = ie_nas_message_container.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nas_message_container error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug("Encoded ServiceRequest message (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ServiceRequest::Decode(NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ServiceRequest message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // ngKSI + Service type
  decoded_result =
      ie_ng_ksi.Decode(buf + decoded_size, len - decoded_size, false, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_result = ie_service_type.Decode(
      buf + decoded_size, len - decoded_size, true, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Service Type

  decoded_result =
      ie_5g_s_tmsi.Decode(buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;

  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First optional IE (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiUplinkDataStatus: {
        Logger::nas_mm().debug("Decoding ie_uplink_data_status (IEI: 0x40)");
        UplinkDataStatus ie_uplink_data_status_tmp = {};
        if ((decoded_result = ie_uplink_data_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_uplink_data_status =
            std::optional<UplinkDataStatus>(ie_uplink_data_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug(
            "Decoding ie_pdu_session_status (IEI 0x%x)", kIeiPduSessionStatus);
        PDUSessionStatus ie_pdu_session_status_tmp = {};
        if ((decoded_result = ie_pdu_session_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_pdu_session_status =
            std::optional<PDUSessionStatus>(ie_pdu_session_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAllowedPduSessionStatus: {
        Logger::nas_mm().debug(
            "Decoding ie_allowed_pdu_session_status, IEI 0x%x",
            kIeiAllowedPduSessionStatus);
        AllowedPDUSessionStatus ie_allowed_pdu_session_status_tmp = {};
        if ((decoded_result = ie_allowed_pdu_session_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_allowed_pdu_session_status = std::optional<AllowedPDUSessionStatus>(
            ie_allowed_pdu_session_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNasMessageContainer: {
        Logger::nas_mm().debug("Decoding ie_nas_message_container(IEI: 0x71)");
        NasMessageContainer ie_nas_message_container_tmp = {};
        if ((decoded_result = ie_nas_message_container_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_nas_message_container =
            std::optional<NasMessageContainer>(ie_nas_message_container_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      }
    }
  }

  Logger::nas_mm().debug(
      "Decoded ServiceRequest message len (%d)", decoded_size);
  return decoded_size;
}
