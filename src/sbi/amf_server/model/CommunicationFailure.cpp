/**
 * Namf_EventExposure
 * AMF Event Exposure Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "CommunicationFailure.h"
#include "Helpers.h"

#include <sstream>

namespace oai::amf::model {

CommunicationFailure::CommunicationFailure() {
  m_NasReleaseCode      = "";
  m_NasReleaseCodeIsSet = false;
  m_RanReleaseCodeIsSet = false;
}

void CommunicationFailure::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw org::openapitools::server::helpers::ValidationException(msg.str());
  }
}

bool CommunicationFailure::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool CommunicationFailure::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "CommunicationFailure" : pathPrefix;

  return success;
}

bool CommunicationFailure::operator==(const CommunicationFailure& rhs) const {
  return

      ((!nasReleaseCodeIsSet() && !rhs.nasReleaseCodeIsSet()) ||
       (nasReleaseCodeIsSet() && rhs.nasReleaseCodeIsSet() &&
        getNasReleaseCode() == rhs.getNasReleaseCode())) &&

      ((!ranReleaseCodeIsSet() && !rhs.ranReleaseCodeIsSet()) ||
       (ranReleaseCodeIsSet() && rhs.ranReleaseCodeIsSet() &&
        getRanReleaseCode() == rhs.getRanReleaseCode()))

          ;
}

bool CommunicationFailure::operator!=(const CommunicationFailure& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const CommunicationFailure& o) {
  j = nlohmann::json();
  if (o.nasReleaseCodeIsSet()) j["nasReleaseCode"] = o.m_NasReleaseCode;
  if (o.ranReleaseCodeIsSet()) j["ranReleaseCode"] = o.m_RanReleaseCode;
}

void from_json(const nlohmann::json& j, CommunicationFailure& o) {
  if (j.find("nasReleaseCode") != j.end()) {
    j.at("nasReleaseCode").get_to(o.m_NasReleaseCode);
    o.m_NasReleaseCodeIsSet = true;
  }
  if (j.find("ranReleaseCode") != j.end()) {
    j.at("ranReleaseCode").get_to(o.m_RanReleaseCode);
    o.m_RanReleaseCodeIsSet = true;
  }
}

std::string CommunicationFailure::getNasReleaseCode() const {
  return m_NasReleaseCode;
}
void CommunicationFailure::setNasReleaseCode(std::string const& value) {
  m_NasReleaseCode      = value;
  m_NasReleaseCodeIsSet = true;
}
bool CommunicationFailure::nasReleaseCodeIsSet() const {
  return m_NasReleaseCodeIsSet;
}
void CommunicationFailure::unsetNasReleaseCode() {
  m_NasReleaseCodeIsSet = false;
}
NgApCause CommunicationFailure::getRanReleaseCode() const {
  return m_RanReleaseCode;
}
void CommunicationFailure::setRanReleaseCode(NgApCause const& value) {
  m_RanReleaseCode      = value;
  m_RanReleaseCodeIsSet = true;
}
bool CommunicationFailure::ranReleaseCodeIsSet() const {
  return m_RanReleaseCodeIsSet;
}
void CommunicationFailure::unsetRanReleaseCode() {
  m_RanReleaseCodeIsSet = false;
}

}  // namespace oai::amf::model
