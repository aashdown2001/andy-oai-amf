#include "TestSignallingApiImpl.h"
#include "itti.hpp"
#include "amf_n2.hpp"
#include "amf_n11.hpp"

#include "pdu_session_context.hpp"
using namespace amf_application;
//using namespace ngap;

extern itti_mw* itti_inst;
extern amf_n11* amf_n11_inst;
extern amf_n2* amf_n2_inst;
namespace oai {
namespace amf {
namespace api {

TestSignallingApiImpl::
    TestSignallingApiImpl(
        std::shared_ptr<Pistache::Rest::Router> rtr,
        amf_application::amf_app* amf_app_inst)
    : TestSignallingApi(rtr),
      m_amf_app(amf_app_inst) {}

void TestSignallingApiImpl::
    test_signalling_network_initiated_deregistration(
        const std::string& subscriptionId,
        Pistache::Http::ResponseWriter& response) {
  response.send(
      Pistache::Http::Code::Ok,
      "test_signalling_network_initiated_deregistration ok!\n");

  std::string supi = subscriptionId;
  Logger::amf_server().debug(
      "Key for PDU Session context: SUPI (%s)", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
    //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;

  std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id ;
  if(amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
       Logger::amf_server().debug("udsf_response: %s", udsf_response.dump().c_str());
       psc.get()->pdu_session_context_from_json(udsf_response);
       //psc = std::shared_ptr<pdu_session_context>(psc1);  
       itti_test_signalling_network_initiated_deregistration* itti_msg =
        new itti_test_signalling_network_initiated_deregistration(AMF_SERVER, TASK_AMF_APP);

        itti_msg->ran_ue_ngap_id = psc.get()->ran_ue_ngap_id;
        itti_msg->amf_ue_ngap_id = psc.get()->amf_ue_ngap_id;

        std::shared_ptr<itti_test_signalling_network_initiated_deregistration> i =
            std::shared_ptr<itti_test_signalling_network_initiated_deregistration>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
        Logger::amf_server().error(
            "Could not send ITTI message %s to task TASK_AMF_APP",
            i->get_msg_name());
    }
  }
  else{
      Logger::amf_server().error("No existing pdu_session_context with assoc_id ");
  }
 
  //***************************stateless

//   if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
//     psc = amf_n11_inst->supi_to_pdu_ctx(supi);

//     itti_test_signalling_network_initiated_deregistration* itti_msg =
//         new itti_test_signalling_network_initiated_deregistration(AMF_SERVER, TASK_AMF_APP);

//     itti_msg->ran_ue_ngap_id = psc.get()->ran_ue_ngap_id;
//     itti_msg->amf_ue_ngap_id = psc.get()->amf_ue_ngap_id;

//     std::shared_ptr<itti_test_signalling_network_initiated_deregistration> i =
//         std::shared_ptr<itti_test_signalling_network_initiated_deregistration>(itti_msg);
//     int ret = itti_inst->send_msg(i);
//     if (0 != ret) {
//       Logger::amf_server().error(
//           "Could not send ITTI message %s to task TASK_AMF_APP",
//           i->get_msg_name());
//     }

//   } else {
//     Logger::amf_server().error(
//         "Cannot get pdu_session_context with SUPI (%s)", supi.c_str());
//   }
}

void TestSignallingApiImpl::test_signalling_paging(
          const std::string& ueContextId,
          Pistache::Http::ResponseWriter& response) {
    response.send(
        Pistache::Http::Code::Ok,
        "test_signalling_paging ok!\n");

  std::string supi = ueContextId;
  Logger::amf_server().debug(
      "Key for PDU Session context: SUPI (%s)", supi.c_str());
  //std::shared_ptr<pdu_session_context> psc;
std::shared_ptr<pdu_session_context> psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
      //***************************stateless
  pdu_session_context *psc1 = new pdu_session_context();
  nlohmann::json udsf_response;

  std::string record_id = "RECORD_ID=\'" +supi  + "\'";
  std::string udsf_url = "http://192.168.83.130:7123/nudsf-dr/v1/amfdata/" + std::string("pdu_session_context/records/") + record_id;
  if(amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
       Logger::amf_server().debug("udsf_response: %s", udsf_response.dump().c_str());
       psc.get()->pdu_session_context_from_json(udsf_response);
       //psc = std::shared_ptr<pdu_session_context>(psc1);  
       itti_test_signalling_paging* itti_msg =
        new itti_test_signalling_paging(AMF_SERVER, TASK_AMF_APP);

    itti_msg->ran_ue_ngap_id = psc.get()->ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = psc.get()->amf_ue_ngap_id;

    std::shared_ptr<itti_test_signalling_paging> i =
        std::shared_ptr<itti_test_signalling_paging>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_server().error(
          "Could not send ITTI message %s to task TASK_AMF_APP",
          i->get_msg_name());
    }
  }
  else{
      Logger::amf_server().error("No existing pdu_session_context with assoc_id ");
  }
 
  //***************************stateless
  
//   if (amf_n11_inst->is_supi_to_pdu_ctx(supi)) {
//     psc = amf_n11_inst->supi_to_pdu_ctx(supi);

//     itti_test_signalling_paging* itti_msg =
//         new itti_test_signalling_paging(AMF_SERVER, TASK_AMF_APP);

//     itti_msg->ran_ue_ngap_id = psc.get()->ran_ue_ngap_id;
//     itti_msg->amf_ue_ngap_id = psc.get()->amf_ue_ngap_id;

//     std::shared_ptr<itti_test_signalling_paging> i =
//         std::shared_ptr<itti_test_signalling_paging>(itti_msg);
//     int ret = itti_inst->send_msg(i);
//     if (0 != ret) {
//       Logger::amf_server().error(
//           "Could not send ITTI message %s to task TASK_AMF_APP",
//           i->get_msg_name());
//     }

//   } else {
//     Logger::amf_server().error(
//         "Cannot get pdu_session_context with SUPI (%s)", supi.c_str());
//   }
}

}  // namespace api
}  // namespace amf
}  // namespace oai

