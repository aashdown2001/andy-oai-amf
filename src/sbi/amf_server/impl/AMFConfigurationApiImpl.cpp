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

#include <AMFConfigurationApiImpl.h>

#include "logger.hpp"

extern itti_mw* itti_inst;

namespace oai::amf::api {

using namespace oai::amf::model;

AMFConfigurationApiImpl::AMFConfigurationApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr, amf_app* amf_app_inst)
    : AMFConfigurationApi(rtr), m_amf_app(amf_app_inst) {}

void AMFConfigurationApiImpl::read_configuration(
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug("Receive AMFConfiguration, handling...");

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the AMFConfiguration in amf_app
  std::shared_ptr<itti_sbi_amf_configuration> itti_msg =
      std::make_shared<itti_sbi_amf_configuration>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->http_version = 1;
  itti_msg->promise_id   = promise_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }

  boost::future_status status;
  // wait for timeout or ready
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    // Wait for the result from APP
    // result includes json content and http response code
    nlohmann::json result = f.get();
    Logger::amf_server().debug("Got result for promise ID %d", promise_id);

    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == 200) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }
      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/json"));
      response.send(Pistache::Http::Code::Ok, json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/problem+json"));
      response.send(
          Pistache::Http::Code(http_response_code), json_data.dump().c_str());
    }
  }
}

}  // namespace oai::amf::api