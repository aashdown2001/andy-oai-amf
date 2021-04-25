#include "TestSignallingApi.h"

namespace oai {
namespace amf {
namespace api {

TestSignallingApi::
    TestSignallingApi(
        std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void TestSignallingApi::init() {
  setupRoutes();
}

void TestSignallingApi::
    setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Post(
      *router,
      base + "/test-signalling/network-initiated-deregistration/:subscriptionId",
      Routes::bind(
          &TestSignallingApi::
              test_signalling_network_initiated_deregistration_handler,
          this));

  // Default handler, called when a route is not found
  router->addCustomHandler(Routes::bind(
      &TestSignallingApi::
          test_signalling_api_default_handler,
      this));
}

void TestSignallingApi::
    test_signalling_network_initiated_deregistration_handler(
        const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto subscriptionId =
      request.param(":subscriptionId").as<std::string>();

  try {
    this->test_signalling_network_initiated_deregistration(subscriptionId, response);
  } catch (nlohmann::detail::exception& e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (std::exception& e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}

void TestSignallingApi::
    test_signalling_api_default_handler(
        const Pistache::Rest::Request&,
        Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}  // namespace api
}  // namespace amf
}  // namespace oai

