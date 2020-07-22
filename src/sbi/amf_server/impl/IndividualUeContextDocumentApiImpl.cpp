/**
* Namf_Communication
* AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved. 
*
* The version of the OpenAPI document: 1.1.0.alpha-1
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/

#include "IndividualUeContextDocumentApiImpl.h"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

IndividualUeContextDocumentApiImpl::IndividualUeContextDocumentApiImpl(std::shared_ptr<Pistache::Rest::Router> rtr, amf_application::amf_app *amf_app_inst)
    : IndividualUeContextDocumentApi(rtr), m_amf_app(amf_app_inst)
    { }

void IndividualUeContextDocumentApiImpl::create_ue_context(const std::string &ueContextId, const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "IndividualUeContextDocumentApiImpl::create_ue_context API has not been implemented yet!\n");
}
void IndividualUeContextDocumentApiImpl::e_bi_assignment(const std::string &ueContextId, const AssignEbiData &assignEbiData, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "IndividualUeContextDocumentApiImpl::e_bi_assignment API has not been implemented yet!\n");
}
void IndividualUeContextDocumentApiImpl::registration_status_update(const std::string &ueContextId, const UeRegStatusUpdateReqData &ueRegStatusUpdateReqData, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "IndividualUeContextDocumentApiImpl::registration_status_update API has not been implemented yet!\n");
}
void IndividualUeContextDocumentApiImpl::release_ue_context(const std::string &ueContextId, const UEContextRelease &uEContextRelease, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "IndividualUeContextDocumentApiImpl::release_ue_context API has not been implemented yet!\n");
}
void IndividualUeContextDocumentApiImpl::u_e_context_transfer(const std::string &ueContextId, const UeContextTransferReqData &ueContextTransferReqData, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "IndividualUeContextDocumentApiImpl::u_e_context_transfer API has not been implemented yet!\n");
}

}
}
}

