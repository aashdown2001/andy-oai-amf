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
 * AssignEbiError.h
 *
 * 
 */

#ifndef AssignEbiError_H_
#define AssignEbiError_H_


#include "ProblemDetails_2.h"
#include "AssignEbiFailed.h"
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
/// 
/// </summary>
class  AssignEbiError
{
public:
    AssignEbiError();
    virtual ~AssignEbiError();

    void validate();

    /////////////////////////////////////////////
    /// AssignEbiError members

    /// <summary>
    /// 
    /// </summary>
    ProblemDetails_2 getError() const;
    void setError(ProblemDetails_2 const& value);
        /// <summary>
    /// 
    /// </summary>
    AssignEbiFailed getFailureDetails() const;
    void setFailureDetails(AssignEbiFailed const& value);
    
    friend void to_json(nlohmann::json& j, const AssignEbiError& o);
    friend void from_json(const nlohmann::json& j, AssignEbiError& o);
protected:
    ProblemDetails_2 m_Error;

    AssignEbiFailed m_FailureDetails;

};

}
}
}

#endif /* AssignEbiError_H_ */
