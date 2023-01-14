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
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "RegistrationAccept.hpp"

#include "3gpp_24.501.hpp"
#include "String2Value.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationAccept::RegistrationAccept()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, REGISTRATION_ACCEPT) {
  ie_5g_guti                                     = std::nullopt;
  ie_equivalent_plmns                            = std::nullopt;
  ie_allowed_nssai                               = std::nullopt;
  ie_rejected_nssai                              = std::nullopt;
  ie_configured_nssai                            = std::nullopt;
  ie_5gs_network_feature_support                 = std::nullopt;
  ie_PDU_session_status                          = std::nullopt;
  ie_pdu_session_reactivation_result             = std::nullopt;
  ie_pdu_session_reactivation_result_error_cause = std::nullopt;
  // ie_ladn_information = std::nullopt;
  ie_MICO_indication                      = std::nullopt;
  ie_network_slicing_indication           = std::nullopt;
  ie_T3512_value                          = std::nullopt;
  ie_Non_3GPP_de_registration_timer_value = std::nullopt;
  ie_T3502_value                          = std::nullopt;
  ie_sor_transparent_container            = std::nullopt;
  ie_eap_message                          = std::nullopt;
  ie_nssai_inclusion_mode                 = std::nullopt;
  ie_negotiated_drx_parameters            = std::nullopt;
  ie_non_3gpp_nw_policies                 = std::nullopt;
  ie_eps_bearer_context_status            = std::nullopt;
  ie_extended_drx_parameters              = std::nullopt;
  ie_T3447_value                          = std::nullopt;
  ie_T3448_value                          = std::nullopt;
  ie_T3324_value                          = std::nullopt;
  ie_ue_radio_capability_id               = std::nullopt;
  ie_pending_nssai                        = std::nullopt;
  ie_tai_list                             = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationAccept::~RegistrationAccept() {}

//------------------------------------------------------------------------------
void RegistrationAccept::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationAccept::set5GSRegistrationResult(
    bool emergency, bool nssaa, bool sms, const uint8_t& value) {
  ie_5gs_registration_result.set(emergency, nssaa, sms, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setSUCI_SUPI_format_IMSI(
    const std::string& mcc, const std::string& mnc,
    const std::string& routingInd, const uint8_t& protection_sch_id,
    const std::string& msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "Encoding SUCI and SUPI format for IMSI error, please choose right "
        "scheme");
    return;
  } else {
    _5GSMobileIdentity ie_5g_guti_tmp = {};
    ie_5g_guti_tmp.SetIei(kIei5gGuti);
    ie_5g_guti_tmp.SetSuciWithSupiImsi(
        mcc, mnc, routingInd, protection_sch_id, msin);
    ie_5g_guti = std::optional<_5GSMobileIdentity>(ie_5g_guti_tmp);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::setSUCI_SUPI_format_IMSI(
    const std::string& mcc, const std::string& mnc,
    const std::string& routingInd, const uint8_t& protection_sch_id,
    const uint8_t& hnpki, const std::string& msin) {
  // TODO:
}

//------------------------------------------------------------------------------
void RegistrationAccept::set5G_GUTI(
    const std::string& mcc, const std::string& mnc,
    const std::string& amfRegionId, const std::string& amfSetId,
    const std::string& amfPointer, const uint32_t& tmsi) {
  _5GSMobileIdentity ie_5g_guti_tmp = {};
  int regionId                      = fromString<int>(amfRegionId);
  int setId                         = fromString<int>(amfSetId);
  int pointer                       = fromString<int>(amfPointer);
  ie_5g_guti_tmp.SetIei(kIei5gGuti);
  ie_5g_guti_tmp.Set5gGuti(
      mcc, mnc, (uint8_t) regionId, (uint16_t) setId, (uint8_t) pointer, tmsi);
  ie_5g_guti = std::optional<_5GSMobileIdentity>(ie_5g_guti_tmp);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setIMEI_IMEISV() {}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gSTmsi() {}

//------------------------------------------------------------------------------
void RegistrationAccept::setEquivalent_PLMNs(
    const std::vector<nas_plmn_t>& list) {
  PlmnList ie_equivalent_plmns_tmp = {};
  ie_equivalent_plmns_tmp.Set(kEquivalentPlmns, list);
  ie_equivalent_plmns = std::optional<PlmnList>(ie_equivalent_plmns_tmp);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setALLOWED_NSSAI(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_allowed_nssai = std::make_optional<NSSAI>(kIeiNSSAIAllowed, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::setRejected_NSSAI(
    const std::vector<Rejected_SNSSAI>& nssai) {
  if (nssai.size() > 0) {
    ie_rejected_nssai = std::make_optional<Rejected_NSSAI>(0x11);
    ie_rejected_nssai.value().setRejectedSNSSAIs(nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::setCONFIGURED_NSSAI(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_configured_nssai = std::make_optional<NSSAI>(kIeiNSSAIConfigured, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::set_5GS_Network_Feature_Support(
    uint8_t value, uint8_t value2) {
  ie_5gs_network_feature_support =
      std::make_optional<_5GS_Network_Feature_Support>(value, value2);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_status(uint16_t value) {
  ie_PDU_session_status = std::make_optional<PDUSessionStatus>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_reactivation_result(uint16_t value) {
  ie_pdu_session_reactivation_result =
      std::make_optional<PDU_Session_Reactivation_Result>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_reactivation_result_error_cause(
    uint8_t session_id, uint8_t value) {
  ie_pdu_session_reactivation_result_error_cause =
      std::make_optional<PDU_Session_Reactivation_Result_Error_Cause>(
          session_id, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setMICO_Indication(bool sprti, bool raai) {
  ie_MICO_indication = std::make_optional<MicoIndication>(sprti, raai);
}

/*
//------------------------------------------------------------------------------
void RegistrationAccept::setLADN_Information(std::vector<bstring> ladnValue) {
  ie_ladn_information = std::make_optional<LadnInformation>(ladnValue);
}
*/

//------------------------------------------------------------------------------
void RegistrationAccept::setNetwork_Slicing_Indication(bool dcni, bool nssci) {
  ie_network_slicing_indication = std::make_optional<NetworkSlicingIndication>(
      kIeiNetworkSlicingIndication, dcni, nssci);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3512_Value(uint8_t unit, uint8_t value) {
  ie_T3512_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3512, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNon_3GPP_de_registration_timer_value(
    uint8_t value) {
  ie_Non_3GPP_de_registration_timer_value = std::make_optional<GprsTimer2>(
      kIeiGprsTimer2Non3gppDeregistration, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3502_value(uint8_t value) {
  ie_T3502_value = std::make_optional<GprsTimer2>(kIeiGprsTimer2T3502, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setSOR_Transparent_Container(
    uint8_t header, const uint8_t (&value)[16]) {
  ie_sor_transparent_container =
      std::make_optional<SOR_Transparent_Container>(header, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetEapMessage(bstring eap) {
  ie_eap_message = std::make_optional<EapMessage>(0x78, eap);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNSSAI_Inclusion_Mode(uint8_t value) {
  ie_nssai_inclusion_mode = std::make_optional<NssaiInclusionMode>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::set_5GS_DRX_arameters(uint8_t value) {
  ie_negotiated_drx_parameters = std::make_optional<_5GS_DRX_Parameters>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNon_3GPP_NW_Provided_Policies(uint8_t value) {
  ie_non_3gpp_nw_policies =
      std::make_optional<Non_3GPP_NW_Provided_Policies>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setEPS_Bearer_Context_Status(uint16_t value) {
  ie_eps_bearer_context_status =
      std::make_optional<EpsBearerContextStatus>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setExtended_DRX_Parameters(
    uint8_t paging_time, uint8_t value) {
  ie_extended_drx_parameters =
      std::make_optional<Extended_DRX_Parameters>(paging_time, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3447_Value(uint8_t unit, uint8_t value) {
  ie_T3447_value = std::make_optional<GprsTimer3>(0x6C, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3448_Value(uint8_t unit, uint8_t value) {
  ie_T3448_value = std::make_optional<GprsTimer3>(0x6B, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3324_Value(uint8_t unit, uint8_t value) {
  ie_T3324_value = std::make_optional<GprsTimer3>(0x6A, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setUE_Radio_Capability_ID(bstring value) {
  ie_ue_radio_capability_id = std::make_optional<UeRadioCapabilityId>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPending_NSSAI(std::vector<struct SNSSAI_s> nssai) {
  ie_pending_nssai = std::make_optional<NSSAI>(0x39, nssai);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setTaiList(std::vector<p_tai_t> tai_list) {
  ie_tai_list = std::make_optional<_5GSTrackingAreaIdList>(tai_list);
}

//------------------------------------------------------------------------------
int RegistrationAccept::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationAccept message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (int size = ie_5gs_registration_result.Encode(
          buf + encoded_size, len - encoded_size)) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_5gs_registration_result error");
    return 0;
  }

  if (!ie_5g_guti.has_value()) {
    Logger::nas_mm().warn("IE ie_5g_guti is not available");
  } else {
    int size =
        ie_5g_guti.value().Encode(buf + encoded_size, len - encoded_size);
    if (size) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_5g_guti error");
      return 0;
    }
  }
  if (!ie_tai_list.has_value()) {
    Logger::nas_mm().warn("IE ie_tai_list is not available");
  } else {
    int size =
        ie_tai_list.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != -1) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_tai_list error");
      return 0;
    }
  }
  if (!ie_equivalent_plmns.has_value()) {
    Logger::nas_mm().warn("IE ie_equivalent_plmns is not available");
  } else {
    if (int size = ie_equivalent_plmns.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_equivalent_plmns error");
      return 0;
    }
  }
  if (!ie_allowed_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_allowed_nssai is not available");
  } else {
    if (int size = ie_allowed_nssai.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_allowed_nssai error");
      return 0;
    }
  }
  if (!ie_rejected_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_rejected_nssai is not available");
  } else {
    if (int size = ie_rejected_nssai.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_rejected_nssai error");
    }
  }
  if (!ie_configured_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_configured_nssai is not available");
  } else {
    if (int size = ie_configured_nssai.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_configured_nssai error");
      return 0;
    }
  }
  if (!ie_5gs_network_feature_support.has_value()) {
    Logger::nas_mm().warn("IE ie_5gs_network_feature_support is not available");
  } else {
    if (int size = ie_5gs_network_feature_support.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_5gs_network_feature_support error");
      return 0;
    }
  }
  if (!ie_PDU_session_status.has_value()) {
    Logger::nas_mm().warn("IE ie_PDU_session_status is not available");
  } else {
    if (int size = ie_PDU_session_status.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_PDU_session_status error");
      return 0;
    }
  }
  if (!ie_pdu_session_reactivation_result.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_pdu_session_reactivation_result is not available");
  } else {
    if (int size = ie_pdu_session_reactivation_result.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_pdu_session_reactivation_result error");
      return 0;
    }
  }
  if (!ie_pdu_session_reactivation_result_error_cause.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_pdu_session_reactivation_result_error_cause is not available");
  } else {
    if (int size =
            ie_pdu_session_reactivation_result_error_cause.value().Encode(
                buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_pdu_session_reactivation_result_error_cause error");
      return 0;
    }
  }
  /*
  if (!ie_ladn_information.has_value()) {
    Logger::nas_mm().warn("IE ie_ladn_information is not available");
  } else {
    if (int size = ie_ladn_information.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ladn_information  error");
      return 0;
    }
  }*/
  if (!ie_MICO_indication.has_value()) {
    Logger::nas_mm().warn("IE ie_MICO_indication is not available");
  } else {
    if (int size = ie_MICO_indication.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_MICO_indication error");
      return 0;
    }
  }
  if (!ie_network_slicing_indication.has_value()) {
    Logger::nas_mm().warn("IE ie_network_slicing_indication is not available");
  } else {
    if (int size = ie_network_slicing_indication.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_network_slicing_indication error");
      return 0;
    }
  }
  if (!ie_T3512_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3512_value is not available");
  } else {
    if (int size = ie_T3512_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3512_value error");
      return 0;
    }
  }
  if (!ie_Non_3GPP_de_registration_timer_value.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_Non_3GPP_de_registration_timer_value is not available");
  } else {
    if (int size = ie_Non_3GPP_de_registration_timer_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_Non_3GPP_de_registration_timer_value error");
      return 0;
    }
  }
  if (!ie_T3502_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3502_value is not available");
  } else {
    if (int size = ie_T3502_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3502_value error");
      return 0;
    }
  }
  if (!ie_sor_transparent_container.has_value()) {
    Logger::nas_mm().warn("IE ie_sor_transparent_container is not available");
  } else {
    if (int size = ie_sor_transparent_container.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_sor_transparent_container error");
      return 0;
    }
  }
  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    if (int size = ie_eap_message.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }
  if (!ie_nssai_inclusion_mode.has_value()) {
    Logger::nas_mm().warn("IE ie_nssai_inclusion_mode is not available");
  } else {
    if (int size = ie_nssai_inclusion_mode.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nssai_inclusion_mode error");
      return 0;
    }
  }
  if (!ie_negotiated_drx_parameters.has_value()) {
    Logger::nas_mm().warn("IE ie_negotiated_drx_parameters is not available");
  } else {
    if (int size = ie_negotiated_drx_parameters.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_negotiated_drx_parameters error");
      return 0;
    }
  }
  if (!ie_non_3gpp_nw_policies.has_value()) {
    Logger::nas_mm().warn("IE ie_non_3gpp_nw_policies is not available");
  } else {
    if (int size = ie_non_3gpp_nw_policies.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_non_3gpp_nw_policies error");
      return 0;
    }
  }
  if (!ie_eps_bearer_context_status.has_value()) {
    Logger::nas_mm().warn("IE ie_eps_bearer_context_status is not available");
  } else {
    if (int size = ie_eps_bearer_context_status.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eps_bearer_context_status error");
      return 0;
    }
  }
  if (!ie_extended_drx_parameters.has_value()) {
    Logger::nas_mm().warn("IE ie_extended_drx_parameters is not available");
  } else {
    if (int size = ie_extended_drx_parameters.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_extended_drx_parameters error");
      return 0;
    }
  }
  if (!ie_T3447_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3447_value is not available");
  } else {
    if (int size = ie_T3447_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3447_value error");
      return 0;
    }
  }
  if (!ie_T3448_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3448_value is not available");
  } else {
    if (int size = ie_T3448_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3448_value error");
      return 0;
    }
  }
  if (!ie_T3324_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3324_value is not available");
  } else {
    if (int size = ie_T3324_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3324_value error");
      return 0;
    }
  }
  if (!ie_ue_radio_capability_id.has_value()) {
    Logger::nas_mm().warn("IE ie_ue_radio_capability_id is not available");
  } else {
    if (int size = ie_ue_radio_capability_id.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_ue_radio_capability_id error");
      return 0;
    }
  }
  if (!ie_pending_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_pending_nssai is not available");
  } else {
    if (int size = ie_pending_nssai.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_pending_nssai error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded RegistrationAccept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationAccept::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationAccept message");
  int decoded_size = 3;

  decoded_size += ie_5gs_registration_result.Decode(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size(%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiMicoIndication: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiMicoIndication);
        MicoIndication ie_MICO_indication_tmp = {};
        decoded_size += ie_MICO_indication_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_MICO_indication =
            std::optional<MicoIndication>(ie_MICO_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiNetworkSlicingIndication: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x9", kIeiNetworkSlicingIndication);
        NetworkSlicingIndication ie_network_slicing_indication_tmp = {};
        decoded_size += ie_network_slicing_indication_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_network_slicing_indication = std::optional<NetworkSlicingIndication>(
            ie_network_slicing_indication_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xA: {
        Logger::nas_mm().debug("Decoding IEI (0xA)");
        NssaiInclusionMode ie_nssai_inclusion_mode_tmp = {};
        decoded_size += ie_nssai_inclusion_mode_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_nssai_inclusion_mode =
            std::optional<NssaiInclusionMode>(ie_nssai_inclusion_mode_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xD: {
        Logger::nas_mm().debug("Decoding IEI (0xD)");
        Non_3GPP_NW_Provided_Policies ie_non_3gpp_nw_policies_tmp = {};
        decoded_size += ie_non_3gpp_nw_policies_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_non_3gpp_nw_policies = std::optional<Non_3GPP_NW_Provided_Policies>(
            ie_non_3gpp_nw_policies_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
    switch (octet) {
      case 0x77: {
        Logger::nas_mm().debug("Decoding IEI (0x77)");
        _5GSMobileIdentity ie_5g_guti_tmp = {};
        decoded_size +=
            ie_5g_guti_tmp.Decode(buf + decoded_size, len - decoded_size, true);
        ie_5g_guti = std::optional<_5GSMobileIdentity>(ie_5g_guti_tmp);
        octet      = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiNSSAIAllowed: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIAllowed);
        NSSAI ie_allowed_nssai_tmp = {};
        decoded_size += ie_allowed_nssai_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_allowed_nssai = std::optional<NSSAI>(ie_allowed_nssai_tmp);
        octet            = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x11: {
        Logger::nas_mm().debug("Decoding IEI (0x11)");
        Rejected_NSSAI ie_rejected_nssai_tmp = {};
        decoded_size += ie_rejected_nssai_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_rejected_nssai =
            std::optional<Rejected_NSSAI>(ie_rejected_nssai_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiNSSAIConfigured: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIConfigured);
        NSSAI ie_configured_nssai_tmp = {};
        decoded_size += ie_configured_nssai_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_configured_nssai = std::optional<NSSAI>(ie_configured_nssai_tmp);
        octet               = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIei5gsNetworkFeatureSupport: {
        Logger::nas_mm().debug("Decoding IEI (0x21)");
        _5GS_Network_Feature_Support ie_5gs_network_feature_support_tmp = {};
        decoded_size += ie_5gs_network_feature_support_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_5gs_network_feature_support =
            std::optional<_5GS_Network_Feature_Support>(
                ie_5gs_network_feature_support_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionStatus);
        PDUSessionStatus ie_PDU_session_status_tmp = {};
        decoded_size += ie_PDU_session_status_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_PDU_session_status =
            std::optional<PDUSessionStatus>(ie_PDU_session_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiPduSessionReactivationResult: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResult);
        PDU_Session_Reactivation_Result ie_pdu_session_reactivation_result_tmp =
            {};
        decoded_size += ie_pdu_session_reactivation_result_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_pdu_session_reactivation_result =
            std::optional<PDU_Session_Reactivation_Result>(
                ie_pdu_session_reactivation_result_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x72: {
        Logger::nas_mm().debug("Decoding IEI (0x72)");
        PDU_Session_Reactivation_Result_Error_Cause
            ie_pdu_session_reactivation_result_error_cause_tmp = {};
        decoded_size +=
            ie_pdu_session_reactivation_result_error_cause_tmp.Decode(
                buf + decoded_size, len - decoded_size, true);
        ie_pdu_session_reactivation_result_error_cause =
            std::optional<PDU_Session_Reactivation_Result_Error_Cause>(
                ie_pdu_session_reactivation_result_error_cause_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break; /*
       case kIeiLadnInformation: {
         Logger::nas_mm().debug("Decoding IEI(0x74)");
         LadnInformation ie_ladn_information_tmp = {};
         decoded_size += ie_ladn_information_tmp.Decode(
             buf + decoded_size, len - decoded_size, true);
         ie_ladn_information =
             std::optional<LadnInformation>(ie_ladn_information_tmp);
         octet = *(buf + decoded_size);
         Logger::nas_mm().debug("Next IEI 0x%x", octet);
       } break;
       */
      case kIeiGprsTimer3T3512: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3512);
        GprsTimer3 ie_T3512_value_tmp(kIeiGprsTimer3T3512);
        decoded_size += ie_T3512_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3512_value = std::optional<GprsTimer3>(ie_T3512_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer2Non3gppDeregistration: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiGprsTimer2Non3gppDeregistration);
        GprsTimer2 ie_Non_3GPP_de_registration_timer_value_tmp(
            kIeiGprsTimer2Non3gppDeregistration);
        decoded_size += ie_Non_3GPP_de_registration_timer_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_Non_3GPP_de_registration_timer_value = std::optional<GprsTimer2>(
            ie_Non_3GPP_de_registration_timer_value_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer2T3502: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer2T3502);
        GprsTimer2 ie_T3502_value_tmp(kIeiGprsTimer2T3502);
        decoded_size += ie_T3502_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3502_value = std::optional<GprsTimer2>(ie_T3502_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x73: {
        Logger::nas_mm().debug("Decoding IEI (0x73)");
        SOR_Transparent_Container ie_sor_transparent_container_tmp = {};
        decoded_size += ie_sor_transparent_container_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_sor_transparent_container = std::optional<SOR_Transparent_Container>(
            ie_sor_transparent_container_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding IEI (0x78)");
        EapMessage ie_eap_message_tmp = {};
        decoded_size += ie_eap_message_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIei5gsDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI (0x%x)", kIei5gsDrxParameters);
        _5GS_DRX_Parameters ie_negotiated_drx_parameters_tmp = {};
        decoded_size += ie_negotiated_drx_parameters_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_negotiated_drx_parameters = std::optional<_5GS_DRX_Parameters>(
            ie_negotiated_drx_parameters_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x60: {
        Logger::nas_mm().debug("Decoding IEI (0x60)");
        EpsBearerContextStatus ie_eps_bearer_context_status_tmp = {};
        decoded_size += ie_eps_bearer_context_status_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_eps_bearer_context_status = std::optional<EpsBearerContextStatus>(
            ie_eps_bearer_context_status_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x6E: {
        Logger::nas_mm().debug("Decoding IEI (0x6E)");
        Extended_DRX_Parameters ie_extended_drx_parameters_tmp = {};
        decoded_size += ie_extended_drx_parameters_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_extended_drx_parameters = std::optional<Extended_DRX_Parameters>(
            ie_extended_drx_parameters_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer3T3447: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3447);
        GprsTimer3 ie_T3447_value_tmp(kIeiGprsTimer3T3447);
        decoded_size += ie_T3447_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3447_value = std::optional<GprsTimer3>(ie_T3447_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer3T3348: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3348);
        GprsTimer3 ie_T3448_value_tmp(kIeiGprsTimer3T3348);
        decoded_size += ie_T3448_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3448_value = std::optional<GprsTimer3>(ie_T3448_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiGprsTimer3T3324: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3324);
        GprsTimer3 ie_T3324_value_tmp(kIeiGprsTimer3T3324);
        decoded_size += ie_T3324_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3324_value = std::optional<GprsTimer3>(ie_T3324_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x67: {
        Logger::nas_mm().debug("Decoding IEI (0x67)");
        UeRadioCapabilityId ie_ue_radio_capability_id_tmp = {};
        decoded_size += ie_ue_radio_capability_id_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_ue_radio_capability_id =
            std::optional<UeRadioCapabilityId>(ie_ue_radio_capability_id_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x39: {
        Logger::nas_mm().debug("Decoding IEI (0x39)");
        NSSAI ie_pending_nssai_tmp = {};
        decoded_size += ie_pending_nssai_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_pending_nssai = std::optional<NSSAI>(ie_pending_nssai_tmp);
        octet            = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x4A: {
        Logger::nas_mm().debug("Decoding IEI (0x4A)");
        PlmnList ie_equivalent_plmns_tmp = {};
        decoded_size += ie_equivalent_plmns_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_equivalent_plmns = std::optional<PlmnList>(ie_equivalent_plmns_tmp);
        octet               = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIei5gsTrackingAreaIdentityList: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIei5gsTrackingAreaIdentityList);
        _5GSTrackingAreaIdList ie_tai_list_tmp = {};
        decoded_size += ie_tai_list_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_tai_list = std::optional<_5GSTrackingAreaIdList>(ie_tai_list_tmp);
        octet       = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        break;
      }
    }
  }
  Logger::nas_mm().debug(
      "Decoded RegistrationAccept message len (%d)", decoded_size);
  return 1;
}
