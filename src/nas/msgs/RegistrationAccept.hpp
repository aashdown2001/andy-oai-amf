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

#ifndef _REGISTRATION_ACCEPT_H_
#define _REGISTRATION_ACCEPT_H_

#include "NasIeHeader.hpp"

namespace nas {

class RegistrationAccept : public NasMmPlainHeader {
 public:
  RegistrationAccept();
  ~RegistrationAccept();

  void setHeader(uint8_t security_header_type);
  void getSecurityHeaderType(uint8_t security_header_type);  // TODO
  bool verifyHeader();                                       // TODO

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len);

  void set5GSRegistrationResult(
      bool emergency, bool nssaa, bool sms, const uint8_t& value);
  // TODO: Get

  // 5GSMobileIdentity
  void setSUCI_SUPI_format_IMSI(
      const std::string& mcc, const std::string& mnc,
      const std::string& routingInd, const uint8_t& protection_sch_id,
      const std::string& msin);
  void setSUCI_SUPI_format_IMSI(
      const std::string& mcc, const std::string& mnc,
      const std::string& routingInd, const uint8_t& protection_sch_id,
      const uint8_t& hnpki, const std::string& msin);
  void set5G_GUTI(
      const std::string& mcc, const std::string& mnc,
      const std::string& amfRegionId, const std::string& amfSetId,
      const std::string& amfPointer, const uint32_t& tmsi);
  void setIMEI_IMEISV();  // TODO:
  void Set5gSTmsi();      // TODO:
  // TODO: Get

  // Equivalent PLMNs
  void setEquivalent_PLMNs(const std::vector<nas_plmn_t>& list);
  // TODO: Get

  void setTaiList(std::vector<p_tai_t> tai_list);

  void setALLOWED_NSSAI(const std::vector<struct SNSSAI_s>& nssai);
  // TODO: Get
  void setRejected_NSSAI(const std::vector<Rejected_SNSSAI>& nssai);
  // TODO: Get
  void setCONFIGURED_NSSAI(const std::vector<struct SNSSAI_s>& nssai);
  // TODO: Get

  void set_5GS_Network_Feature_Support(uint8_t value, uint8_t value2);
  // TODO: Get

  void setPDU_session_status(uint16_t value);
  // TODO: Get

  void setPDU_session_reactivation_result(uint16_t value);
  // TODO: Get

  void setPDU_session_reactivation_result_error_cause(
      uint8_t session_id, uint8_t value);
  // TODO: Get

  // TODO: LADN information

  void setMICO_Indication(bool sprti, bool raai);
  // TODO: Get

  void setNetwork_Slicing_Indication(bool dcni, bool nssci);
  // TODO: Get

  // TODO: Service Area List

  void setT3512_Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void setNon_3GPP_de_registration_timer_value(uint8_t value);
  // TODO: Get

  void setT3502_value(uint8_t value);
  // TODO: Get

  // TODO: Emergency number list
  // TODO: Extended emergency number list

  void setSOR_Transparent_Container(uint8_t header, uint8_t* value);
  // TODO: Get

  void setEAP_Message(bstring eap);
  // TODO: Get

  void setNSSAI_Inclusion_Mode(uint8_t value);
  // TODO: Get

  // TODO: Operator-defined access category definitions

  void set_5GS_DRX_arameters(uint8_t value);
  // TODO: Get

  void setNon_3GPP_NW_Provided_Policies(uint8_t value);
  // TODO: Get

  void setEPS_Bearer_Context_Status(uint16_t value);
  // TODO: Get

  void setExtended_DRX_Parameters(uint8_t paging_time, uint8_t value);
  // TODO: Get

  void setT3447_Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void setT3448_Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void setT3324_Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void setUE_Radio_Capability_ID(uint8_t value);
  // TODO: Get

  void setPending_NSSAI(std::vector<struct SNSSAI_s> nssai);
  // TODO: Get

 public:
  _5GS_Registration_Result ie_5gs_registration_result;  // Mandatory

  std::optional<_5GSMobileIdentity> ie_5g_guti;                  // Optional
  std::optional<PLMN_List> ie_equivalent_plmns;                  // Optional
  _5GSTrackingAreaIdList* ie_tai_list;                           // Optional
  NSSAI* ie_allowed_nssai;                                       // Optional
  Rejected_NSSAI* ie_rejected_nssai;                             // Optional
  NSSAI* ie_configured_nssai;                                    // Optional
  _5GS_Network_Feature_Support* ie_5gs_network_feature_support;  // Optional

  PDUSessionStatus* ie_PDU_session_status;  // Optional
  PDU_Session_Reactivation_Result*
      ie_pdu_session_reactivation_result;  // Optional
  PDU_Session_Reactivation_Result_Error_Cause*
      ie_pdu_session_reactivation_result_error_cause;  // Optional
  // TODO: LADN information
  MicoIndication* ie_MICO_indication;                       // Optional
  NetworkSlicingIndication* ie_network_slicing_indication;  // Optional
  // TODO: Service Area List
  GPRS_Timer_3* ie_T3512_value;  // Optional

  GPRS_Timer_2* ie_Non_3GPP_de_registration_timer_value;  // Optional
  GPRS_Timer_2* ie_T3502_value;                           // Optional
  // TODO: Emergency number list
  // TODO: Extended emergency number list
  SOR_Transparent_Container* ie_sor_transparent_container;  // Optional
  EAP_Message* ie_eap_message;                              // Optional
  NSSAI_Inclusion_Mode* ie_nssai_inclusion_mode;            // Optional
  // TODO: Operator-defined access category definitions
  _5GS_DRX_Parameters* ie_negotiated_drx_parameters;        // Optional
  Non_3GPP_NW_Provided_Policies* ie_non_3gpp_nw_policies;   // Optional
  EPS_Bearer_Context_Status* ie_eps_bearer_context_status;  // Optional
  Extended_DRX_Parameters* ie_extended_drx_parameters;      // Optional
  GPRS_Timer_3* ie_T3447_value;                             // Optional
  GPRS_Timer_3* ie_T3448_value;                             // Optional
  GPRS_Timer_3* ie_T3324_value;                             // Optional
  UE_Radio_Capability_ID* ie_ue_radio_capability_id;  // Which Release 16.x.x?
  NSSAI* ie_pending_nssai;                            // Which Release 16.x.x?
};

}  // namespace nas

#endif
