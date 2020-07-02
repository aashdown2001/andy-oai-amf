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
 * SmContextMessage.h
 *
 * 
 */

#ifndef OAI_SMF_MODEL_SmContextMessage_H_
#define OAI_SMF_MODEL_SmContextMessage_H_


#include "../ModelBase.h"

#include "SmContextCreateData.h"
#include "HttpContent.h"

namespace oai {
namespace smf {
namespace model {


/// <summary>
/// 
/// </summary>
class  SmContextMessage
    : public ModelBase
{
public:
	SmContextMessage();
    virtual ~SmContextMessage();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    void fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    void fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SmContextMessage members

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<SmContextCreateData> getJsonData() const;
    bool jsonDataIsSet() const;
    void unsetJsonData();

    void setJsonData(const std::shared_ptr<SmContextCreateData>& value);

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<HttpContent> getBinaryDataN1SmMessage() const;
    bool binaryDataN1SmMessageIsSet() const;
    void unsetBinaryDataN1SmMessage();

    void setBinaryDataN1SmMessage(const std::shared_ptr<HttpContent>& value);

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<HttpContent> getBinaryDataN2SmInformation() const;
    bool binaryDataN2SmInformationIsSet() const;
    void unsetBinaryDataN2SmInformation();

    void setBinaryDataN2SmInformation(const std::shared_ptr<HttpContent>& value);


protected:
    std::shared_ptr<SmContextCreateData> m_JsonData;
    bool m_JsonDataIsSet;
    std::shared_ptr<HttpContent> m_BinaryDataN1SmMessage;
    bool m_BinaryDataN1SmMessageIsSet;
    std::shared_ptr<HttpContent> m_BinaryDataN2SmInformation;
    bool m_BinaryDataN2SmInformationIsSet;
};


}
}
}

#endif /* OAI_SMF_MODEL_SmContextMessage_H_ */
