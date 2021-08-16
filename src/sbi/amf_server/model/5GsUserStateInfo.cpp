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

#include "5GsUserStateInfo.h"
#include "Helpers.h"

#include <sstream>

namespace oai::amf::model {

_5GsUserStateInfo::_5GsUserStateInfo() {}

void _5GsUserStateInfo::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw org::openapitools::server::helpers::ValidationException(msg.str());
  }
}

bool _5GsUserStateInfo::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool _5GsUserStateInfo::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "5GsUserStateInfo" : pathPrefix;

  return success;
}

bool _5GsUserStateInfo::operator==(const _5GsUserStateInfo& rhs) const {
  return

      (getR5gsUserState() == rhs.getR5gsUserState()) &&

      (getAccessType() == rhs.getAccessType())

          ;
}

bool _5GsUserStateInfo::operator!=(const _5GsUserStateInfo& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const _5GsUserStateInfo& o) {
  j                 = nlohmann::json();
  j["5gsUserState"] = o.m_r_5gsUserState;
  j["accessType"]   = o.m_AccessType;
}

void from_json(const nlohmann::json& j, _5GsUserStateInfo& o) {
  j.at("5gsUserState").get_to(o.m_r_5gsUserState);
  j.at("accessType").get_to(o.m_AccessType);
}

_5GsUserState _5GsUserStateInfo::getR5gsUserState() const {
  return m_r_5gsUserState;
}
void _5GsUserStateInfo::setR5gsUserState(_5GsUserState const& value) {
  m_r_5gsUserState = value;
}
AccessType _5GsUserStateInfo::getAccessType() const {
  return m_AccessType;
}
void _5GsUserStateInfo::setAccessType(AccessType const& value) {
  m_AccessType = value;
}

}  // namespace oai::amf::model
