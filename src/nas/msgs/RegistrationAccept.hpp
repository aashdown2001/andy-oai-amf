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

  void SetHeader(uint8_t security_header_type);
  void GetSecurityHeaderType(uint8_t security_header_type);  // TODO
  bool verifyHeader();                                       // TODO

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void Set5gsRegistrationResult(
      bool emergency, bool nssaa, bool sms, const uint8_t& value);
  // TODO: Get

  // 5GSMobileIdentity
  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routingInd, const uint8_t& protection_sch_id,
      const std::string& msin);
  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routingInd, const uint8_t& protection_sch_id,
      const uint8_t& hnpki, const std::string& msin);
  void Set5gGuti(
      const std::string& mcc, const std::string& mnc,
      const std::string& amfRegionId, const std::string& amfSetId,
      const std::string& amfPointer, const uint32_t& tmsi);
  void SetImeiImeisv();  // TODO:
  void Set5gSTmsi();     // TODO:
  // TODO: Get

  // Equivalent PLMNs
  void SetEquivalentPlmns(const std::vector<nas_plmn_t>& list);
  // TODO: Get

  void setTaiList(std::vector<p_tai_t> tai_list);

  void SetAllowedNssai(const std::vector<struct SNSSAI_s>& nssai);
  // TODO: Get
  void SetRejectedNssai(const std::vector<Rejected_SNSSAI>& nssai);
  // TODO: Get
  void SetConfiguredNssai(const std::vector<struct SNSSAI_s>& nssai);
  // TODO: Get

  void Set5gsNetworkFeatureSupport(uint8_t value, uint8_t value2);
  // TODO: Get

  void SetPduSessionStatus(uint16_t value);
  // TODO: Get

  void SetPduSessionReactivationResult(uint16_t value);
  // TODO: Get

  void SetPduSessionReactivationResultErrorCause(
      uint8_t session_id, uint8_t value);
  // TODO: Get

  // TODO: LADN information

  void SetMicoIndication(bool sprti, bool raai);
  // TODO: Get

  void SetNetworkSlicingIndication(bool dcni, bool nssci);
  // TODO: Get

  // TODO: Service Area List

  void SetT3512Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void SetNon3gppDeregistrationTimerValue(uint8_t value);
  // TODO: Get

  void SetT3502Value(uint8_t value);
  // TODO: Get

  // TODO: Emergency number list
  // TODO: Extended emergency number list

  void SetSorTransparentContainer(uint8_t header, const uint8_t (&value)[16]);
  // TODO: Get

  void SetEapMessage(bstring eap);
  // TODO: Get

  void SetNssaiInclusionMode(uint8_t value);
  // TODO: Get

  // TODO: Operator-defined access category definitions

  void Set5gsDrxParameters(uint8_t value);
  // TODO: Get

  void SetNon3gppNwProvidedPolicies(uint8_t value);
  // TODO: Get

  void SetEpsBearerContextsStatus(uint16_t value);
  // TODO: Get

  void SetExtendedDrxParameters(uint8_t paging_time, uint8_t value);
  // TODO: Get

  void SetT3447Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void SetT3448Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void SetT3324Value(uint8_t unit, uint8_t value);
  // TODO: Get

  void SetUeRadioCapabilityId(bstring value);
  // TODO: Get

  void SetPendingNssai(std::vector<struct SNSSAI_s> nssai);
  // TODO: Get

 public:
  _5GS_Registration_Result ie_5gs_registration_result;  // Mandatory

  std::optional<_5GSMobileIdentity> ie_5g_guti;       // Optional
  std::optional<PlmnList> ie_equivalent_plmns;        // Optional
  std::optional<_5GSTrackingAreaIdList> ie_tai_list;  // Optional
  std::optional<NSSAI> ie_allowed_nssai;              // Optional
  std::optional<Rejected_NSSAI> ie_rejected_nssai;    // Optional
  std::optional<NSSAI> ie_configured_nssai;           // Optional
  std::optional<_5GS_Network_Feature_Support>
      ie_5gs_network_feature_support;                     // Optional
  std::optional<PDUSessionStatus> ie_PDU_session_status;  // Optional
  std::optional<PDU_Session_Reactivation_Result>
      ie_pdu_session_reactivation_result;  // Optional
  std::optional<PDU_Session_Reactivation_Result_Error_Cause>
      ie_pdu_session_reactivation_result_error_cause;  // Optional
  // TODO: std::optional<LadnInformation> ie_ladn_information;               //
  // Optional
  std::optional<MicoIndication> ie_MICO_indication;  // Optional
  std::optional<NetworkSlicingIndication>
      ie_network_slicing_indication;  // Optional
  // TODO: Service Area List
  std::optional<GprsTimer3> ie_T3512_value;  // Optional
  std::optional<GprsTimer2>
      ie_Non_3GPP_de_registration_timer_value;  // Optional
  std::optional<GprsTimer2> ie_T3502_value;     // Optional
  // TODO: Emergency number list
  // TODO: Extended emergency number list
  std::optional<SorTransparentContainer>
      ie_sor_transparent_container;                           // Optional
  std::optional<EapMessage> ie_eap_message;                   // Optional
  std::optional<NssaiInclusionMode> ie_nssai_inclusion_mode;  // Optional
  // TODO: Operator-defined access category definitions
  std::optional<_5GS_DRX_Parameters> ie_negotiated_drx_parameters;  // Optional
  std::optional<Non_3GPP_NW_Provided_Policies>
      ie_non_3gpp_nw_policies;  // Optional
  std::optional<EpsBearerContextStatus>
      ie_eps_bearer_context_status;  // Optional
  std::optional<Extended_DRX_Parameters>
      ie_extended_drx_parameters;            // Optional
  std::optional<GprsTimer3> ie_T3447_value;  // Optional
  std::optional<GprsTimer3> ie_T3448_value;  // Optional
  std::optional<GprsTimer3> ie_T3324_value;  // Optional
  std::optional<UeRadioCapabilityId>
      ie_ue_radio_capability_id;  // Release 16.4.1
  // TODO: UE radio capability ID deletion indication
  std::optional<NSSAI> ie_pending_nssai;  // Release 16.4.1
  // TODO: Ciphering key data (Release 16.4.1)
  // TODO: CAG information list (Release 16.4.1)
  // TODO: Truncated 5G-S-TMSI configuration (Release 16.4.1)
  // TODO: Negotiated WUS assistance information (Release 16.4.1)
};

}  // namespace nas

#endif
