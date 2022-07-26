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

/*
 *  itti_sbi_msg.hpp
 *
 *  Date: 2021
 *  Company KCL
 *  Author: shivam.gandhi@kcl.ac.uk
 */

#ifndef ITTI_MSG_SBI_HPP_INCLUDED_
#define ITTI_MSG_SBI_HPP_INCLUDED_

#include "itti_msg.hpp"
#include "pistache/http.h"
#include "amf_msg.hpp"
#include "N1MessageNotification.h"
#include "UeN1N2InfoSubscriptionCreateData.h"
#include "amf.hpp"
#include "amf_profile.hpp"
#include "bstrlib.h"
#include "itti_msg.hpp"
#include "SliceInfoForRegistration.h"

// using namespace oai::amf::model;
using namespace amf_application;


class itti_msg_n11 : public itti_msg {
 public:
  itti_msg_n11(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}
  itti_msg_n11(const itti_msg_n11& i) : itti_msg(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

 public:
  long amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
};

class itti_nsmf_pdusession_create_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_create_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_CREATE_SM_CTX, origin, destination) {}
  itti_nsmf_pdusession_create_sm_context(
      const itti_nsmf_pdusession_create_sm_context& i)
      : itti_msg_n11(i) {}

 public:
  uint8_t req_type;
  uint8_t pdu_sess_id;
  bstring dnn;
  bstring sm_msg;
  snssai_t snssai;
  plmn_t plmn;
};

class itti_pdu_session_resource_setup_response : public itti_msg_n11 {
 public:
  itti_pdu_session_resource_setup_response(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(PDU_SESSION_RESOURCE_SETUP_RESPONSE, origin, destination) {
  }
  itti_pdu_session_resource_setup_response(
      const itti_pdu_session_resource_setup_response& i)
      : itti_msg_n11(i) {}

 public:
  uint8_t pdu_session_id;
  bstring n2sm;
};

class itti_nsmf_pdusession_update_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_update_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_UPDATE_SM_CTX, origin, destination) {
    is_n2sm_set = false;
    promise_id  = 0;
  }
  itti_nsmf_pdusession_update_sm_context(
      const itti_nsmf_pdusession_update_sm_context& i)
      : itti_msg_n11(i) {
    pdu_session_id = i.pdu_session_id;
    n2sm           = i.n2sm;
    is_n2sm_set    = i.is_n2sm_set;
    n2sm_info_type = i.n2sm_info_type;
    promise_id     = i.promise_id;
  }

 public:
  std::string supi;
  uint8_t pdu_session_id;
  bstring n2sm;
  bool is_n2sm_set;
  std::string n2sm_info_type;
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  uint32_t promise_id;
  std::string ho_state;
};

class itti_nsmf_pdusession_release_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_release_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_RELEASE_SM_CTX, origin, destination) {}
  itti_nsmf_pdusession_release_sm_context(
      const itti_nsmf_pdusession_release_sm_context& i)
      : itti_msg_n11(i) {
    supi             = i.supi;
    pdu_session_id   = i.pdu_session_id;
    promise_id       = i.promise_id;
    context_location = i.context_location;
  }

 public:
  std::string supi;
  uint8_t pdu_session_id;
  uint32_t promise_id;
  std::string context_location;
};



class itti_sbi_msg : public itti_msg {
 public:
  itti_sbi_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}
  itti_sbi_msg(const itti_sbi_msg& i) : itti_msg(i) {}
  itti_sbi_msg(
      const itti_sbi_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(i) {
    origin      = orig;
    destination = dest;
  }
};


//-----------------------------------------------------------------------------
class itti_sbi_register_nf_instance_request : public itti_sbi_msg {
 public:
  itti_sbi_register_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_NF_INSTANCE_REQUEST, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "SBI_REGISTER_NF_INSTANCE_REQUEST"; };

  amf_application::amf_profile profile;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_register_nf_instance_response : public itti_sbi_msg {
 public:
  itti_sbi_register_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_NF_INSTANCE_RESPONSE, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "SBI_REGISTER_NF_INSTANCE_RESPONSE"; };

  amf_application::amf_profile profile;
  uint8_t http_version;
  uint8_t http_response_code;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_nf_instance_request : public itti_sbi_msg {
 public:
  itti_sbi_update_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UPDATE_NF_INSTANCE_REQUEST, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "SBI_UPDATE_NF_INSTANCE_REQUEST"; };

  //  std::vector<oai::amf::model::PatchItem> patch_items;
  uint8_t http_version;
  std::string smf_instance_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_nf_instance_response : public itti_sbi_msg {
 public:
  itti_sbi_update_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UPDATE_NF_INSTANCE_RESPONSE, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "SBI_UPDATE_NF_INSTANCE_RESPONSE"; };

  uint8_t http_version;
  std::string amf_instance_id;
  uint8_t http_response_code;
};

//-----------------------------------------------------------------------------
class itti_sbi_deregister_nf_instance : public itti_sbi_msg {
 public:
  itti_sbi_deregister_nf_instance(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_DEREGISTER_NF_INSTANCE, orig, dest), http_version(1) {}
  const char* get_msg_name() { return "SBI_DEREGISTER_NF_INSTANCE"; };

  uint8_t http_version;
  std::string amf_instance_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_slice_selection_subscription_data : public itti_sbi_msg {
 public:
  itti_sbi_slice_selection_subscription_data(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_SLICE_SELECTION_SUBSCRIPTION_DATA, orig, dest),
        http_version(1) {}
  const char* get_msg_name() {
    return "SBI_SLICE_SELECTION_SUBSCRIPTION_DATA";
  };

  uint8_t http_version;
  std::string supi;
  plmn_t plmn;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_network_slice_selection_information : public itti_sbi_msg {
 public:
  itti_sbi_network_slice_selection_information(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NETWORK_SLICE_SELECTION_INFORMATION, orig, dest),
        http_version(1) {}
  const char* get_msg_name() {
    return "SBI_NETWORK_SLICE_SELECTION_INFORMATION";
  };

  uint8_t http_version;
  std::string nf_instance_id;
  oai::amf::model::SliceInfoForRegistration slice_info;
  // plmn_t plmn;
  tai_t tai;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_nf_instance_discovery : public itti_sbi_msg {
 public:
  itti_sbi_nf_instance_discovery(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NF_INSTANCE_DISCOVERY, orig, dest),
        target_amf_set_is_set(false),
        http_version(1) {}
  const char* get_msg_name() { return "SBI_NF_INSTANCE_DISCOVERY"; };

  uint8_t http_version;
  std::string target_amf_set;
  bool target_amf_set_is_set;
  std::string target_nf_type;
  std::string nrf_amf_set;
  uint32_t promise_id;
};


//-----------------------------------------------------------------------------
class itti_sbi_n1_message_notify : public itti_sbi_msg {
 public:
	itti_sbi_n1_message_notify(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_N1_MESSAGE_NOTIFY, orig, dest), http_version(1) {}
  const char* get_msg_name() { return "SBI_N1_MESSAGE_NOTIFY"; };

  uint8_t http_version;
  std::string target_amf_uri;
  std::string supi;
  bstring registration_request;
};










//-----------------------------------------------------------------------------
class itti_sbi_event_exposure_request : public itti_sbi_msg {
 public:
  itti_sbi_event_exposure_request(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_EVENT_EXPOSURE_REQUEST, orig, dest),
        event_exposure(),
        http_version(1) {}
  itti_sbi_event_exposure_request(const itti_sbi_event_exposure_request& i)
      : itti_sbi_msg(i), event_exposure(i.event_exposure), http_version(1) {}
  itti_sbi_event_exposure_request(
      const itti_sbi_event_exposure_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        event_exposure(i.event_exposure),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_EVENT_EXPOSURE_REQUEST"; };
  event_exposure_msg event_exposure;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_notification_data : public itti_sbi_msg {
 public:
  itti_sbi_notification_data(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NOTIFICATION_DATA, orig, dest),
        notification_msg(),
        http_version(1) {}
  itti_sbi_notification_data(const itti_sbi_notification_data& i)
      : itti_sbi_msg(i),
        notification_msg(i.notification_msg),
        http_version(1) {}
  itti_sbi_notification_data(
      const itti_sbi_notification_data& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        notification_msg(i.notification_msg),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_NOTIFICATION_DATA"; };
  data_notification_msg notification_msg;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_notify_subscribed_event : public itti_sbi_msg {
 public:
  itti_sbi_notify_subscribed_event(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NOTIFY_SUBSCRIBED_EVENT, orig, dest),
        notif_id(),
        http_version() {}

  itti_sbi_notify_subscribed_event(const itti_sbi_notify_subscribed_event& i)
      : itti_sbi_msg(i), notif_id(i.notif_id), http_version(i.http_version) {}
  itti_sbi_notify_subscribed_event(
      const itti_sbi_notify_subscribed_event& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        notif_id(i.notif_id),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_NOTIFY_SUBSCRIBED_EVENT"; };

  std::string notif_id;
  std::vector<amf_application::event_notification> event_notifs;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1_message_notification : public itti_sbi_msg {
 public:
  itti_sbi_n1_message_notification(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_N1_MESSAGE_NOTIFICATION, orig, dest),
        notification_msg(),
        ue_id(),
        n1sm(),
        http_version(1) {}
  itti_sbi_n1_message_notification(const itti_sbi_n1_message_notification& i)
      : itti_sbi_msg(i),
        notification_msg(i.notification_msg),
        ue_id(i.ue_id),
        n1sm(i.n1sm),
        http_version(1) {}
  itti_sbi_n1_message_notification(
      const itti_sbi_n1_message_notification& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        notification_msg(i.notification_msg),
        ue_id(i.ue_id),
        n1sm(i.n1sm),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_N1_MESSAGE_NOTIFICATION"; };
  oai::amf::model::N1MessageNotification notification_msg;
  std::string ue_id;
  std::string n1sm;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1n2_message_subscribe : public itti_sbi_msg {
 public:
  itti_sbi_n1n2_message_subscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_N1N2_MESSAGE_SUBSCRIBE, orig, dest),
        ue_cxt_id(),
        subscription_data(),
        http_version(1),
        promise_id(pid) {}
  itti_sbi_n1n2_message_subscribe(const itti_sbi_n1n2_message_subscribe& i)
      : itti_sbi_msg(i),
        ue_cxt_id(i.ue_cxt_id),
        subscription_data(i.subscription_data),
        http_version(1),
        promise_id() {}
  itti_sbi_n1n2_message_subscribe(
      const itti_sbi_n1n2_message_subscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        ue_cxt_id(i.ue_cxt_id),
        subscription_data(i.subscription_data),
        http_version(i.http_version),
        promise_id(i.promise_id) {}
  const char* get_msg_name() { return "SBI_N1N2_MESSAGE_SUBSCRIBE"; };
  std::string ue_cxt_id;
  oai::amf::model::UeN1N2InfoSubscriptionCreateData subscription_data;
  uint8_t http_version;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1n2_message_unsubscribe : public itti_sbi_msg {
 public:
  itti_sbi_n1n2_message_unsubscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_N1N2_MESSAGE_SUBSCRIBE, orig, dest),
        ue_cxt_id(),
        subscription_id(),
        http_version(1),
        promise_id(pid) {}
  itti_sbi_n1n2_message_unsubscribe(const itti_sbi_n1n2_message_unsubscribe& i)
      : itti_sbi_msg(i),
        ue_cxt_id(i.ue_cxt_id),
        subscription_id(i.subscription_id),
        http_version(1),
        promise_id() {}
  itti_sbi_n1n2_message_unsubscribe(
      const itti_sbi_n1n2_message_unsubscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        ue_cxt_id(i.ue_cxt_id),
        subscription_id(i.subscription_id),
        http_version(i.http_version),
        promise_id(i.promise_id) {}
  const char* get_msg_name() { return "SBI_N1N2_MESSAGE_UNSUBSCRIBE"; };
  std::string ue_cxt_id;
  std::string subscription_id;
  uint8_t http_version;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_amf_configuration : public itti_sbi_msg {
 public:
  itti_sbi_amf_configuration(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AMF_CONFIGURATION, orig, dest),
        http_version(1),
        promise_id(pid) {}
  const char* get_msg_name() { return "SBI_AMF_CONFIGURATION"; };

  uint8_t http_version;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_amf_configuration : public itti_sbi_msg {
 public:
  itti_sbi_update_amf_configuration(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_UPDATE_AMF_CONFIGURATION, orig, dest),
        http_version(1),
        promise_id(pid) {}
  const char* get_msg_name() { return "SBI_UPDATE_AMF_CONFIGURATION"; };

  uint8_t http_version;
  uint32_t promise_id;
  nlohmann::json configuration;
};

#endif /* ITTI_MSG_SBI_HPP_INCLUDED_ */
