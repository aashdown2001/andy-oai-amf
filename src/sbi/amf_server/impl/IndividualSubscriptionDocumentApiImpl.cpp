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

#include "IndividualSubscriptionDocumentApiImpl.h"
#include "bstrlib.h"
#include "amf_n2.hpp"

#include <shared_mutex>
using namespace amf_application;
using namespace ngap;
extern amf_n2 *amf_n2_inst;
extern unsigned char* format_string_as_hex(std::string str);
extern char* bstring2charString(bstring b);
extern void msg_str_2_msg_hex(std::string msg, bstring &b);
extern void convert_string_2_hex(std::string &input, std::string &output);
namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

IndividualSubscriptionDocumentApiImpl::IndividualSubscriptionDocumentApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr,
    amf_application::amf_app* amf_app_inst)
    : IndividualSubscriptionDocumentApi(rtr), m_amf_app(amf_app_inst) {}

void IndividualSubscriptionDocumentApiImpl::a_mf_status_change_subscribe_modfy(
    const std::string& subscriptionId, const SubscriptionData& subscriptionData,
    Pistache::Http::ResponseWriter& response) {
  response.send(
      Pistache::Http::Code::Ok,
      "IndividualSubscriptionDocumentApiImpl::a_mf_status_change_subscribe_"
      "modfy API has not been implemented yet!\n");
}
void IndividualSubscriptionDocumentApiImpl::a_mf_status_change_un_subscribe(
    const std::string& subscriptionId,
    Pistache::Http::ResponseWriter& response) {
  response.send(
      Pistache::Http::Code::Ok,
      "IndividualSubscriptionDocumentApiImpl::a_mf_status_change_un_subscribe "
      "API has not been implemented yet!\n");
}
void IndividualSubscriptionDocumentApiImpl::gnb_message_from_plugin(const std::uint32_t &assoc_id, const std::uint16_t &stream , TransData transdata,Pistache::Http::ResponseWriter &response) {
    
    printf("receive n2 message from plugin, sent to ngap task\n");
    printf("transdata = %s\n",transdata.get_amf_data().c_str());
    uint8_t buffer[SCTP_RECV_BUFFER_SIZE];
    std::memcpy(buffer, format_string_as_hex(transdata.get_amf_data()),transdata.get_amf_data().length()/2+1);
    bstring payload = blk2bstr(buffer, transdata.get_amf_data().length()/2+1);
    printf("%s\n,length %d",bdata(payload),blength(payload));

    // try to get gnb_context from UDSF
    printf("try to get gnb_context from UDSF in IndividualSubscriptionDocumentApiImpl.cpp \n");
    bool is_gnb_context_present = false;
    std::shared_ptr<gnb_context> gc = std::shared_ptr<gnb_context>(new gnb_context());
    amf_n2_inst->set_assoc_id_2_gnb_context(assoc_id, gc);
    nlohmann::json udsf_response;
    std::string record_id = "RECORD_ID=\'" +std::to_string(assoc_id)  + "\'";
    std::string udsf_url = "http://10.103.239.53:7123/nudsf-dr/v1/amfdata/" + std::string("gnb_context/records/") + record_id ;
    if(!amf_n2_inst->curl_http_client_udsf(udsf_url,"","GET",udsf_response)){
      Logger::amf_n2().error("No existing gNG context with assoc_id ...");
      is_gnb_context_present = false;
    }else if(udsf_response.dump().length()<8){
      Logger::amf_n2().error("No existing gNG context with assoc_id .....");
      is_gnb_context_present = false;
    }else{
      is_gnb_context_present = true;
    }
    if(is_gnb_context_present){
      Logger::amf_n2().debug("udsf_response: %s", udsf_response.dump().c_str());
      gc.get()->gnb_context_from_json(udsf_response);
    }else{
      amf_n2_inst->handle_sctp_new_association(assoc_id,10,8);
    } 
    amf_n2_inst->handle_receive(payload,assoc_id,stream,10,8);
    response.send(Pistache::Http::Code::Ok, "plugin message\n");

    //if (!amf_n2_inst->is_assoc_id_2_gnb_context(assoc_id)) {//to do - get context from udsf
    //    amf_n2_inst->handle_sctp_new_association(assoc_id,10,8);
    //}
}



}  // namespace api
}  // namespace amf
}  // namespace oai
