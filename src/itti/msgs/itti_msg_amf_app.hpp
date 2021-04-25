#ifndef _AMF_APP_ITTI_H_
#define _AMF_APP_ITTI_H_

#include "amf.hpp"
#include "itti_msg.hpp"
#include "NgapIEsStruct.hpp"
#include <string>
using namespace ngap;
#include "bstrlib.h"

class itti_msg_amf_app : public itti_msg {
 public:
  itti_msg_amf_app(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}

  itti_msg_amf_app(const itti_msg_amf_app& i) : itti_msg(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
};

class itti_nas_signalling_establishment_request : public itti_msg_amf_app {
 public:
  itti_nas_signalling_establishment_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NAS_SIG_ESTAB_REQ, origin, destination) {}
  itti_nas_signalling_establishment_request(
      const itti_nas_signalling_establishment_request& i)
      : itti_msg_amf_app(i) {}
  int rrc_cause;
  int ueCtxReq;
  NrCgi_t cgi;
  Tai_t tai;
  bstring nas_buf;
  bool is_5g_s_tmsi_present;
  std::string _5g_s_tmsi;
};

class itti_n1n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_n1n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(N1N2_MESSAGE_TRANSFER_REQ, origin, destination) {}
  itti_n1n2_message_transfer_request(
      const itti_n1n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {}

  std::string supi;
  bstring n1sm;
  bstring n2sm;
  bool is_n2sm_set;
  bool is_n1sm_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
  // other parameters
};

class itti_paging_n1n2_message_transfer : public itti_msg_amf_app {
 public:
  itti_paging_n1n2_message_transfer(const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(PAGING_N1N2_MESSAGE_TRANSFER, origin, destination) {}
  itti_paging_n1n2_message_transfer(const itti_paging_n1n2_message_transfer& i) : itti_msg_amf_app(i) {}

 public:
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  plmn_t plmn;
};

class itti_test_signalling_network_initiated_deregistration : public itti_msg_amf_app {
 public:
  itti_test_signalling_network_initiated_deregistration(const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(TEST_SIGNALLING_NETWORK_INITIATED_DEREGISTRAATION, origin, destination) {}
  itti_test_signalling_network_initiated_deregistration(const itti_test_signalling_network_initiated_deregistration& i) : itti_msg_amf_app(i) {}

 public:
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
};



#endif
