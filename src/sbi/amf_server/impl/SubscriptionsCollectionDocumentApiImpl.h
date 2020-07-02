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

/*
* SubscriptionsCollectionDocumentApiImpl.h
*
* 
*/

#ifndef SUBSCRIPTIONS_COLLECTION_DOCUMENT_API_IMPL_H_
#define SUBSCRIPTIONS_COLLECTION_DOCUMENT_API_IMPL_H_


#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>

#include <SubscriptionsCollectionDocumentApi.h>

#include <pistache/optional.h>

#include "ProblemDetails.h"
#include "SubscriptionData.h"

#include "amf_app.hpp"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

class SubscriptionsCollectionDocumentApiImpl : public oai::amf::api::SubscriptionsCollectionDocumentApi {
public:
    SubscriptionsCollectionDocumentApiImpl(std::shared_ptr<Pistache::Rest::Router>, amf_application::amf_app *amf_app_inst);
    ~SubscriptionsCollectionDocumentApiImpl() {}

    void a_mf_status_change_subscribe(const SubscriptionData &subscriptionData, Pistache::Http::ResponseWriter &response);
private:
    amf_application::amf_app *m_amf_app;
};

}
}
}



#endif
