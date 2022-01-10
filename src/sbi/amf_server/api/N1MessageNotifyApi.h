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
 * N1MessageNotifyApi.h
 *
 *
 */

#ifndef N1MessageNotifyApi_H_
#define N1MessageNotifyApi_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>

#include "N1MessageNotification.h"
#include "ProblemDetails.h"
#include <string>

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

class N1MessageNotifyApi {
 public:
  N1MessageNotifyApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~N1MessageNotifyApi() {}
  void init();

  const std::string base = "/namf-comm/";

 private:
  void setupRoutes();

  void n1_message_notify_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void n1_message_notify_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  virtual void receive_n1_message_notification(
      const std::string& ueContextId,
      const N1MessageNotification& notificationData,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif /* N1MessageNotifyApi_H_ */
