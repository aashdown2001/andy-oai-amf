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

#include "RegistrationRequest.hpp"

#include "3gpp_24.501.hpp"
#include "conversions.hpp"
#include "String2Value.hpp"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
RegistrationRequest::RegistrationRequest()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, REGISTRATION_REQUEST) {
  ie_non_current_native_nas_ksi  = std::nullopt;
  ie_5g_mm_capability            = std::nullopt;
  ie_ue_security_capability      = std::nullopt;
  ie_requested_NSSAI             = std::nullopt;
  ie_s1_ue_network_capability    = std::nullopt;
  ie_uplink_data_status          = std::nullopt;
  ie_last_visited_registered_TAI = std::nullopt;
  ie_PDU_session_status          = std::nullopt;
  ie_MICO_indication             = std::nullopt;
  ie_ue_status                   = std::nullopt;
  ie_additional_guti             = std::nullopt;
  ie_allowed_PDU_session_status  = std::nullopt;
  ie_ues_usage_setting           = std::nullopt;
  ie_5gs_drx_parameters          = std::nullopt;
  ie_eps_nas_message_container   = std::nullopt;
  ie_ladn_indication             = std::nullopt;
  ie_payload_container_type      = std::nullopt;
  ie_payload_container           = std::nullopt;
  ie_network_slicing_indication  = std::nullopt;
  ie_5gs_update_type             = std::nullopt;
  ie_nas_message_container       = std::nullopt;
  ie_eps_bearer_context_status   = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationRequest::~RegistrationRequest() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationRequest::set5gsRegistrationType(bool is_for, uint8_t type) {
  ie_5gsregistrationtype.setFollowOnReq(is_for);
  ie_5gsregistrationtype.setRegType(type);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::get5GSRegistrationType(
    bool& is_for, uint8_t& reg_type /*3bits*/) {
  is_for   = ie_5gsregistrationtype.isFollowOnReq();
  reg_type = ie_5gsregistrationtype.getRegType();
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI.setNasKeyIdentifier(key_set_id);
  ie_ngKSI.setTypeOfSecurityContext(tsc);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getngKSI(uint8_t& ng_ksi) {
  ng_ksi =
      (ie_ngKSI.getTypeOfSecurityContext()) | ie_ngKSI.getNasKeyIdentifier();
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, const string msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "encoding suci and supi format for imsi error, please choose right "
        "interface");
    return;
  } else {
    ie_5gs_mobile_identity.setSuciWithSupiImsi(
        mcc, mnc, routingInd, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getMobileIdentityType() {
  return ie_5gs_mobile_identity.getTypeOfIdentity();
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi) {
  ie_5gs_mobile_identity.getSuciWithSupiImsi(imsi);
  return true;
}

//------------------------------------------------------------------------------
std::string RegistrationRequest::get_5g_guti() {
  std::optional<nas::_5G_GUTI_t> guti = std::nullopt;
  ie_5gs_mobile_identity.get5GGUTI(guti);
  if (!guti.has_value()) return {};

  std::string guti_str = guti.value().mcc + guti.value().mnc +
                         std::to_string(guti.value().amf_region_id) +
                         std::to_string(guti.value().amf_set_id) +
                         std::to_string(guti.value().amf_pointer) +
                         conv::tmsi_to_string(guti.value()._5g_tmsi);
  Logger::nas_mm().debug("5G GUTI %s", guti_str.c_str());
  return guti_str;
}

//------------------------------------------------------------------------------
// Additional_GUTI
void RegistrationRequest::setAdditional_GUTI_SUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, uint8_t amf_region_id,
    uint8_t amf_set_id, uint8_t amf_pointer, const string _5g_tmsi) {
  _5GSMobileIdentity ie_additional_guti_tmp = {};
  ie_additional_guti_tmp.setIEI(0x77);
  uint32_t tmsi = fromString<uint32_t>(_5g_tmsi);
  ie_additional_guti_tmp.set5GGUTI(
      mcc, mnc, amf_region_id, amf_set_id, amf_pointer, tmsi);
  ie_additional_guti =
      std::optional<_5GSMobileIdentity>(ie_additional_guti_tmp);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getAdditionalGuti(nas::_5G_GUTI_t& guti) {
  if (ie_additional_guti.has_value()) {
    std::optional<nas::_5G_GUTI_t> guti = std::nullopt;
    ie_additional_guti.value().get5GGUTI(guti);
    if (!guti.has_value()) return false;
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, uint8_t hnpki, const string msin) {}

//------------------------------------------------------------------------------
void RegistrationRequest::set5G_GUTI() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setIMEI_IMEISV() {}

//------------------------------------------------------------------------------
void RegistrationRequest::set5G_S_TMSI() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setNonCurrentNativeNasKSI(
    uint8_t tsc, uint8_t key_set_id) {
  ie_non_current_native_nas_ksi =
      std::make_optional<NasKeySetIdentifier>(0xC, tsc, key_set_id);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getNonCurrentNativeNasKSI(uint8_t& value) const {
  if (ie_non_current_native_nas_ksi.has_value()) {
    value |=
        (ie_non_current_native_nas_ksi.value().getTypeOfSecurityContext()) |
        (ie_non_current_native_nas_ksi.value().getNasKeyIdentifier());
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set5GMMCapability(uint8_t value) {
  ie_5g_mm_capability = std::make_optional<_5GMMCapability>(0x10, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::get5GMMCapability(uint8_t& value) {
  if (ie_5g_mm_capability.has_value()) {
    value = ie_5g_mm_capability.value().getValue();
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUESecurityCapability(
    uint8_t g_EASel, uint8_t g_IASel) {
  ie_ue_security_capability =
      std::make_optional<UESecurityCapability>(0x2E, g_EASel, g_IASel);
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUESecurityCapability(
    uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel) {
  ie_ue_security_capability = std::make_optional<UESecurityCapability>(
      0x2E, g_EASel, g_IASel, EEASel, EIASel);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeSecurityCapability(uint8_t& ea, uint8_t& ia) {
  if (ie_ue_security_capability.has_value()) {
    ea = ie_ue_security_capability.value().getEASel();
    ia = ie_ue_security_capability.value().getIASel();
    return true;
  } else {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeSecurityCapability(
    uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia) {
  if (ie_ue_security_capability.has_value()) {
    ea = ie_ue_security_capability.value().getEASel();
    ia = ie_ue_security_capability.value().getIASel();
    if (ie_ue_security_capability.value().getLength() >= 4) {
      eea = ie_ue_security_capability.value().getEEASel();
      eia = ie_ue_security_capability.value().getEIASel();
    }
    return true;
  } else {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setRequestedNSSAI(
    std::vector<struct SNSSAI_s> nssai) {
  ie_requested_NSSAI = std::make_optional<NSSAI>(0x2F, nssai);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getRequestedNssai(
    std::vector<struct SNSSAI_s>& nssai) {
  if (ie_requested_NSSAI.has_value()) {
    ie_requested_NSSAI.value().getValue(nssai);
  } else {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setLastVisitedRegisteredTAI(
    const std::string& mcc, const std::string mnc, const uint32_t& tac) {
  ie_last_visited_registered_TAI =
      std::make_optional<_5GSTrackingAreaIdentity>(0, mcc, mnc, tac);
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUENetworkCapability(
    uint8_t g_EEASel, uint8_t g_EIASel) {
  ie_s1_ue_network_capability =
      std::make_optional<UENetworkCapability>(0x17, g_EEASel, g_EIASel);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getS1UeNetworkCapability(uint8_t& eea, uint8_t& eia) {
  if (ie_s1_ue_network_capability.has_value()) {
    eea = ie_s1_ue_network_capability.value().getEEASel();
    eia = ie_s1_ue_network_capability.value().getEIASel();
  } else {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUplinkDataStatus(const uint16_t& value) {
  ie_uplink_data_status = std::make_optional<UplinkDataStatus>(0x40, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUplinkDataStatus(uint16_t& value) {
  if (ie_uplink_data_status.has_value()) {
    value = ie_uplink_data_status.value().getValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPDUSessionStatus(uint16_t value) {
  ie_PDU_session_status = std::make_optional<PDUSessionStatus>(0x50, value);
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getPduSessionStatus() {
  if (ie_PDU_session_status.has_value()) {
    return ie_PDU_session_status.value().getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setMICOIndication(bool sprti, bool raai) {
  ie_MICO_indication = std::make_optional<MICOIndication>(0x0B, sprti, raai);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getMicoIndication(uint8_t& sprti, uint8_t& raai) {
  if (ie_MICO_indication.has_value()) {
    sprti = ie_MICO_indication.value().getSPRTI();
    raai  = ie_MICO_indication.value().getRAAI();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUEStatus(bool n1, bool s1) {
  ie_ue_status = std::make_optional<UEStatus>(0x2B, n1, s1);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeStatus(uint8_t& n1ModeReg, uint8_t& s1ModeReg) {
  if (ie_ue_status.has_value()) {
    n1ModeReg = ie_ue_status.value().getN1();
    s1ModeReg = ie_ue_status.value().getS1();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setAllowedPDUSessionStatus(uint16_t value) {
  ie_allowed_PDU_session_status = std::make_optional<AllowedPDUSessionStatus>(
      0x25, value);  // TODO: remove hardcoded value
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getAllowedPduSessionStatus() {
  if (ie_allowed_PDU_session_status.has_value()) {
    return ie_allowed_PDU_session_status.value().getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUES_Usage_Setting(bool ues_usage_setting) {
  ie_ues_usage_setting =
      std::make_optional<UEUsageSetting>(0x18, ues_usage_setting);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getUEsUsageSetting() {
  if (ie_ues_usage_setting.has_value()) {
    return ie_ues_usage_setting.value().getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set_5GS_DRX_arameters(uint8_t value) {
  ie_5gs_drx_parameters =
      std::make_optional<_5GS_DRX_Parameters>(kIei5gsDrxParameters, value);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::get5GSDrxParameters() {
  if (ie_5gs_drx_parameters.has_value()) {
    return ie_5gs_drx_parameters.value().getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setEPS_NAS_Message_Container(bstring value) {
  ie_eps_nas_message_container = std::make_optional<EPS_NAS_Message_Container>(
      kIeiEpsNasMessageContainer, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getEpsNasMessageContainer(bstring& epsNas) {
  if (ie_eps_nas_message_container) {
    ie_eps_nas_message_container->getValue(epsNas);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setLADN_Indication(std::vector<bstring> ladnValue) {
  ie_ladn_indication = std::make_optional<LADN_Indication>(0x74, ladnValue);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getLadnIndication(std::vector<bstring>& ladnValue) {
  if (ie_ladn_indication.has_value()) {
    ie_ladn_indication.value().getValue(ladnValue);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPayload_Container_Type(uint8_t value) {
  ie_payload_container_type = std::make_optional<Payload_Container_Type>(
      kIeiPayloadContainerType, value);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getPayloadContainerType() {
  if (ie_payload_container_type.has_value()) {
    return ie_payload_container_type.value().getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPayload_Container(
    std::vector<PayloadContainerEntry> content) {
  ie_payload_container =
      std::make_optional<Payload_Container>(kIeiPayloadContainer, content);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getPayloadContainer(
    std::vector<PayloadContainerEntry>& content) {
  if (ie_payload_container.has_value()) {
    return ie_payload_container.value().getValue(content);
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setNetwork_Slicing_Indication(bool dcni, bool nssci) {
  ie_network_slicing_indication = std::make_optional<NetworkSlicingIndication>(
      kIeiNetworkSlicingIndication, dcni, nssci);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getNetworkSlicingIndication(
    uint8_t& dcni, uint8_t& nssci) {
  if (ie_network_slicing_indication.has_value()) {
    dcni  = ie_network_slicing_indication.value().getDCNI();
    nssci = ie_network_slicing_indication.value().getNSSCI();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set_5GS_Update_Type(
    uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms) {
  ie_5gs_update_type = std::make_optional<_5GS_Update_Type>(
      kIei5gsUpdateType, eps_pnb_ciot, _5gs_pnb_ciot, ng_ran, sms);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::get5GSUpdateType(
    uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
    bool& sms_requested) {
  if (ie_5gs_update_type.has_value()) {
    eps_pnb_ciot  = ie_5gs_update_type.value().getEPS_PNB_CIoT();
    _5gs_pnb_ciot = ie_5gs_update_type.value().get_5GS_PNB_CIoT();
    ng_ran_rcu    = ie_5gs_update_type.value().getNG_RAN();
    sms_requested = ie_5gs_update_type.value().getSMS();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setNAS_Message_Container(bstring value) {
  ie_nas_message_container =
      std::make_optional<NAS_Message_Container>(kIeiNasMessageContainer, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getNasMessageContainer(bstring& nas) {
  if (ie_nas_message_container.has_value()) {
    ie_nas_message_container.value().getValue(nas);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setEPS_Bearer_Context_Status(uint16_t value) {
  ie_eps_bearer_context_status = std::make_optional<EPS_Bearer_Context_Status>(
      kIeiEpsBearerContextStatus, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getEpsBearerContextStatus(uint16_t& value) {
  if (ie_eps_bearer_context_status.has_value()) {
    value = ie_eps_bearer_context_status.value().getValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::get5gsRegistrationType(bool& is_for, uint8_t& type) {
  is_for = ie_5gsregistrationtype.isFollowOnReq();
  type   = ie_5gsregistrationtype.getRegType();
}

//------------------------------------------------------------------------------
int RegistrationRequest::encode2Buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationRequest message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::encode2Buffer(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // 5GS Registration Type
  if ((encoded_ie_size = ie_5gsregistrationtype.encode2Buffer(
           buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding IE 5GS Registration Type error");
    return KEncodeDecodeError;
  }
  //  ngKSI
  if ((encoded_ie_size = ie_ngKSI.encode2Buffer(
           buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding IE ie_ngKSI error");
    return KEncodeDecodeError;
  }
  encoded_size += 1;  // 1/2 for 5GS registration type and 1/2 for ngKSI

  // 5GS Mobile Identity
  if ((encoded_ie_size = ie_5gs_mobile_identity.encode2Buffer(
           buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding IE 5GS Mobile Identity error");
    return KEncodeDecodeError;
  } else {
    encoded_size += encoded_ie_size;
  }

  // Non-current native NAS key set identifier
  if (!ie_non_current_native_nas_ksi.has_value()) {
    Logger::nas_mm().warn(
        "IE Non-current native NAS key set identifier is not available");
  } else {
    if ((encoded_ie_size = ie_non_current_native_nas_ksi.value().encode2Buffer(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error(
          "Encoding IE Non-current native NAS key set identifier error");
      return KEncodeDecodeError;
    } else {
      encoded_size += encoded_ie_size;
    }
  }

  // 5GMM capability
  if (!ie_5g_mm_capability.has_value()) {
    Logger::nas_mm().warn("IE 5GMM capability is not available");
  } else {
    if ((encoded_ie_size = ie_5g_mm_capability.value().encode2Buffer(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("Encoding 5GMM capability error");
      return KEncodeDecodeError;
    } else {
      encoded_size += encoded_ie_size;
    }
  }

  // UE security capability
  if (!ie_ue_security_capability.has_value()) {
    Logger::nas_mm().warn("IE UE security capability is not available");
  } else {
    if ((encoded_ie_size = ie_ue_security_capability.value().encode2Buffer(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("encoding UE security capability error");
      return KEncodeDecodeError;
    } else {
      encoded_size += encoded_ie_size;
    }
  }

  // Requested NSSAI
  if (!ie_requested_NSSAI.has_value()) {
    Logger::nas_mm().warn("IE Requested NSSAI is not available");
  } else {
    if ((encoded_ie_size = ie_requested_NSSAI.value().encode2Buffer(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("Encoding Requested NSSAI error");
      return KEncodeDecodeError;
    } else {
      encoded_size += encoded_ie_size;
    }
  }

  // Last visited registered TAI
  if (!ie_last_visited_registered_TAI.has_value()) {
    Logger::nas_mm().warn("IE ie_Last_visited_registered_TAI is not available");
  } else {
    if (int size = ie_last_visited_registered_TAI.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_Last_visited_registered_TAI  error");
      return 0;
    }
  }

  // S1 UE network capability
  if (!ie_s1_ue_network_capability.has_value()) {
    Logger::nas_mm().warn("IE ie_s1_ue_network_capability is not available");
  } else {
    if (int size = ie_s1_ue_network_capability.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_s1_ue_network_capability  error");
      return 0;
    }
  }
  if (!ie_uplink_data_status.has_value()) {
    Logger::nas_mm().warn("IE ie_uplink_data_status is not available");
  } else {
    if (int size = ie_uplink_data_status.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_uplink_data_status  error");
      return 0;
    }
  }
  if (!ie_PDU_session_status.has_value()) {
    Logger::nas_mm().warn("IE ie_PDU_session_status is not available");
  } else {
    if (int size = ie_PDU_session_status.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_PDU_session_status  error");
      return 0;
    }
  }
  if (!ie_MICO_indication.has_value()) {
    Logger::nas_mm().warn("IE ie_MICO_indicationl is not available");
  } else {
    if (int size = ie_MICO_indication.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_MICO_indicationl  error");
      return 0;
    }
  }
  if (!ie_ue_status.has_value()) {
    Logger::nas_mm().warn("IE ie_ue_status is not available");
  } else {
    if (int size = ie_ue_status.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ue_status  error");
      return 0;
    }
  }
  if (!ie_additional_guti.has_value()) {
    Logger::nas_mm().warn("IE ie_additional_guti- is not available");
  } else {
    if (int size = ie_additional_guti.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie ie_additional_guti  error");
      return 0;
    }
  }
  if (!ie_allowed_PDU_session_status.has_value()) {
    Logger::nas_mm().warn("IE ie_allowed_PDU_session_status is not available");
  } else {
    if (int size = ie_allowed_PDU_session_status.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_allowed_PDU_session_status  error");
      return 0;
    }
  }
  if (!ie_ues_usage_setting.has_value()) {
    Logger::nas_mm().warn("IE ie_ues_usage_setting is not available");
  } else {
    if (int size = ie_ues_usage_setting.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ues_usage_setting  error");
      return 0;
    }
  }
  if (!ie_5gs_drx_parameters.has_value()) {
    Logger::nas_mm().warn("IE ie_5gs_drx_parameters is not available");
  } else {
    if (int size = ie_5gs_drx_parameters.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_5gs_drx_parameters  error");
      return 0;
    }
  }
  if (!ie_eps_nas_message_container) {
    Logger::nas_mm().warn("IE ie_eps_nas_message_container is not available");
  } else {
    if (int size = ie_eps_nas_message_container->encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_eps_nas_message_container  error");
      return 0;
    }
  }
  if (!ie_ladn_indication.has_value()) {
    Logger::nas_mm().warn("IE ie_ladn_indication is not available");
  } else {
    if (int size = ie_ladn_indication.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ladn_indication  error");
      return 0;
    }
  }
  if (!ie_payload_container_type.has_value()) {
    Logger::nas_mm().warn("IE ie_payload_container_type is not available");
  } else {
    if (int size = ie_payload_container_type.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_payload_container_type  error");
      return 0;
    }
  }
  if (!ie_payload_container or !ie_payload_container_type) {
    Logger::nas_mm().warn("IE ie_payload_container is not available");
  } else {
    if (int size = ie_payload_container->encode2Buffer(
            buf + encoded_size, len - encoded_size,
            ie_payload_container_type.value().getValue())) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_payload_container  error");
      return 0;
    }
  }
  if (!ie_network_slicing_indication.has_value()) {
    Logger::nas_mm().warn("IE ie_network_slicing_indication is not available");
  } else {
    if (int size = ie_network_slicing_indication.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_network_slicing_indication  error");
      return 0;
    }
  }
  if (!ie_5gs_update_type.has_value()) {
    Logger::nas_mm().warn("IE ie_5gs_update_type is not available");
  } else {
    if (int size = ie_5gs_update_type.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_5gs_update_type  error");
      return 0;
    }
  }

  if (!ie_nas_message_container.has_value()) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    if (int size = ie_nas_message_container.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_nas_message_container  error");
      return 0;
    }
  }
  if (!ie_eps_bearer_context_status.has_value()) {
    Logger::nas_mm().warn("IE ie_eps_bearer_context_status is not available");
  } else {
    if (int size = ie_eps_bearer_context_status.value().encode2Buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_eps_bearer_context_status  error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "encoded RegistrationRequest message len(%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int RegistrationRequest::decodeFromBuffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationRequest message");
  int decoded_size    = 0;
  int decoded_size_ie = 0;
  int decoded_result  = 0;
  // plain_header           = header;
  decoded_size = NasMmPlainHeader::decodeFromBuffer(buf, len);
  // ie_5gsregistrationtype = new _5GSRegistrationType();
  decoded_size += ie_5gsregistrationtype.decodeFromBuffer(
      buf + decoded_size, len - decoded_size, false);
  decoded_size += ie_ngKSI.decodeFromBuffer(
      buf + decoded_size, len - decoded_size, false, true);
  decoded_size++;
  decoded_size += ie_5gs_mobile_identity.decodeFromBuffer(
      buf + decoded_size, len - decoded_size, false);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI 0x%x", octet);
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case 0xC: {
        Logger::nas_mm().debug("Decoding IEI(0xC)");
        NasKeySetIdentifier ie_non_current_native_nas_ksi_tmp = {};
        if ((decoded_result =
                 ie_non_current_native_nas_ksi_tmp.decodeFromBuffer(
                     buf + decoded_size, len - decoded_size, true, false)) <= 0)
          return decoded_result;
        ie_non_current_native_nas_ksi = std::optional<NasKeySetIdentifier>(
            ie_non_current_native_nas_ksi_tmp);
        decoded_size += decoded_result;
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0xB: {
        Logger::nas_mm().debug("Decoding IEI (0xB)");
        MICOIndication ie_MICO_indication_tmp = {};
        decoded_size += ie_MICO_indication_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_MICO_indication =
            std::optional<MICOIndication>(ie_MICO_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiPayloadContainerType: {
        Logger::nas_mm().debug("Decoding IEI 0x8: Payload Container Type");
        Payload_Container_Type ie_payload_container_type_tmp = {};
        decoded_size += ie_payload_container_type_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_payload_container_type = std::optional<Payload_Container_Type>(
            ie_payload_container_type_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x9: {
        Logger::nas_mm().debug("Decoding IEI (0x9)");
        NetworkSlicingIndication ie_network_slicing_indication_tmp = {};
        decoded_size += ie_network_slicing_indication_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_network_slicing_indication = std::optional<NetworkSlicingIndication>(
            ie_network_slicing_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
    }
    switch (octet) {
      case 0x10: {
        Logger::nas_mm().debug("Decoding 5GMMCapability (IEI 0x10)");
        _5GMMCapability ie_5g_mm_capability_tmp = {};
        if ((decoded_size_ie = ie_5g_mm_capability_tmp.decodeFromBuffer(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        decoded_size += decoded_size_ie;
        ie_5g_mm_capability =
            std::optional<_5GMMCapability>(ie_5g_mm_capability_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2E: {
        Logger::nas_mm().debug("Decoding IEI (0x2E)");
        UESecurityCapability ie_ue_security_capability_tmp = {};
        if ((decoded_size_ie = ie_ue_security_capability_tmp.decodeFromBuffer(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        decoded_size += decoded_size_ie;
        ie_ue_security_capability =
            std::optional<UESecurityCapability>(ie_ue_security_capability_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2F: {
        Logger::nas_mm().debug("Decoding IEI (0x2F)");
        NSSAI ie_requested_NSSAI_tmp = {};
        decoded_size += ie_requested_NSSAI_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_requested_NSSAI = std::optional<NSSAI>(ie_requested_NSSAI_tmp);
        octet              = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x52: {
        Logger::nas_mm().debug("Decoding IEI(0x52)");
        _5GSTrackingAreaIdentity last_visited_registered_tai_tmp = {};
        decoded_size += last_visited_registered_tai_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_last_visited_registered_TAI =
            std::optional<_5GSTrackingAreaIdentity>(
                last_visited_registered_tai_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x17: {
        Logger::nas_mm().debug("Decoding IEI (0x17)");
        UENetworkCapability ie_s1_ue_network_capability_tmp = {};
        decoded_size += ie_s1_ue_network_capability_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_s1_ue_network_capability =
            std::optional<UENetworkCapability>(ie_s1_ue_network_capability_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x40: {
        Logger::nas_mm().debug("Decoding IEI(0x40)");
        UplinkDataStatus ie_uplink_data_status_tmp = {};
        decoded_size += ie_uplink_data_status_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_uplink_data_status =
            std::optional<UplinkDataStatus>(ie_uplink_data_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x50: {
        Logger::nas_mm().debug("Decoding IEI (0x50)");
        PDUSessionStatus ie_PDU_session_status_tmp;
        decoded_size += ie_PDU_session_status_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_PDU_session_status =
            std::optional<PDUSessionStatus>(ie_PDU_session_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2B: {
        Logger::nas_mm().debug("Decoding IEI (0x2B)");
        UEStatus ie_ue_status_tmp = {};
        decoded_size += ie_ue_status_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_ue_status = std::optional<UEStatus>(ie_ue_status_tmp);
        octet        = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x77: {
        Logger::nas_mm().debug("Decoding IEI (0x77)");
        _5GSMobileIdentity ie_additional_guti_tmp = {};
        decoded_size += ie_additional_guti_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_additional_guti =
            std::optional<_5GSMobileIdentity>(ie_additional_guti_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x25: {
        Logger::nas_mm().debug("Decoding IEI(0x25)");
        AllowedPDUSessionStatus ie_allowed_PDU_session_status_tmp = {};
        decoded_size += ie_allowed_PDU_session_status_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_allowed_PDU_session_status = std::optional<AllowedPDUSessionStatus>(
            ie_allowed_PDU_session_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x18: {
        Logger::nas_mm().debug("Decoding IEI(0x18)");
        UEUsageSetting ie_ues_usage_setting_tmp = {};
        decoded_size += ie_ues_usage_setting_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_ues_usage_setting =
            std::optional<UEUsageSetting>(ie_ues_usage_setting_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIei5gsDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI 0x51: 5GS DRX Parameters");
        _5GS_DRX_Parameters ie_5gs_drx_parameters_tmp = {};
        decoded_size += ie_5gs_drx_parameters_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_5gs_drx_parameters =
            std::optional<_5GS_DRX_Parameters>(ie_5gs_drx_parameters_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiEpsNasMessageContainer: {
        Logger::nas_mm().debug("Decoding IEI 0x70: EPS NAS Message Container ");
        EPS_NAS_Message_Container ie_eps_nas_message_container_tmp = {};
        decoded_size += ie_eps_nas_message_container_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_eps_nas_message_container = std::optional<EPS_NAS_Message_Container>(
            ie_eps_nas_message_container_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x74: {  // TODO: verify IEI value (spec Ox79)
        Logger::nas_mm().debug("Decoding IEI(0x74)");
        LADN_Indication ie_ladn_indication_tmp = {};
        decoded_size += ie_ladn_indication_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_ladn_indication =
            std::optional<LADN_Indication>(ie_ladn_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiPayloadContainer: {
        Logger::nas_mm().debug("Decoding IEI 0x7B: Payload Container");
        Payload_Container ie_payload_container_tmp = {};
        decoded_size += ie_payload_container_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true,
            N1_SM_INFORMATION);  // TODO: verified type of Payload container
        ie_payload_container =
            std::optional<Payload_Container>(ie_payload_container_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x53: {
        Logger::nas_mm().debug("Decoding IEI(0x53)");
        _5GS_Update_Type ie_5gs_update_type_tmp = {};
        decoded_size += ie_5gs_update_type_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_5gs_update_type =
            std::optional<_5GS_Update_Type>(ie_5gs_update_type_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x71: {
        Logger::nas_mm().debug("Decoding IEI(0x71)");
        NAS_Message_Container ie_nas_message_container_tmp = {};
        decoded_size += ie_nas_message_container_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_nas_message_container =
            std::optional<NAS_Message_Container>(ie_nas_message_container_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x60: {
        Logger::nas_mm().debug("Decoding IEI(0x71)");
        EPS_Bearer_Context_Status ie_eps_bearer_context_status_tmp = {};
        decoded_size += ie_eps_bearer_context_status_tmp.decodeFromBuffer(
            buf + decoded_size, len - decoded_size, true);
        ie_eps_bearer_context_status = std::optional<EPS_Bearer_Context_Status>(
            ie_eps_bearer_context_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded RegistrationRequest message (len %d)", decoded_size);
  return 1;
}
