#ifndef _AMFQUERYCONNECTEDAPI_H_
#define _AMFQUERYCONNECTEDAPI_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>

#include <string>

namespace oai {
namespace amf {
namespace api {


class AMFQueryConnectedApi {
 public:
  AMFQueryConnectedApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~AMFQueryConnectedApi() {}
  void init();

  const std::string base = "/openxg-amf/";

 private:
  void setupRoutes();
  std::shared_ptr<Pistache::Rest::Router> router;
  
    //Get the list of GNBS
    void gnbs_get_am_data_handler(
        const Pistache::Rest::Request &request,              
        Pistache::Http::ResponseWriter response);
    void gnbs_default_handler(
        const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    virtual void gnbs_get_am_data(
        Pistache::Http::ResponseWriter &response)=0;
    
    //Gets a list of ue numbers
    void ues_get_am_data_handler(
        const Pistache::Rest::Request &request,              
        Pistache::Http::ResponseWriter response);
    void ues_default_handler(
        const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    virtual void ues_get_am_data(
        Pistache::Http::ResponseWriter &response)=0;

};

}
}
}

#endif