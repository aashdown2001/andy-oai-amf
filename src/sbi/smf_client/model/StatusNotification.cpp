/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.1.1-SNAPSHOT.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#include "StatusNotification.h"

namespace oai {
namespace smf {
namespace model {

StatusNotification::StatusNotification() {}

StatusNotification::~StatusNotification() {}

void StatusNotification::validate() {
  // TODO: implement validation
}

web::json::value StatusNotification::toJson() const {
  web::json::value val = web::json::value::object();

  val[utility::conversions::to_string_t("statusInfo")] =
      ModelBase::toJson(m_StatusInfo);

  return val;
}

void StatusNotification::fromJson(const web::json::value& val) {
  std::shared_ptr<StatusInfo> newStatusInfo(new StatusInfo());
  newStatusInfo->fromJson(
      val.at(utility::conversions::to_string_t("statusInfo")));
  setStatusInfo(newStatusInfo);
}

void StatusNotification::toMultipart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) const {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }

  m_StatusInfo->toMultipart(
      multipart, utility::conversions::to_string_t("statusInfo."));
}

void StatusNotification::fromMultiPart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }

  std::shared_ptr<StatusInfo> newStatusInfo(new StatusInfo());
  newStatusInfo->fromMultiPart(
      multipart, utility::conversions::to_string_t("statusInfo."));
  setStatusInfo(newStatusInfo);
}

std::shared_ptr<StatusInfo> StatusNotification::getStatusInfo() const {
  return m_StatusInfo;
}

void StatusNotification::setStatusInfo(
    const std::shared_ptr<StatusInfo>& value) {
  m_StatusInfo = value;
}

}  // namespace model
}  // namespace smf
}  // namespace oai
