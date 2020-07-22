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

#include "SubscriptionsCollectionDocumentApiImpl.h"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

SubscriptionsCollectionDocumentApiImpl::SubscriptionsCollectionDocumentApiImpl(std::shared_ptr<Pistache::Rest::Router> rtr, amf_application::amf_app *amf_app_inst)
    : SubscriptionsCollectionDocumentApi(rtr), m_amf_app(amf_app_inst)
    { }

void SubscriptionsCollectionDocumentApiImpl::a_mf_status_change_subscribe(const SubscriptionData &subscriptionData, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "SubscriptionsCollectionDocumentApiImpl::a_mf_status_change_subscribe API has not been implemented yet!\n");
}

}
}
}

