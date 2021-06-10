#ifndef _TESTSIGNALLING_API_H_
#define _TESTSIGNALLING_API_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>

#include <nlohmann/json.hpp>

#include "ProblemDetails.h"
#include <string>

namespace oai {
namespace amf {
namespace api {

class TestSignallingApi {
 public:
  TestSignallingApi(
      std::shared_ptr<Pistache::Rest::Router>);
  virtual ~TestSignallingApi() {}
  void init();

  const std::string base = "/namf-test/v1";

 private:
  void setupRoutes();

  void test_signalling_network_initiated_deregistration_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void test_signalling_paging_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void test_signalling_api_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;


  virtual void test_signalling_network_initiated_deregistration(
      const std::string& subscriptionId,
      Pistache::Http::ResponseWriter& response) = 0;
  virtual void test_signalling_paging(
      const std::string& ueContextId,
      Pistache::Http::ResponseWriter& response) = 0;
};


}  // namespace api
}  // namespace amf
}  // namespace oai

#endif

