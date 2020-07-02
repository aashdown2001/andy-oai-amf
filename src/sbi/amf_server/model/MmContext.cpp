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


#include "MmContext.h"

namespace oai {
namespace amf {
namespace model {

MmContext::MmContext()
{
    m_NasSecurityModeIsSet = false;
    m_NasDownlinkCount = 0;
    m_NasDownlinkCountIsSet = false;
    m_NasUplinkCount = 0;
    m_NasUplinkCountIsSet = false;
    m_UeSecurityCapabilityIsSet = false;
    m_S1UeNetworkCapabilityIsSet = false;
    m_AllowedNssaiIsSet = false;
    m_NssaiMappingListIsSet = false;
    m_NsInstanceListIsSet = false;
    m_ExpectedUEbehaviorIsSet = false;
    
}

MmContext::~MmContext()
{
}

void MmContext::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const MmContext& o)
{
    j = nlohmann::json();
    j["accessType"] = o.m_AccessType;
    if(o.nasSecurityModeIsSet())
        j["nasSecurityMode"] = o.m_NasSecurityMode;
    if(o.nasDownlinkCountIsSet())
        j["nasDownlinkCount"] = o.m_NasDownlinkCount;
    if(o.nasUplinkCountIsSet())
        j["nasUplinkCount"] = o.m_NasUplinkCount;
    if(o.ueSecurityCapabilityIsSet())
        j["ueSecurityCapability"] = o.m_UeSecurityCapability;
    if(o.s1UeNetworkCapabilityIsSet())
        j["s1UeNetworkCapability"] = o.m_S1UeNetworkCapability;
    if(o.allowedNssaiIsSet())
        j["allowedNssai"] = o.m_AllowedNssai;
    if(o.nssaiMappingListIsSet())
        j["nssaiMappingList"] = o.m_NssaiMappingList;
    if(o.nsInstanceListIsSet())
        j["nsInstanceList"] = o.m_NsInstanceList;
    if(o.expectedUEbehaviorIsSet())
        j["expectedUEbehavior"] = o.m_ExpectedUEbehavior;
}

void from_json(const nlohmann::json& j, MmContext& o)
{
    j.at("accessType").get_to(o.m_AccessType);
    if(j.find("nasSecurityMode") != j.end())
    {
        j.at("nasSecurityMode").get_to(o.m_NasSecurityMode);
        o.m_NasSecurityModeIsSet = true;
    } 
    if(j.find("nasDownlinkCount") != j.end())
    {
        j.at("nasDownlinkCount").get_to(o.m_NasDownlinkCount);
        o.m_NasDownlinkCountIsSet = true;
    } 
    if(j.find("nasUplinkCount") != j.end())
    {
        j.at("nasUplinkCount").get_to(o.m_NasUplinkCount);
        o.m_NasUplinkCountIsSet = true;
    } 
    if(j.find("ueSecurityCapability") != j.end())
    {
        j.at("ueSecurityCapability").get_to(o.m_UeSecurityCapability);
        o.m_UeSecurityCapabilityIsSet = true;
    } 
    if(j.find("s1UeNetworkCapability") != j.end())
    {
        j.at("s1UeNetworkCapability").get_to(o.m_S1UeNetworkCapability);
        o.m_S1UeNetworkCapabilityIsSet = true;
    } 
    if(j.find("allowedNssai") != j.end())
    {
        j.at("allowedNssai").get_to(o.m_AllowedNssai);
        o.m_AllowedNssaiIsSet = true;
    } 
    if(j.find("nssaiMappingList") != j.end())
    {
        j.at("nssaiMappingList").get_to(o.m_NssaiMappingList);
        o.m_NssaiMappingListIsSet = true;
    } 
    if(j.find("nsInstanceList") != j.end())
    {
        j.at("nsInstanceList").get_to(o.m_NsInstanceList);
        o.m_NsInstanceListIsSet = true;
    } 
    if(j.find("expectedUEbehavior") != j.end())
    {
        j.at("expectedUEbehavior").get_to(o.m_ExpectedUEbehavior);
        o.m_ExpectedUEbehaviorIsSet = true;
    } 
}

AccessType MmContext::getAccessType() const
{
    return m_AccessType;
}
void MmContext::setAccessType(AccessType const& value)
{
    m_AccessType = value;
    
}
NasSecurityMode MmContext::getNasSecurityMode() const
{
    return m_NasSecurityMode;
}
void MmContext::setNasSecurityMode(NasSecurityMode const& value)
{
    m_NasSecurityMode = value;
    m_NasSecurityModeIsSet = true;
}
bool MmContext::nasSecurityModeIsSet() const
{
    return m_NasSecurityModeIsSet;
}
void MmContext::unsetNasSecurityMode()
{
    m_NasSecurityModeIsSet = false;
}
int32_t MmContext::getNasDownlinkCount() const
{
    return m_NasDownlinkCount;
}
void MmContext::setNasDownlinkCount(int32_t const value)
{
    m_NasDownlinkCount = value;
    m_NasDownlinkCountIsSet = true;
}
bool MmContext::nasDownlinkCountIsSet() const
{
    return m_NasDownlinkCountIsSet;
}
void MmContext::unsetNasDownlinkCount()
{
    m_NasDownlinkCountIsSet = false;
}
int32_t MmContext::getNasUplinkCount() const
{
    return m_NasUplinkCount;
}
void MmContext::setNasUplinkCount(int32_t const value)
{
    m_NasUplinkCount = value;
    m_NasUplinkCountIsSet = true;
}
bool MmContext::nasUplinkCountIsSet() const
{
    return m_NasUplinkCountIsSet;
}
void MmContext::unsetNasUplinkCount()
{
    m_NasUplinkCountIsSet = false;
}
std::string MmContext::getUeSecurityCapability() const
{
    return m_UeSecurityCapability;
}
void MmContext::setUeSecurityCapability(std::string const& value)
{
    m_UeSecurityCapability = value;
    m_UeSecurityCapabilityIsSet = true;
}
bool MmContext::ueSecurityCapabilityIsSet() const
{
    return m_UeSecurityCapabilityIsSet;
}
void MmContext::unsetUeSecurityCapability()
{
    m_UeSecurityCapabilityIsSet = false;
}
std::string MmContext::getS1UeNetworkCapability() const
{
    return m_S1UeNetworkCapability;
}
void MmContext::setS1UeNetworkCapability(std::string const& value)
{
    m_S1UeNetworkCapability = value;
    m_S1UeNetworkCapabilityIsSet = true;
}
bool MmContext::s1UeNetworkCapabilityIsSet() const
{
    return m_S1UeNetworkCapabilityIsSet;
}
void MmContext::unsetS1UeNetworkCapability()
{
    m_S1UeNetworkCapabilityIsSet = false;
}
std::vector<Snssai>& MmContext::getAllowedNssai()
{
    return m_AllowedNssai;
}
bool MmContext::allowedNssaiIsSet() const
{
    return m_AllowedNssaiIsSet;
}
void MmContext::unsetAllowedNssai()
{
    m_AllowedNssaiIsSet = false;
}
std::vector<NssaiMapping>& MmContext::getNssaiMappingList()
{
    return m_NssaiMappingList;
}
bool MmContext::nssaiMappingListIsSet() const
{
    return m_NssaiMappingListIsSet;
}
void MmContext::unsetNssaiMappingList()
{
    m_NssaiMappingListIsSet = false;
}
std::vector<std::string>& MmContext::getNsInstanceList()
{
    return m_NsInstanceList;
}
bool MmContext::nsInstanceListIsSet() const
{
    return m_NsInstanceListIsSet;
}
void MmContext::unsetNsInstanceList()
{
    m_NsInstanceListIsSet = false;
}
ExpectedUeBehavior MmContext::getExpectedUEbehavior() const
{
    return m_ExpectedUEbehavior;
}
void MmContext::setExpectedUEbehavior(ExpectedUeBehavior const& value)
{
    m_ExpectedUEbehavior = value;
    m_ExpectedUEbehaviorIsSet = true;
}
bool MmContext::expectedUEbehaviorIsSet() const
{
    return m_ExpectedUEbehaviorIsSet;
}
void MmContext::unsetExpectedUEbehavior()
{
    m_ExpectedUEbehaviorIsSet = false;
}

}
}
}

