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


#include "N1MessageNotification.h"

namespace oai {
namespace amf {
namespace model {

N1MessageNotification::N1MessageNotification()
{
    m_N1NotifySubscriptionId = "";
    m_N1NotifySubscriptionIdIsSet = false;
    m_LcsCorrelationId = "";
    m_LcsCorrelationIdIsSet = false;
    m_RegistrationCtxtContainerIsSet = false;
    
}

N1MessageNotification::~N1MessageNotification()
{
}

void N1MessageNotification::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const N1MessageNotification& o)
{
    j = nlohmann::json();
    if(o.n1NotifySubscriptionIdIsSet())
        j["n1NotifySubscriptionId"] = o.m_N1NotifySubscriptionId;
    j["n1MessageContainer"] = o.m_N1MessageContainer;
    if(o.lcsCorrelationIdIsSet())
        j["lcsCorrelationId"] = o.m_LcsCorrelationId;
    if(o.registrationCtxtContainerIsSet())
        j["registrationCtxtContainer"] = o.m_RegistrationCtxtContainer;
}

void from_json(const nlohmann::json& j, N1MessageNotification& o)
{
    if(j.find("n1NotifySubscriptionId") != j.end())
    {
        j.at("n1NotifySubscriptionId").get_to(o.m_N1NotifySubscriptionId);
        o.m_N1NotifySubscriptionIdIsSet = true;
    } 
    j.at("n1MessageContainer").get_to(o.m_N1MessageContainer);
    if(j.find("lcsCorrelationId") != j.end())
    {
        j.at("lcsCorrelationId").get_to(o.m_LcsCorrelationId);
        o.m_LcsCorrelationIdIsSet = true;
    } 
    if(j.find("registrationCtxtContainer") != j.end())
    {
        j.at("registrationCtxtContainer").get_to(o.m_RegistrationCtxtContainer);
        o.m_RegistrationCtxtContainerIsSet = true;
    } 
}

std::string N1MessageNotification::getN1NotifySubscriptionId() const
{
    return m_N1NotifySubscriptionId;
}
void N1MessageNotification::setN1NotifySubscriptionId(std::string const& value)
{
    m_N1NotifySubscriptionId = value;
    m_N1NotifySubscriptionIdIsSet = true;
}
bool N1MessageNotification::n1NotifySubscriptionIdIsSet() const
{
    return m_N1NotifySubscriptionIdIsSet;
}
void N1MessageNotification::unsetN1NotifySubscriptionId()
{
    m_N1NotifySubscriptionIdIsSet = false;
}
N1MessageContainer N1MessageNotification::getN1MessageContainer() const
{
    return m_N1MessageContainer;
}
void N1MessageNotification::setN1MessageContainer(N1MessageContainer const& value)
{
    m_N1MessageContainer = value;
    
}
std::string N1MessageNotification::getLcsCorrelationId() const
{
    return m_LcsCorrelationId;
}
void N1MessageNotification::setLcsCorrelationId(std::string const& value)
{
    m_LcsCorrelationId = value;
    m_LcsCorrelationIdIsSet = true;
}
bool N1MessageNotification::lcsCorrelationIdIsSet() const
{
    return m_LcsCorrelationIdIsSet;
}
void N1MessageNotification::unsetLcsCorrelationId()
{
    m_LcsCorrelationIdIsSet = false;
}
RegistrationContextContainer N1MessageNotification::getRegistrationCtxtContainer() const
{
    return m_RegistrationCtxtContainer;
}
void N1MessageNotification::setRegistrationCtxtContainer(RegistrationContextContainer const& value)
{
    m_RegistrationCtxtContainer = value;
    m_RegistrationCtxtContainerIsSet = true;
}
bool N1MessageNotification::registrationCtxtContainerIsSet() const
{
    return m_RegistrationCtxtContainerIsSet;
}
void N1MessageNotification::unsetRegistrationCtxtContainer()
{
    m_RegistrationCtxtContainerIsSet = false;
}

}
}
}

