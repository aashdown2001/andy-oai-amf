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



#include "QosFlowUsageReport.h"

namespace oai {
namespace smf {
namespace model {




QosFlowUsageReport::QosFlowUsageReport()
{
    m_Qfi = 0;
    m_StartTimeStamp = utility::datetime();
    m_EndTimeStamp = utility::datetime();
    m_DownlinkVolume = 0L;
    m_UplinkVolume = 0L;
}

QosFlowUsageReport::~QosFlowUsageReport()
{
}

void QosFlowUsageReport::validate()
{
    // TODO: implement validation
}

web::json::value QosFlowUsageReport::toJson() const
{
    web::json::value val = web::json::value::object();

    val[utility::conversions::to_string_t("qfi")] = ModelBase::toJson(m_Qfi);
    val[utility::conversions::to_string_t("startTimeStamp")] = ModelBase::toJson(m_StartTimeStamp);
    val[utility::conversions::to_string_t("endTimeStamp")] = ModelBase::toJson(m_EndTimeStamp);
    val[utility::conversions::to_string_t("downlinkVolume")] = ModelBase::toJson(m_DownlinkVolume);
    val[utility::conversions::to_string_t("uplinkVolume")] = ModelBase::toJson(m_UplinkVolume);

    return val;
}

void QosFlowUsageReport::fromJson(const web::json::value& val)
{
    setQfi(ModelBase::int32_tFromJson(val.at(utility::conversions::to_string_t("qfi"))));
    setStartTimeStamp
    (ModelBase::dateFromJson(val.at(utility::conversions::to_string_t("startTimeStamp"))));
    setEndTimeStamp
    (ModelBase::dateFromJson(val.at(utility::conversions::to_string_t("endTimeStamp"))));
    setDownlinkVolume(ModelBase::int64_tFromJson(val.at(utility::conversions::to_string_t("downlinkVolume"))));
    setUplinkVolume(ModelBase::int64_tFromJson(val.at(utility::conversions::to_string_t("uplinkVolume"))));
}

void QosFlowUsageReport::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("qfi"), m_Qfi));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("startTimeStamp"), m_StartTimeStamp));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("endTimeStamp"), m_EndTimeStamp));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("downlinkVolume"), m_DownlinkVolume));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("uplinkVolume"), m_UplinkVolume));
}

void QosFlowUsageReport::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    setQfi(ModelBase::int32_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("qfi"))));
    setStartTimeStamp(ModelBase::dateFromHttpContent(multipart->getContent(utility::conversions::to_string_t("startTimeStamp"))));
    setEndTimeStamp(ModelBase::dateFromHttpContent(multipart->getContent(utility::conversions::to_string_t("endTimeStamp"))));
    setDownlinkVolume(ModelBase::int64_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("downlinkVolume"))));
    setUplinkVolume(ModelBase::int64_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("uplinkVolume"))));
}

int32_t QosFlowUsageReport::getQfi() const
{
    return m_Qfi;
}

void QosFlowUsageReport::setQfi(int32_t value)
{
    m_Qfi = value;
    
}

utility::datetime QosFlowUsageReport::getStartTimeStamp() const
{
    return m_StartTimeStamp;
}

void QosFlowUsageReport::setStartTimeStamp(const utility::datetime& value)
{
    m_StartTimeStamp = value;
    
}

utility::datetime QosFlowUsageReport::getEndTimeStamp() const
{
    return m_EndTimeStamp;
}

void QosFlowUsageReport::setEndTimeStamp(const utility::datetime& value)
{
    m_EndTimeStamp = value;
    
}

int64_t QosFlowUsageReport::getDownlinkVolume() const
{
    return m_DownlinkVolume;
}

void QosFlowUsageReport::setDownlinkVolume(int64_t value)
{
    m_DownlinkVolume = value;
    
}

int64_t QosFlowUsageReport::getUplinkVolume() const
{
    return m_UplinkVolume;
}

void QosFlowUsageReport::setUplinkVolume(int64_t value)
{
    m_UplinkVolume = value;
    
}

}
}
}


