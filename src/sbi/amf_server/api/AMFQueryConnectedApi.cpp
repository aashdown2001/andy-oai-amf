#include "AMFQueryConnectedApi.h"
#include "Helpers.h"
#include <nlohmann/json.hpp>
#include "logger.hpp"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::helpers;

AMFQueryConnectedApi::AMFQueryConnectedApi(
    std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void AMFQueryConnectedApi::init() {
  setupRoutes();
}

void AMFQueryConnectedApi::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Get(
      *router, base + "/v1/query_connected_gnbs/",
      Routes::bind(&AMFQueryConnectedApi::gnbs_get_am_data_handler, this));
  router->addCustomHandler(
      Routes::bind(&AMFQueryConnectedApi::gnbs_default_handler, this));

  Routes::Get(
      *router, base + "/v1/query_connected_ues/",
      Routes::bind(&AMFQueryConnectedApi::ues_get_am_data_handler, this));
  router->addCustomHandler(
      Routes::bind(&AMFQueryConnectedApi::ues_default_handler, this));
}

void AMFQueryConnectedApi::gnbs_get_am_data_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  try {
    this->gnbs_get_am_data(response);
  } catch (nlohmann::detail::exception& e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError& e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception& e) {
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void AMFQueryConnectedApi::gnbs_default_handler(
    const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

void AMFQueryConnectedApi::ues_get_am_data_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  try {
    this->ues_get_am_data(response);
  } catch (nlohmann::detail::exception& e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError& e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception& e) {
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void AMFQueryConnectedApi::ues_default_handler(
    const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}  // namespace api
}  // namespace amf
}  // namespace oai
