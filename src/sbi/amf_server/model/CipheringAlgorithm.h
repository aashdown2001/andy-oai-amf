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
 * CipheringAlgorithm.h
 *
 * 
 */

#ifndef CipheringAlgorithm_H_
#define CipheringAlgorithm_H_


#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
/// 
/// </summary>
class  CipheringAlgorithm
{
public:
    CipheringAlgorithm();
    virtual ~CipheringAlgorithm();

    void validate();

    /////////////////////////////////////////////
    /// CipheringAlgorithm members


    friend void to_json(nlohmann::json& j, const CipheringAlgorithm& o);
    friend void from_json(const nlohmann::json& j, CipheringAlgorithm& o);
protected:
};

}
}
}

#endif /* CipheringAlgorithm_H_ */
