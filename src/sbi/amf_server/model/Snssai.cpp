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


#include "Snssai.h"

namespace oai {
namespace amf {
namespace model {

Snssai::Snssai()
{
    m_Sst = 0;
    m_Sd = "";
    m_SdIsSet = false;
    
}

Snssai::~Snssai()
{
}

void Snssai::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const Snssai& o)
{
    j = nlohmann::json();
    j["sst"] = o.m_Sst;
    if(o.sdIsSet())
        j["sd"] = o.m_Sd;
}

void from_json(const nlohmann::json& j, Snssai& o)
{
    j.at("sst").get_to(o.m_Sst);
    if(j.find("sd") != j.end())
    {
        j.at("sd").get_to(o.m_Sd);
        o.m_SdIsSet = true;
    } 
}

int32_t Snssai::getSst() const
{
    return m_Sst;
}
void Snssai::setSst(int32_t const value)
{
    m_Sst = value;
    
}
std::string Snssai::getSd() const
{
    return m_Sd;
}
void Snssai::setSd(std::string const& value)
{
    m_Sd = value;
    m_SdIsSet = true;
}
bool Snssai::sdIsSet() const
{
    return m_SdIsSet;
}
void Snssai::unsetSd()
{
    m_SdIsSet = false;
}

}
}
}

