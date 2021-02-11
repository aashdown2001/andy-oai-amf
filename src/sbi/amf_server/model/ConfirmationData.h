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
 * ConfirmationData.h
 *
 *
 */

#ifndef ConfirmationData_H_
#define ConfirmationData_H_


#include <string>
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class  ConfirmationData
{
public:
    ConfirmationData();
    virtual ~ConfirmationData();

    void validate();

    /////////////////////////////////////////////
    /// ConfirmationData members

    /// <summary>
    ///
    /// </summary>
    std::string getResStar() const;
    void setResStar(std::string const& value);

    friend void to_json(nlohmann::json& j, const ConfirmationData& o);
    friend void from_json(const nlohmann::json& j, ConfirmationData& o);
protected:
    std::string m_ResStar;

};

}
}
}

#endif /* ConfirmationData_H_ */