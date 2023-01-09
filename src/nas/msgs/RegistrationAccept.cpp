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
  ie_allowed_nssai                               = nullptr;
  ie_rejected_nssai                              = nullptr;
  ie_configured_nssai                            = nullptr;
  ie_5gs_network_feature_support                 = nullptr;
  ie_PDU_session_status                          = nullptr;
  ie_pdu_session_reactivation_result             = nullptr;
  ie_pdu_session_reactivation_result_error_cause = nullptr;
  ie_MICO_indication                             = nullptr;
  ie_network_slicing_indication                  = nullptr;
  ie_T3512_value                                 = nullptr;
  ie_Non_3GPP_de_registration_timer_value        = nullptr;
  ie_T3502_value                                 = nullptr;
  ie_sor_transparent_container                   = nullptr;
  ie_eap_message                                 = nullptr;
  ie_nssai_inclusion_mode                        = nullptr;
  ie_negotiated_drx_parameters                   = nullptr;
  ie_non_3gpp_nw_policies                        = nullptr;
  ie_eps_bearer_context_status                   = nullptr;
  ie_extended_drx_parameters                     = nullptr;
  ie_T3447_value                                 = nullptr;
  ie_T3448_value                                 = nullptr;
  ie_T3324_value                                 = nullptr;
  ie_ue_radio_capability_id                      = nullptr;
  ie_pending_nssai                               = nullptr;
  ie_tai_list                                    = nullptr;
}

//------------------------------------------------------------------------------
RegistrationAccept::~RegistrationAccept() {}

//------------------------------------------------------------------------------
void RegistrationAccept::setHeader(uint8_t security_header_type) {
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
  PLMN_List ie_equivalent_plmns_tmp = {};
  ie_equivalent_plmns_tmp.set(kEquivalentPlmns, list);
  ie_equivalent_plmns = std::optional<PLMN_List>(ie_equivalent_plmns_tmp);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setALLOWED_NSSAI(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_allowed_nssai = new NSSAI(0x15, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::setRejected_NSSAI(
    const std::vector<Rejected_SNSSAI>& nssai) {
  if (nssai.size() > 0) {
    ie_rejected_nssai = new Rejected_NSSAI(0x11);
    ie_rejected_nssai->setRejectedSNSSAIs(nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::setCONFIGURED_NSSAI(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_configured_nssai = new NSSAI(0x31, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::set_5GS_Network_Feature_Support(
    uint8_t value, uint8_t value2) {
  ie_5gs_network_feature_support =
      new _5GS_Network_Feature_Support(0x21, value, value2);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_status(uint16_t value) {
  ie_PDU_session_status = new PDUSessionStatus(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_reactivation_result(uint16_t value) {
  ie_pdu_session_reactivation_result =
      new PDU_Session_Reactivation_Result(0x26, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPDU_session_reactivation_result_error_cause(
    uint8_t session_id, uint8_t value) {
  ie_pdu_session_reactivation_result_error_cause =
      new PDU_Session_Reactivation_Result_Error_Cause(0x72, session_id, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setMICO_Indication(bool sprti, bool raai) {
  ie_MICO_indication = new MicoIndication(sprti, raai);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNetwork_Slicing_Indication(bool dcni, bool nssci) {
  ie_network_slicing_indication =
      new NetworkSlicingIndication(0x09, dcni, nssci);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3512_Value(uint8_t unit, uint8_t value) {
  ie_T3512_value = new GPRS_Timer_3(0x5E, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNon_3GPP_de_registration_timer_value(
    uint8_t value) {
  ie_Non_3GPP_de_registration_timer_value = new GPRS_Timer_2(0x5D, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3502_value(uint8_t value) {
  ie_T3502_value = new GPRS_Timer_2(0x16, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setSOR_Transparent_Container(
    uint8_t header, uint8_t* value) {
  ie_sor_transparent_container =
      new SOR_Transparent_Container(0x73, header, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setEAP_Message(bstring eap) {
  ie_eap_message = new EAP_Message(0x78, eap);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNSSAI_Inclusion_Mode(uint8_t value) {
  ie_nssai_inclusion_mode = new NSSAI_Inclusion_Mode(0x0A, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::set_5GS_DRX_arameters(uint8_t value) {
  ie_negotiated_drx_parameters = new _5GS_DRX_Parameters(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setNon_3GPP_NW_Provided_Policies(uint8_t value) {
  ie_non_3gpp_nw_policies = new Non_3GPP_NW_Provided_Policies(0x0D, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setEPS_Bearer_Context_Status(uint16_t value) {
  ie_eps_bearer_context_status = new EPS_Bearer_Context_Status(0x60, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setExtended_DRX_Parameters(
    uint8_t paging_time, uint8_t value) {
  ie_extended_drx_parameters =
      new Extended_DRX_Parameters(0x6E, paging_time, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3447_Value(uint8_t unit, uint8_t value) {
  ie_T3447_value = new GPRS_Timer_3(0x6C, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3448_Value(uint8_t unit, uint8_t value) {
  ie_T3448_value = new GPRS_Timer_3(0x6B, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setT3324_Value(uint8_t unit, uint8_t value) {
  ie_T3324_value = new GPRS_Timer_3(0x6A, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setUE_Radio_Capability_ID(uint8_t value) {
  ie_ue_radio_capability_id = new UE_Radio_Capability_ID(0x67, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setPending_NSSAI(std::vector<struct SNSSAI_s> nssai) {
  ie_pending_nssai = new NSSAI(0x39, nssai);
}

//------------------------------------------------------------------------------
void RegistrationAccept::setTaiList(std::vector<p_tai_t> tai_list) {
  ie_tai_list = new _5GSTrackingAreaIdList(0x54, tai_list);
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
  if (!ie_tai_list) {
    Logger::nas_mm().warn("IE ie_tai_list is not available");
  } else {
    int size = ie_tai_list->Encode(buf + encoded_size, len - encoded_size);
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
  if (!ie_allowed_nssai) {
    Logger::nas_mm().warn("IE ie_allowed_nssai is not available");
  } else {
    if (int size =
            ie_allowed_nssai->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_allowed_nssai error");
      return 0;
    }
  }
  if (!ie_rejected_nssai) {
    Logger::nas_mm().warn("IE ie_rejected_nssai is not available");
  } else {
    if (int size =
            ie_rejected_nssai->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_rejected_nssai error");
    }
  }
  if (!ie_configured_nssai) {
    Logger::nas_mm().warn("IE ie_configured_nssai is not available");
  } else {
    if (int size = ie_configured_nssai->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_configured_nssai error");
      return 0;
    }
  }
  if (!ie_5gs_network_feature_support) {
    Logger::nas_mm().warn("IE ie_5gs_network_feature_support is not available");
  } else {
    if (int size = ie_5gs_network_feature_support->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_5gs_network_feature_support error");
      return 0;
    }
  }
  if (!ie_PDU_session_status) {
    Logger::nas_mm().warn("IE ie_PDU_session_status is not available");
  } else {
    if (int size = ie_PDU_session_status->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_PDU_session_status error");
      return 0;
    }
  }
  if (!ie_pdu_session_reactivation_result) {
    Logger::nas_mm().warn(
        "IE ie_pdu_session_reactivation_result is not available");
  } else {
    if (int size = ie_pdu_session_reactivation_result->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_pdu_session_reactivation_result error");
      return 0;
    }
  }
  if (!ie_pdu_session_reactivation_result_error_cause) {
    Logger::nas_mm().warn(
        "IE ie_pdu_session_reactivation_result_error_cause is not available");
  } else {
    if (int size = ie_pdu_session_reactivation_result_error_cause->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_pdu_session_reactivation_result_error_cause error");
      return 0;
    }
  }
  if (!ie_MICO_indication) {
    Logger::nas_mm().warn("IE ie_MICO_indication is not available");
  } else {
    if (int size = ie_MICO_indication->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_MICO_indication error");
      return 0;
    }
  }
  if (!ie_network_slicing_indication) {
    Logger::nas_mm().warn("IE ie_network_slicing_indication is not available");
  } else {
    if (int size = ie_network_slicing_indication->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_network_slicing_indication error");
      return 0;
    }
  }
  if (!ie_T3512_value) {
    Logger::nas_mm().warn("IE ie_T3512_value is not available");
  } else {
    if (int size =
            ie_T3512_value->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3512_value error");
      return 0;
    }
  }
  if (!ie_Non_3GPP_de_registration_timer_value) {
    Logger::nas_mm().warn(
        "IE ie_Non_3GPP_de_registration_timer_value is not available");
  } else {
    if (int size = ie_Non_3GPP_de_registration_timer_value->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_Non_3GPP_de_registration_timer_value error");
      return 0;
    }
  }
  if (!ie_T3502_value) {
    Logger::nas_mm().warn("IE ie_T3502_value is not available");
  } else {
    if (int size =
            ie_T3502_value->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3502_value error");
      return 0;
    }
  }
  if (!ie_sor_transparent_container) {
    Logger::nas_mm().warn("IE ie_sor_transparent_container is not available");
  } else {
    if (int size = ie_sor_transparent_container->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_sor_transparent_container error");
      return 0;
    }
  }
  if (!ie_eap_message) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    if (int size =
            ie_eap_message->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }
  if (!ie_nssai_inclusion_mode) {
    Logger::nas_mm().warn("IE ie_nssai_inclusion_mode is not available");
  } else {
    if (int size = ie_nssai_inclusion_mode->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nssai_inclusion_mode error");
      return 0;
    }
  }
  if (!ie_negotiated_drx_parameters) {
    Logger::nas_mm().warn("IE ie_negotiated_drx_parameters is not available");
  } else {
    if (int size = ie_negotiated_drx_parameters->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_negotiated_drx_parameters error");
      return 0;
    }
  }
  if (!ie_non_3gpp_nw_policies) {
    Logger::nas_mm().warn("IE ie_non_3gpp_nw_policies is not available");
  } else {
    if (int size = ie_non_3gpp_nw_policies->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_non_3gpp_nw_policies error");
      return 0;
    }
  }
  if (!ie_eps_bearer_context_status) {
    Logger::nas_mm().warn("IE ie_eps_bearer_context_status is not available");
  } else {
    if (int size = ie_eps_bearer_context_status->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eps_bearer_context_status error");
      return 0;
    }
  }
  if (!ie_extended_drx_parameters) {
    Logger::nas_mm().warn("IE ie_extended_drx_parameters is not available");
  } else {
    if (int size = ie_extended_drx_parameters->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_extended_drx_parameters error");
      return 0;
    }
  }
  if (!ie_T3447_value) {
    Logger::nas_mm().warn("IE ie_T3447_value is not available");
  } else {
    if (int size =
            ie_T3447_value->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3447_value error");
      return 0;
    }
  }
  if (!ie_T3448_value) {
    Logger::nas_mm().warn("IE ie_T3448_value is not available");
  } else {
    if (int size =
            ie_T3448_value->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3448_value error");
      return 0;
    }
  }
  if (!ie_T3324_value) {
    Logger::nas_mm().warn("IE ie_T3324_value is not available");
  } else {
    if (int size =
            ie_T3324_value->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3324_value error");
      return 0;
    }
  }
  if (!ie_ue_radio_capability_id) {
    Logger::nas_mm().warn("IE ie_ue_radio_capability_id is not available");
  } else {
    if (int size = ie_ue_radio_capability_id->Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_ue_radio_capability_id error");
      return 0;
    }
  }
  if (!ie_pending_nssai) {
    Logger::nas_mm().warn("IE ie_pending_nssai is not available");
  } else {
    if (int size =
            ie_pending_nssai->Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_pending_nssai error");
      return 0;
    }
  }
#if 0
        if(!ie_tai_list){
          Logger::nas_mm().warn("IE ie_tai_list is not available");
        }else{
          int size = ie_tai_list->Encode(buf+encoded_size, len-encoded_size);
          if(size != -1){
            encoded_size += size;
          }else{
	    Logger::nas_mm().error("Encoding ie_tai_list error");
	    return 0;
          }
        }
#endif
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
      case 0xB: {
        Logger::nas_mm().debug("Decoding IEI (0xB)");
        ie_MICO_indication = new MicoIndication();
        decoded_size += ie_MICO_indication->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x9: {
        Logger::nas_mm().debug("Decoding IEI (0x9)");
        ie_network_slicing_indication = new NetworkSlicingIndication();
        decoded_size += ie_network_slicing_indication->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xA: {
        Logger::nas_mm().debug("Decoding IEI (0xA)");
        ie_nssai_inclusion_mode = new NSSAI_Inclusion_Mode();
        decoded_size += ie_nssai_inclusion_mode->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0xD: {
        Logger::nas_mm().debug("Decoding IEI (0xD)");
        ie_non_3gpp_nw_policies = new Non_3GPP_NW_Provided_Policies();
        decoded_size += ie_non_3gpp_nw_policies->Decode(
            buf + decoded_size, len - decoded_size, true);
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
      case 0x15: {
        Logger::nas_mm().debug("Decoding IEI (0x15)");
        ie_allowed_nssai = new NSSAI();
        decoded_size += ie_allowed_nssai->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x11: {
        Logger::nas_mm().debug("Decoding IEI (0x11)");
        ie_rejected_nssai = new Rejected_NSSAI();
        decoded_size += ie_rejected_nssai->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x31: {
        Logger::nas_mm().debug("Decoding IEI (0x31)");
        ie_configured_nssai = new NSSAI();
        decoded_size += ie_configured_nssai->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x21: {
        Logger::nas_mm().debug("Decoding IEI (0x21)");
        ie_5gs_network_feature_support = new _5GS_Network_Feature_Support();
        decoded_size += ie_5gs_network_feature_support->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionStatus);
        ie_PDU_session_status = new PDUSessionStatus();
        decoded_size += ie_PDU_session_status->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x26: {
        Logger::nas_mm().debug("Decoding IEI (0x26)");
        ie_pdu_session_reactivation_result =
            new PDU_Session_Reactivation_Result();
        decoded_size += ie_pdu_session_reactivation_result->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x72: {
        Logger::nas_mm().debug("Decoding IEI (0x72)");
        ie_pdu_session_reactivation_result_error_cause =
            new PDU_Session_Reactivation_Result_Error_Cause();
        decoded_size += ie_pdu_session_reactivation_result_error_cause->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x5E: {
        Logger::nas_mm().debug("Decoding IEI (0x5E)");
        ie_T3512_value = new GPRS_Timer_3();
        decoded_size += ie_T3512_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x5D: {
        Logger::nas_mm().debug("Decoding IEI (0x5D)");
        ie_Non_3GPP_de_registration_timer_value = new GPRS_Timer_2();
        decoded_size += ie_Non_3GPP_de_registration_timer_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x16: {
        Logger::nas_mm().debug("Decoding IEI (0x16)");
        ie_T3502_value = new GPRS_Timer_2();
        decoded_size += ie_T3502_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x73: {
        Logger::nas_mm().debug("Decoding IEI (0x73)");
        ie_sor_transparent_container = new SOR_Transparent_Container();
        decoded_size += ie_sor_transparent_container->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding IEI (0x78)");
        ie_eap_message = new EAP_Message();
        decoded_size += ie_eap_message->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIei5gsDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI (0x%x)", kIei5gsDrxParameters);
        ie_negotiated_drx_parameters = new _5GS_DRX_Parameters();
        decoded_size += ie_negotiated_drx_parameters->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x60: {
        Logger::nas_mm().debug("Decoding IEI (0x60)");
        ie_eps_bearer_context_status = new EPS_Bearer_Context_Status();
        decoded_size += ie_eps_bearer_context_status->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x6E: {
        Logger::nas_mm().debug("Decoding IEI (0x6E)");
        ie_extended_drx_parameters = new Extended_DRX_Parameters();
        decoded_size += ie_extended_drx_parameters->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x6C: {
        Logger::nas_mm().debug("Decoding IEI (0x6C)");
        ie_T3447_value = new GPRS_Timer_3();
        decoded_size += ie_T3447_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x6B: {
        Logger::nas_mm().debug("Decoding IEI (0x6B)");
        ie_T3448_value = new GPRS_Timer_3();
        decoded_size += ie_T3448_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x6A: {
        Logger::nas_mm().debug("Decoding IEI (0x6A)");
        ie_T3324_value = new GPRS_Timer_3();
        decoded_size += ie_T3324_value->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x67: {
        Logger::nas_mm().debug("Decoding IEI (0x67)");
        ie_ue_radio_capability_id = new UE_Radio_Capability_ID();
        decoded_size += ie_ue_radio_capability_id->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x39: {
        Logger::nas_mm().debug("Decoding IEI (0x39)");
        ie_pending_nssai = new NSSAI();
        decoded_size += ie_pending_nssai->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x4A: {
        Logger::nas_mm().debug("Decoding IEI (0x4A)");
        PLMN_List ie_equivalent_plmns_tmp = {};
        decoded_size += ie_equivalent_plmns_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_equivalent_plmns = std::optional<PLMN_List>(ie_equivalent_plmns_tmp);
        octet               = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded RegistrationAccept message len (%d)", decoded_size);
  return 1;
}
