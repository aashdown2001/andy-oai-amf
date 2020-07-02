/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved. 
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.1.1-SNAPSHOT.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * Tai.h
 *
 * 
 */

#ifndef OAI_SMF_MODEL_Tai_H_
#define OAI_SMF_MODEL_Tai_H_


#include "../ModelBase.h"

#include <cpprest/details/basic_types.h>
#include "PlmnId.h"

namespace oai {
namespace smf {
namespace model {


/// <summary>
/// 
/// </summary>
class  Tai
    : public ModelBase
{
public:
    Tai();
    virtual ~Tai();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    void fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    void fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// Tai members

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<PlmnId> getPlmnId() const;
    
    void setPlmnId(const std::shared_ptr<PlmnId>& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTac() const;
    
    void setTac(const utility::string_t& value);


protected:
    std::shared_ptr<PlmnId> m_PlmnId;
        utility::string_t m_Tac;
    };


}
}
}

#endif /* OAI_SMF_MODEL_Tai_H_ */
