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

#ifndef _REGISTRATION_REQUEST_H_
#define _REGISTRATION_REQUEST_H_

#include <bstrlib.h>
#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>

#include "NasIeHeader.hpp"
using namespace std;
namespace nas {

class RegistrationRequest : public NasMmPlainHeader {
 public:
  RegistrationRequest();
  ~RegistrationRequest();

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len);

  void setHeader(uint8_t security_header_type);
  void getSecurityHeaderType(uint8_t security_header_type);
  bool verifyHeader();

  void set5gsRegistrationType(bool is_for, uint8_t type);
  bool get5GSRegistrationType(bool& is_for, uint8_t& reg_type /*3bits*/);

  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  bool getngKSI(uint8_t& ng_ksi);

  uint8_t getMobilityIdentityType();

  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, const string msin);
  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, uint8_t hnpki, const string msin);
  bool getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi);

  void set5G_GUTI();
  std::string get_5g_guti();

  void setIMEI_IMEISV();
  void set5G_S_TMSI();
  void setAdditional_GUTI_SUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, uint8_t amf_region_id,
      uint8_t amf_set_id, uint8_t amf_pointer, const string _5g_tmsi);
  bool getAdditionalGuti(nas::_5G_GUTI_t& guti);

  void setNon_current_native_nas_ksi(uint8_t tsc, uint8_t key_set_id);
  uint8_t getNonCurrentNativeNasKSI();

  void set5G_MM_capability(uint8_t value);
  uint8_t get5GMMCapability();

  void setUE_Security_Capability(uint8_t g_EASel, uint8_t g_IASel);
  void setUE_Security_Capability(
      uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel);
  bool getUeSecurityCapability(uint8_t& ea, uint8_t& ia);
  bool getUeSecurityCapability(
      uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia);

  void setRequested_NSSAI(std::vector<struct SNSSAI_s> nssai);
  bool getRequestedNssai(std::vector<struct SNSSAI_s>& nssai);

  void setUENetworkCapability(uint8_t g_EEASel, uint8_t g_EIASel);
  bool getS1UeNetworkCapability(uint8_t& eea, uint8_t& eia);

  void setUplink_data_status(uint16_t value);
  uint16_t getUplinkDataStatus();

  void setLast_Visited_Registered_TAI(
      uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3, uint32_t TAC);

  void setPDU_session_status(uint16_t value);
  uint16_t getPduSessionStatus();

  void setMICO_Indication(bool sprti, bool raai);
  bool getMicoIndication(uint8_t& sprti, uint8_t& raai);

  void setUE_Status(bool n1, bool s1);
  bool getUeStatus(uint8_t& n1ModeReg, uint8_t& s1ModeReg);

  void setAllowed_PDU_Session_Status(uint16_t value);
  uint16_t getAllowedPduSessionStatus();

  void setUES_Usage_Setting(bool ues_usage_setting);
  uint8_t getUEsUsageSetting();

  void set_5GS_DRX_arameters(uint8_t value);
  uint8_t get5GSDrxParameters();

  void get5gsRegistrationType(bool& is_for, uint8_t& type);

  void setEPS_NAS_Message_Container(bstring value);

  void setLADN_Indication(std::vector<bstring> ladnValue);
  bool getLadnIndication(std::vector<bstring>& ladnValue);

  void setPayload_Container_Type(uint8_t value);
  uint8_t getPayloadContainerType();

  void setPayload_Container(std::vector<PayloadContainerEntry> content);
  bool getPayloadContainer(std::vector<PayloadContainerEntry>& content);

  void setNetwork_Slicing_Indication(bool dcni, bool nssci);
  bool getNetworkSlicingIndication(uint8_t& dcni, uint8_t& nssci);

  void set_5GS_Update_Type(
      uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms);
  bool get5GSUpdateType(
      uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
      bool& sms_requested);

  void setNAS_Message_Container(bstring value);
  bool getNasMessageContainer(bstring& nas);

  bool getEpsNasMessageContainer(bstring& epsNas);
  // bool getEpsNasMessageContainer(bstring& epsNas);

  void setEPS_Bearer_Context_Status(uint16_t value);
  uint16_t getEpsBearerContextStatus();

 public:
  // NasMmPlainHeader* plain_header;
  _5GSRegistrationType ie_5gsregistrationtype;  // Mandatory
  NasKeySetIdentifier ie_ngKSI;                 // Mandatory
  _5GSMobileIdentity ie_5gs_mobility_id;        // Mandatory

  NasKeySetIdentifier* ie_non_current_native_nas_ksi;  // Optional
  _5GMMCapability* ie_5g_mm_capability;                // Optional
  UESecurityCapability* ie_ue_security_capability;     // Optional
  NSSAI* ie_requested_NSSAI;                           // Optional
  // TODO: Last visited registered TAI
  UENetworkCapability* ie_s1_ue_network_capability;             // Optional
  UplinkDataStatus* ie_uplink_data_status;                      // Optional
  _5GS_Tracking_Area_Identity* ie_last_visited_registered_TAI;  // Optional
  PDU_Session_Status* ie_PDU_session_status;                    // Optional
  MICO_Indication* ie_MICO_indicationl;                         // Optional
  UE_Status* ie_ue_status;                                      // Optional
  _5GSMobileIdentity* ie_additional_guti;                       // Optional
  Allowed_PDU_Session_Status* ie_allowed_PDU_session_status;    // Optional
  UES_Usage_Setting* ie_ues_usage_setting;                      // Optional
  _5GS_DRX_arameters* ie_5gs_drx_parameters;                    // Optional
  EPS_NAS_Message_Container* ie_eps_nas_message_container;      // Optional
  LADN_Indication* ie_ladn_indication;                          // Optional
  Payload_Container_Type* ie_payload_container_type;            // Optional
  Payload_Container* ie_payload_container;                      // Optional
  Network_Slicing_Indication* ie_network_slicing_indication;    // Optional
  _5GS_Update_Type* ie_5gs_update_type;                         // Optional
  // TODO: Mobile station classmark 2
  // TODO: Supported codecs
  NAS_Message_Container* ie_nas_message_container;          // Optional
  EPS_Bearer_Context_Status* ie_eps_bearer_context_status;  // Optional
  // TODO: Requested extended DRX parameters
  // TODO: T3324 value
};

}  // namespace nas

#endif
