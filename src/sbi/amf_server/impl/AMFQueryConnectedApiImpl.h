#ifndef _AMFQUERYCONNECTEDAPIIMPL_H_
#define _AMFQUERYCONNECTEDAPIIMPL_H_

#include "AMFQueryConnectedApi.h"
#include "amf_app.hpp"

namespace oai {
namespace amf {
namespace api {


class AMFQueryConnectedApiImpl : public oai::amf::api::AMFQueryConnectedApi {
 public:
    AMFQueryConnectedApiImpl(
      std::shared_ptr<Pistache::Rest::Router>,
      amf_application::amf_app* amf_app_inst);
    ~AMFQueryConnectedApiImpl() {}

    //Get gNBs
    void gnbs_get_am_data(
        Pistache::Http::ResponseWriter &response);
    //Get UEs
    void ues_get_am_data(
        Pistache::Http::ResponseWriter &response);
    
 private:
  amf_application::amf_app* m_amf_app;
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif