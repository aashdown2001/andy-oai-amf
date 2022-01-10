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

/*
 * N1MessageNotifyApiImpl.h
 *
 *
 */

#ifndef N1_MESSAGE_NOTIFY_API_IMPL_H_
#define N1_MESSAGE_NOTIFY_API_IMPL_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>

#include <N1MessageNotifyApi.h>

#include <pistache/optional.h>

#include "ProblemDetails.h"

#include "amf_app.hpp"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;
using namespace oai::amf::api;

class N1MessageNotifyApiImpl : public N1MessageNotifyApi {
 public:
  N1MessageNotifyApiImpl(
      std::shared_ptr<Pistache::Rest::Router>,
      amf_application::amf_app* amf_app_inst);
  ~N1MessageNotifyApiImpl() {}

  void receive_n1_message_notification(
      const std::string& ueContextId,
      const N1MessageNotification& notificationData,
      Pistache::Http::ResponseWriter& response);

 private:
  amf_application::amf_app* m_amf_app;
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif
