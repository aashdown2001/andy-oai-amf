/**
 * Namf_EventExposure
 * AMF Event Exposure Service © 2022, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved. 
 *
 * The version of the OpenAPI document: 1.1.7
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 6.0.1.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "LossOfConnectivityReason_anyOf.h"
#include "Helpers.h"
#include <stdexcept>
#include <sstream>

namespace oai::amf::model {


LossOfConnectivityReason_anyOf::LossOfConnectivityReason_anyOf() {}

void LossOfConnectivityReason_anyOf::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::amf::helpers::ValidationException(msg.str());
  }
}

bool LossOfConnectivityReason_anyOf::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool LossOfConnectivityReason_anyOf::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "LossOfConnectivityReason_anyOf" : pathPrefix;

  if (m_value == LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::
                     INVALID_VALUE_OPENAPI_GENERATED) {
    success = false;
    msg << _pathPrefix << ": has no value;";
  }

  return success;
}

bool LossOfConnectivityReason_anyOf::operator==(const LossOfConnectivityReason_anyOf& rhs) const {
  return getValue() == rhs.getValue();
}

bool LossOfConnectivityReason_anyOf::operator!=(const LossOfConnectivityReason_anyOf& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const LossOfConnectivityReason_anyOf& o) {
  j = nlohmann::json();

  switch (o.getValue()) {
    case LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::
        INVALID_VALUE_OPENAPI_GENERATED:
      j = "INVALID_VALUE_OPENAPI_GENERATED";
      break;
    case LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::DEREGISTERED:
      j = "DEREGISTERED";
      break;
    case LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::MAX_DETECTION_TIME_EXPIRED:
      j = "MAX_DETECTION_TIME_EXPIRED";
      break;
    case LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::PURGED:
      j = "PURGED";
      break;
  }
}

void from_json(const nlohmann::json& j, LossOfConnectivityReason_anyOf& o) {
  auto s = j.get<std::string>();
  if (s == "DEREGISTERED") {
    o.setValue(LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::DEREGISTERED);
  } else if (s == "MAX_DETECTION_TIME_EXPIRED") {
    o.setValue(LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::MAX_DETECTION_TIME_EXPIRED);
  } else if (s == "PURGED") {
    o.setValue(LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf::PURGED);
  } else {
    std::stringstream ss;
    ss << "Unexpected value " << s << " in json"
       << " cannot be converted to enum of type"
       << " LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf";
    throw std::invalid_argument(ss.str());
  }
}

LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf LossOfConnectivityReason_anyOf::getValue()
    const {
   return m_value;
}
void LossOfConnectivityReason_anyOf::setValue(
    LossOfConnectivityReason_anyOf::eLossOfConnectivityReason_anyOf value) {
   m_value = value;
}

}  // namespace oai::amf::model
