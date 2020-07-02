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


#include "RegistrationContextContainer.h"

namespace oai {
namespace amf {
namespace model {

RegistrationContextContainer::RegistrationContextContainer()
{
    m_LocalTimeZone = "";
    m_LocalTimeZoneIsSet = false;
    m_AnN2ApId = 0;
    m_InitialAmfName = "";
    m_RrcEstCause = "";
    m_RrcEstCauseIsSet = false;
    m_UeContextRequest = false;
    m_UeContextRequestIsSet = false;
    m_AnN2IPv4Addr = "";
    m_AnN2IPv4AddrIsSet = false;
    m_AnN2IPv6AddrIsSet = false;
    m_AllowedNssaiIsSet = false;
    m_ConfiguredNssaiIsSet = false;
    m_RejectedNssaiInPlmnIsSet = false;
    m_RejectedNssaiInTaIsSet = false;
    
}

RegistrationContextContainer::~RegistrationContextContainer()
{
}

void RegistrationContextContainer::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const RegistrationContextContainer& o)
{
    j = nlohmann::json();
    j["ueContext"] = o.m_UeContext;
    if(o.localTimeZoneIsSet())
        j["localTimeZone"] = o.m_LocalTimeZone;
    j["anType"] = o.m_AnType;
    j["anN2ApId"] = o.m_AnN2ApId;
    j["ranNodeId"] = o.m_RanNodeId;
    j["initialAmfName"] = o.m_InitialAmfName;
    j["userLocation"] = o.m_UserLocation;
    if(o.rrcEstCauseIsSet())
        j["rrcEstCause"] = o.m_RrcEstCause;
    if(o.ueContextRequestIsSet())
        j["ueContextRequest"] = o.m_UeContextRequest;
    if(o.anN2IPv4AddrIsSet())
        j["anN2IPv4Addr"] = o.m_AnN2IPv4Addr;
    if(o.anN2IPv6AddrIsSet())
        j["anN2IPv6Addr"] = o.m_AnN2IPv6Addr;
    if(o.allowedNssaiIsSet())
        j["allowedNssai"] = o.m_AllowedNssai;
    if(o.configuredNssaiIsSet())
        j["configuredNssai"] = o.m_ConfiguredNssai;
    if(o.rejectedNssaiInPlmnIsSet())
        j["rejectedNssaiInPlmn"] = o.m_RejectedNssaiInPlmn;
    if(o.rejectedNssaiInTaIsSet())
        j["rejectedNssaiInTa"] = o.m_RejectedNssaiInTa;
}

void from_json(const nlohmann::json& j, RegistrationContextContainer& o)
{
    j.at("ueContext").get_to(o.m_UeContext);
    if(j.find("localTimeZone") != j.end())
    {
        j.at("localTimeZone").get_to(o.m_LocalTimeZone);
        o.m_LocalTimeZoneIsSet = true;
    } 
    j.at("anType").get_to(o.m_AnType);
    j.at("anN2ApId").get_to(o.m_AnN2ApId);
    j.at("ranNodeId").get_to(o.m_RanNodeId);
    j.at("initialAmfName").get_to(o.m_InitialAmfName);
    j.at("userLocation").get_to(o.m_UserLocation);
    if(j.find("rrcEstCause") != j.end())
    {
        j.at("rrcEstCause").get_to(o.m_RrcEstCause);
        o.m_RrcEstCauseIsSet = true;
    } 
    if(j.find("ueContextRequest") != j.end())
    {
        j.at("ueContextRequest").get_to(o.m_UeContextRequest);
        o.m_UeContextRequestIsSet = true;
    } 
    if(j.find("anN2IPv4Addr") != j.end())
    {
        j.at("anN2IPv4Addr").get_to(o.m_AnN2IPv4Addr);
        o.m_AnN2IPv4AddrIsSet = true;
    } 
    if(j.find("anN2IPv6Addr") != j.end())
    {
        j.at("anN2IPv6Addr").get_to(o.m_AnN2IPv6Addr);
        o.m_AnN2IPv6AddrIsSet = true;
    } 
    if(j.find("allowedNssai") != j.end())
    {
        j.at("allowedNssai").get_to(o.m_AllowedNssai);
        o.m_AllowedNssaiIsSet = true;
    } 
    if(j.find("configuredNssai") != j.end())
    {
        j.at("configuredNssai").get_to(o.m_ConfiguredNssai);
        o.m_ConfiguredNssaiIsSet = true;
    } 
    if(j.find("rejectedNssaiInPlmn") != j.end())
    {
        j.at("rejectedNssaiInPlmn").get_to(o.m_RejectedNssaiInPlmn);
        o.m_RejectedNssaiInPlmnIsSet = true;
    } 
    if(j.find("rejectedNssaiInTa") != j.end())
    {
        j.at("rejectedNssaiInTa").get_to(o.m_RejectedNssaiInTa);
        o.m_RejectedNssaiInTaIsSet = true;
    } 
}

UeContext RegistrationContextContainer::getUeContext() const
{
    return m_UeContext;
}
void RegistrationContextContainer::setUeContext(UeContext const& value)
{
    m_UeContext = value;
    
}
std::string RegistrationContextContainer::getLocalTimeZone() const
{
    return m_LocalTimeZone;
}
void RegistrationContextContainer::setLocalTimeZone(std::string const& value)
{
    m_LocalTimeZone = value;
    m_LocalTimeZoneIsSet = true;
}
bool RegistrationContextContainer::localTimeZoneIsSet() const
{
    return m_LocalTimeZoneIsSet;
}
void RegistrationContextContainer::unsetLocalTimeZone()
{
    m_LocalTimeZoneIsSet = false;
}
AccessType RegistrationContextContainer::getAnType() const
{
    return m_AnType;
}
void RegistrationContextContainer::setAnType(AccessType const& value)
{
    m_AnType = value;
    
}
int32_t RegistrationContextContainer::getAnN2ApId() const
{
    return m_AnN2ApId;
}
void RegistrationContextContainer::setAnN2ApId(int32_t const value)
{
    m_AnN2ApId = value;
    
}
GlobalRanNodeId RegistrationContextContainer::getRanNodeId() const
{
    return m_RanNodeId;
}
void RegistrationContextContainer::setRanNodeId(GlobalRanNodeId const& value)
{
    m_RanNodeId = value;
    
}
std::string RegistrationContextContainer::getInitialAmfName() const
{
    return m_InitialAmfName;
}
void RegistrationContextContainer::setInitialAmfName(std::string const& value)
{
    m_InitialAmfName = value;
    
}
UserLocation RegistrationContextContainer::getUserLocation() const
{
    return m_UserLocation;
}
void RegistrationContextContainer::setUserLocation(UserLocation const& value)
{
    m_UserLocation = value;
    
}
std::string RegistrationContextContainer::getRrcEstCause() const
{
    return m_RrcEstCause;
}
void RegistrationContextContainer::setRrcEstCause(std::string const& value)
{
    m_RrcEstCause = value;
    m_RrcEstCauseIsSet = true;
}
bool RegistrationContextContainer::rrcEstCauseIsSet() const
{
    return m_RrcEstCauseIsSet;
}
void RegistrationContextContainer::unsetRrcEstCause()
{
    m_RrcEstCauseIsSet = false;
}
bool RegistrationContextContainer::isUeContextRequest() const
{
    return m_UeContextRequest;
}
void RegistrationContextContainer::setUeContextRequest(bool const value)
{
    m_UeContextRequest = value;
    m_UeContextRequestIsSet = true;
}
bool RegistrationContextContainer::ueContextRequestIsSet() const
{
    return m_UeContextRequestIsSet;
}
void RegistrationContextContainer::unsetUeContextRequest()
{
    m_UeContextRequestIsSet = false;
}
std::string RegistrationContextContainer::getAnN2IPv4Addr() const
{
    return m_AnN2IPv4Addr;
}
void RegistrationContextContainer::setAnN2IPv4Addr(std::string const& value)
{
    m_AnN2IPv4Addr = value;
    m_AnN2IPv4AddrIsSet = true;
}
bool RegistrationContextContainer::anN2IPv4AddrIsSet() const
{
    return m_AnN2IPv4AddrIsSet;
}
void RegistrationContextContainer::unsetAnN2IPv4Addr()
{
    m_AnN2IPv4AddrIsSet = false;
}
std::string RegistrationContextContainer::getAnN2IPv6Addr() const
{
    return m_AnN2IPv6Addr;
}
void RegistrationContextContainer::setAnN2IPv6Addr(std::string const& value)
{
    m_AnN2IPv6Addr = value;
    m_AnN2IPv6AddrIsSet = true;
}
bool RegistrationContextContainer::anN2IPv6AddrIsSet() const
{
    return m_AnN2IPv6AddrIsSet;
}
void RegistrationContextContainer::unsetAnN2IPv6Addr()
{
    m_AnN2IPv6AddrIsSet = false;
}
AllowedNssai RegistrationContextContainer::getAllowedNssai() const
{
    return m_AllowedNssai;
}
void RegistrationContextContainer::setAllowedNssai(AllowedNssai const& value)
{
    m_AllowedNssai = value;
    m_AllowedNssaiIsSet = true;
}
bool RegistrationContextContainer::allowedNssaiIsSet() const
{
    return m_AllowedNssaiIsSet;
}
void RegistrationContextContainer::unsetAllowedNssai()
{
    m_AllowedNssaiIsSet = false;
}
std::vector<ConfiguredSnssai>& RegistrationContextContainer::getConfiguredNssai()
{
    return m_ConfiguredNssai;
}
bool RegistrationContextContainer::configuredNssaiIsSet() const
{
    return m_ConfiguredNssaiIsSet;
}
void RegistrationContextContainer::unsetConfiguredNssai()
{
    m_ConfiguredNssaiIsSet = false;
}
std::vector<Snssai>& RegistrationContextContainer::getRejectedNssaiInPlmn()
{
    return m_RejectedNssaiInPlmn;
}
bool RegistrationContextContainer::rejectedNssaiInPlmnIsSet() const
{
    return m_RejectedNssaiInPlmnIsSet;
}
void RegistrationContextContainer::unsetRejectedNssaiInPlmn()
{
    m_RejectedNssaiInPlmnIsSet = false;
}
std::vector<Snssai>& RegistrationContextContainer::getRejectedNssaiInTa()
{
    return m_RejectedNssaiInTa;
}
bool RegistrationContextContainer::rejectedNssaiInTaIsSet() const
{
    return m_RejectedNssaiInTaIsSet;
}
void RegistrationContextContainer::unsetRejectedNssaiInTa()
{
    m_RejectedNssaiInTaIsSet = false;
}

}
}
}

