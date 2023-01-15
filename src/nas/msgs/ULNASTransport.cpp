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

#include "ULNASTransport.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ULNASTransport::ULNASTransport()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, UL_NAS_TRANSPORT) {
  ie_pdu_session_identity_2        = std::nullopt;
  ie_old_pdu_session_identity_2    = std::nullopt;
  ie_request_type                  = std::nullopt;
  ie_s_nssai                       = std::nullopt;
  ie_dnn                           = std::nullopt;
  ie_additional_information        = std::nullopt;
  ie_ma_pdu_session_information    = std::nullopt;
  ie_release_assistance_indication = std::nullopt;
}

//------------------------------------------------------------------------------
ULNASTransport::~ULNASTransport() {}

//------------------------------------------------------------------------------
void ULNASTransport::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void ULNASTransport::SetPayloadContainerType(uint8_t value) {
  ie_payload_container_type.SetValue(value);
}

//------------------------------------------------------------------------------
uint8_t ULNASTransport::GetPayloadContainerType() {
  return ie_payload_container_type.GetValue();
}

//------------------------------------------------------------------------------
void ULNASTransport::SetPayloadContainer(
    std::vector<PayloadContainerEntry> content) {
  ie_payload_container.SetValue(content);
}

//------------------------------------------------------------------------------
void ULNASTransport::GetPayloadContainer(bstring& content) {
  ie_payload_container.GetValue(content);
}

//------------------------------------------------------------------------------
void ULNASTransport::GetPayloadContainer(
    std::vector<PayloadContainerEntry>& content) {
  ie_payload_container.GetValue(content);
}

//------------------------------------------------------------------------------
void ULNASTransport::SetPduSessionIdentity2(uint8_t value) {
  ie_pdu_session_identity_2 =
      std::make_optional<PduSessionIdentity2>(kIeiPduSessionId, value);
}

//------------------------------------------------------------------------------
uint8_t ULNASTransport::GetPduSessionId() {
  if (ie_pdu_session_identity_2.has_value()) {
    return ie_pdu_session_identity_2.value().GetValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void ULNASTransport::SetOldPduSessionIdentity2(uint8_t value) {
  ie_old_pdu_session_identity_2 =
      std::make_optional<PduSessionIdentity2>(kIeiOldPduSessionId, value);
}

//------------------------------------------------------------------------------
bool ULNASTransport::GetOldPduSessionId(uint8_t& value) {
  if (ie_old_pdu_session_identity_2.has_value()) {
    value = ie_old_pdu_session_identity_2.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void ULNASTransport::SetRequestType(uint8_t value) {
  ie_request_type = std::make_optional<RequestType>(value);
}

//------------------------------------------------------------------------------
bool ULNASTransport::GetRequestType(uint8_t& value) {
  if (ie_request_type.has_value()) {
    value = ie_request_type.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void ULNASTransport::SetSNssai(SNSSAI_s snssai) {
  ie_s_nssai =
      std::make_optional<S_NSSAI>(std::optional<uint8_t>{kIeiSNssai}, snssai);
}

//------------------------------------------------------------------------------
bool ULNASTransport::GetSNssai(SNSSAI_s& snssai) {
  if (ie_s_nssai.has_value()) {
    ie_s_nssai.value().getValue(snssai);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void ULNASTransport::setDNN(bstring dnn) {
  ie_dnn = std::make_optional<DNN>(dnn);
}

//------------------------------------------------------------------------------
bool ULNASTransport::getDnn(bstring& dnn) {
  if (ie_dnn.has_value()) {
    ie_dnn.value().GetValue(dnn);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void ULNASTransport::SetAdditionalInformation(const bstring& value) {
  ie_additional_information = std::make_optional<AdditionalInformation>(value);
}

//------------------------------------------------------------------------------
void ULNASTransport::SetMaPduSessionInformation(uint8_t value) {
  ie_ma_pdu_session_information =
      std::make_optional<MaPduSessionInformation>(value);
}

//------------------------------------------------------------------------------
void ULNASTransport::SetReleaseAssistanceIndication(uint8_t value) {
  ie_release_assistance_indication =
      std::make_optional<Release_Assistance_Indication>(0x0F, value);
}

//------------------------------------------------------------------------------
int ULNASTransport::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding UL NAS Transport message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  int size =
      ie_payload_container_type.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding ie_payload_container_type error");
    return KEncodeDecodeError;
  }
  if (size == 0)
    size++;  // 1/2 octet for  ie_payload_container_type, 1/2 octet for spare
  encoded_size += size;

  size = ie_payload_container.Encode(
      buf + encoded_size, len - encoded_size,
      ie_payload_container_type.GetValue());

  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_payload_container  error");
    return KEncodeDecodeError;
  }

  if (!ie_pdu_session_identity_2.has_value()) {
    Logger::nas_mm().warn("IE ie_pdu_session_identity_2 is not available");
  } else {
    size = ie_pdu_session_identity_2.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_pdu_session_identity_2  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_old_pdu_session_identity_2.has_value()) {
    Logger::nas_mm().warn("IE ie_old_pdu_session_identity_2 is not available");
  } else {
    size = ie_old_pdu_session_identity_2.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding IE ie_old_pdu_session_identity_2  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_request_type.has_value()) {
    Logger::nas_mm().warn("IE ie_request_type is not available");
  } else {
    size =
        ie_request_type.value().Encode(buf + encoded_size, len - encoded_size);

    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_request_type  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_s_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_s_nssai is not available");
  } else {
    size = ie_s_nssai.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_s_nssai  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_dnn.has_value()) {
    Logger::nas_mm().warn("IE ie_dnn is not available");
  } else {
    size = ie_dnn.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_dnn  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_additional_information.has_value()) {
    Logger::nas_mm().warn("IE ie_additional_information is not available");
  } else {
    size = ie_additional_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_additional_information  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_ma_pdu_session_information.has_value()) {
    Logger::nas_mm().warn("IE ie_ma_pdu_session_information is not available");
  } else {
    size = ie_ma_pdu_session_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding IE ie_ma_pdu_session_information  error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_release_assistance_indication.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_release_assistance_indication is not available");
  } else {
    size = ie_release_assistance_indication.value().Encode(
        buf + encoded_size, len - encoded_size);

    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding IE ie_release_assistance_indication  error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded UL NAS Transport message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ULNASTransport::Decode(NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ULNASTransport message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Payload Container Type
  decoded_result = ie_payload_container_type.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;
  decoded_size++;  // 1/2 octet for PayloadContainerType, 1/2 octet for spare

  // Payload Container
  decoded_result += ie_payload_container.Decode(
      buf + decoded_size, len - decoded_size, false,
      ie_payload_container_type.GetValue());
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiRequestType: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRequestType);
        RequestType ie_request_type_tmp = {};
        if ((decoded_result = ie_request_type_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_request_type = std::optional<RequestType>(ie_request_type_tmp);
        octet           = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xA: {
        Logger::nas_mm().debug("Decoding IEI (0xA)");
        MaPduSessionInformation ie_ma_pdu_session_information_tmp = {};
        if ((decoded_result = ie_ma_pdu_session_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_ma_pdu_session_information = std::optional<MaPduSessionInformation>(
            ie_ma_pdu_session_information_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xF: {
        Logger::nas_mm().debug("Decoding IEI (0xF)");
        Release_Assistance_Indication ie_release_assistance_indication_tmp = {};
        if ((decoded_result = ie_release_assistance_indication_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_release_assistance_indication =
            std::optional<Release_Assistance_Indication>(
                ie_release_assistance_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      default: {
        flag = true;
      }
    }
    switch (octet) {
      case kIeiPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionId);
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
      case kIeiOldPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiOldPduSessionId);
        PduSessionIdentity2 ie_old_pdu_session_identity_2_tmp = {};
        if ((decoded_result = ie_old_pdu_session_identity_2_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_old_pdu_session_identity_2 = std::optional<PduSessionIdentity2>(
            ie_old_pdu_session_identity_2_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x22: {
        Logger::nas_mm().debug("Decoding IEI (0x22)");
        auto s_nssai_ie = std::optional<uint8_t>(kIeiSNssai);
        S_NSSAI ie_s_nssai_tmp(s_nssai_ie);
        if ((decoded_result = ie_s_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, kIeIsOptional)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_s_nssai = std::optional<S_NSSAI>(ie_s_nssai_tmp);
        octet      = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiDnn: {
        Logger::nas_mm().debug("Decoding IEI (0x25)");
        DNN ie_dnn_tmp = {};
        if ((decoded_result = ie_dnn_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_dnn = std::optional<DNN>(ie_dnn_tmp);

        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiAdditionalInformation: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAdditionalInformation);
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
      default: {
        // TODO:
        if (flag) {
          Logger::nas_mm().debug("Unknown IEI (0x%x)", octet);
          decoded_size++;
          return decoded_size;
          //*(buf + decoded_size) = 0x00;
          // octet                 = *(buf + decoded_size);
          // Logger::nas_mm().debug("Next IEI (0x%x)", octet);
        }
      }
    }
    flag = false;
  }

  Logger::nas_mm().debug(
      "Decoded ULNASTransport message len(%d)", decoded_size);
  return decoded_size;
}
