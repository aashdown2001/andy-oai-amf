/**
 * Namf_Communication
 * AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "N1MessageNotifyApiImpl.h"
#include "itti_msg_sbi.hpp"
#include "conversions.hpp"
#include "itti.hpp"

using namespace amf_application;
extern itti_mw* itti_inst;

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

N1MessageNotifyApiImpl::N1MessageNotifyApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr,
    amf_application::amf_app* amf_app_inst)
    : N1MessageNotifyApi(rtr), m_amf_app(amf_app_inst) {}

void N1MessageNotifyApiImpl::receive_n1_message_notification(
    const std::string& ueContextId,
    const N1MessageNotification& notificationData, std::string& n1sm_str,
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug("Receive N1MessageNotify, handling...");

  std::string supi = ueContextId;
  Logger::amf_server().debug("SUPI (%s)", supi.c_str());

  // Handle the message in amf_app
  std::shared_ptr<itti_sbi_n1_message_notification> itti_msg =
      std::make_shared<itti_sbi_n1_message_notification>(
          TASK_AMF_SBI, TASK_AMF_APP);
  itti_msg->notification_msg = notificationData;
  itti_msg->ue_id            = supi;
  itti_msg->n1sm             = n1sm_str;
  itti_msg->http_version     = 1;

  oai::amf::model::ProblemDetails problem_details = {};
  uint32_t http_code                              = {0};

  // Send response
  response.send(Pistache::Http::Code::No_Content);
  // TODO: problem

  // Process N1 Notification Message at AMF APP
  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }
}

}  // namespace api
}  // namespace amf
}  // namespace oai