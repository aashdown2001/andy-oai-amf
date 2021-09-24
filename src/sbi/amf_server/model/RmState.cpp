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

#include "RmState.h"
#include "Helpers.h"

#include <sstream>

namespace oai::amf::model {

RmState::RmState() {}

void RmState::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::amf::helpers::ValidationException(msg.str());
  }
}

bool RmState::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool RmState::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success                  = true;
  const std::string _pathPrefix = pathPrefix.empty() ? "RmState" : pathPrefix;

  return success;
}

bool RmState::operator==(const RmState& rhs) const {
  return true;  // TODO

  ;
}

bool RmState::operator!=(const RmState& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const RmState& o) {
  j = nlohmann::json();
}

void from_json(const nlohmann::json& j, RmState& o) {}

}  // namespace oai::amf::model