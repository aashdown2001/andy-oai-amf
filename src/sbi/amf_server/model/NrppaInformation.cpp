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


#include "NrppaInformation.h"

namespace oai {
namespace amf {
namespace model {

NrppaInformation::NrppaInformation()
{
    m_NfId = "";
    m_ServiceInstanceId = "";
    m_ServiceInstanceIdIsSet = false;
    
}

NrppaInformation::~NrppaInformation()
{
}

void NrppaInformation::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const NrppaInformation& o)
{
    j = nlohmann::json();
    j["nfId"] = o.m_NfId;
    j["nrppaPdu"] = o.m_NrppaPdu;
    if(o.serviceInstanceIdIsSet())
        j["serviceInstanceId"] = o.m_ServiceInstanceId;
}

void from_json(const nlohmann::json& j, NrppaInformation& o)
{
    j.at("nfId").get_to(o.m_NfId);
    j.at("nrppaPdu").get_to(o.m_NrppaPdu);
    if(j.find("serviceInstanceId") != j.end())
    {
        j.at("serviceInstanceId").get_to(o.m_ServiceInstanceId);
        o.m_ServiceInstanceIdIsSet = true;
    } 
}

std::string NrppaInformation::getNfId() const
{
    return m_NfId;
}
void NrppaInformation::setNfId(std::string const& value)
{
    m_NfId = value;
    
}
N2InfoContent NrppaInformation::getNrppaPdu() const
{
    return m_NrppaPdu;
}
void NrppaInformation::setNrppaPdu(N2InfoContent const& value)
{
    m_NrppaPdu = value;
    
}
std::string NrppaInformation::getServiceInstanceId() const
{
    return m_ServiceInstanceId;
}
void NrppaInformation::setServiceInstanceId(std::string const& value)
{
    m_ServiceInstanceId = value;
    m_ServiceInstanceIdIsSet = true;
}
bool NrppaInformation::serviceInstanceIdIsSet() const
{
    return m_ServiceInstanceIdIsSet;
}
void NrppaInformation::unsetServiceInstanceId()
{
    m_ServiceInstanceIdIsSet = false;
}

}
}
}

