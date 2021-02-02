/**
* AUSF API
* AUSF UE Authentication Service. © 2020, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
*
* The version of the OpenAPI document: 1.1.1
*
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/
/*
 * UEAuthenticationCtx.h
 *
 *
 */

#ifndef UEAuthenticationCtx_H_
#define UEAuthenticationCtx_H_


#include "Av5gAka.h"
#include "LinksValueSchema.h"
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class  UEAuthenticationCtx
{
public:
    UEAuthenticationCtx();
    virtual ~UEAuthenticationCtx();

    void validate();

    /////////////////////////////////////////////
    /// UEAuthenticationCtx members

    /// <summary>
    ///
    /// </summary>
    std::string getAuthType() const;
    void setAuthType(std::string const& value);
    /// <summary>
    ///
    /// </summary>
    Av5gAka getR5gAuthData() const;
    void setR5gAuthData(Av5gAka const& value);
    /// <summary>
    ///
    /// </summary>
    std::map<std::string, LinksValueSchema>& getLinks();
    void setLinks(std::map<std::string, LinksValueSchema> const& value);
    /// <summary>
    ///
    /// </summary>
    std::string getServingNetworkName() const;
    void setServingNetworkName(std::string const& value);
    bool servingNetworkNameIsSet() const;
    void unsetServingNetworkName();

    friend void to_json(nlohmann::json& j, const UEAuthenticationCtx& o);
    friend void from_json(const nlohmann::json& j, UEAuthenticationCtx& o);
protected:
    std::string m_AuthType;

    Av5gAka m_r_5gAuthData;

    std::map<std::string, LinksValueSchema> m__links;

    std::string m_ServingNetworkName;
    bool m_ServingNetworkNameIsSet;
};

}
}
}

#endif /* UEAuthenticationCtx_H_ */
