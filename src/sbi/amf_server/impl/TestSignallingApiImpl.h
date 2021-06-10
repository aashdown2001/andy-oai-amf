#ifndef _TESTSIGNALLING_API_IMPL_H_
#define _TESTSIGNALLING_API_IMPL_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>

#include <TestSignallingApi.h>

#include <pistache/optional.h>

#include "ProblemDetails.h"
#include <string>

#include "amf_app.hpp"

namespace oai {
namespace amf {
namespace api {

class TestSignallingApiImpl
    : public oai::amf::api::
          TestSignallingApi {
 public:
  TestSignallingApiImpl(
      std::shared_ptr<Pistache::Rest::Router>,
      amf_application::amf_app* amf_app_inst);
  ~TestSignallingApiImpl() {}

  void test_signalling_network_initiated_deregistration(
      const std::string& subscriptionId,
      Pistache::Http::ResponseWriter& response);
  void test_signalling_paging(
          const std::string& ueContextId,
          Pistache::Http::ResponseWriter& response);

 private:
  amf_application::amf_app* m_amf_app;
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif

