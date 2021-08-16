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

#include "NrLocation.h"
#include "Helpers.h"

#include <sstream>

namespace oai::amf::model {

NrLocation::NrLocation() {
  m_AgeOfLocationInformation      = 0;
  m_AgeOfLocationInformationIsSet = false;
  m_UeLocationTimestamp           = "";
  m_UeLocationTimestampIsSet      = false;
  m_GeographicalInformation       = "";
  m_GeographicalInformationIsSet  = false;
  m_GeodeticInformation           = "";
  m_GeodeticInformationIsSet      = false;
  m_GlobalGnbIdIsSet              = false;
}

void NrLocation::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw org::openapitools::server::helpers::ValidationException(msg.str());
  }
}

bool NrLocation::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool NrLocation::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "NrLocation" : pathPrefix;

  if (ageOfLocationInformationIsSet()) {
    const int32_t& value = m_AgeOfLocationInformation;
    const std::string currentValuePath =
        _pathPrefix + ".ageOfLocationInformation";

    if (value < 0) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
    if (value > 32767) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 32767;";
    }
  }

  if (geographicalInformationIsSet()) {
    const std::string& value = m_GeographicalInformation;
    const std::string currentValuePath =
        _pathPrefix + ".geographicalInformation";
  }

  if (geodeticInformationIsSet()) {
    const std::string& value           = m_GeodeticInformation;
    const std::string currentValuePath = _pathPrefix + ".geodeticInformation";
  }

  return success;
}

bool NrLocation::operator==(const NrLocation& rhs) const {
  return

      (getTai() == rhs.getTai()) &&

      (getNcgi() == rhs.getNcgi()) &&

      ((!ageOfLocationInformationIsSet() &&
        !rhs.ageOfLocationInformationIsSet()) ||
       (ageOfLocationInformationIsSet() &&
        rhs.ageOfLocationInformationIsSet() &&
        getAgeOfLocationInformation() == rhs.getAgeOfLocationInformation())) &&

      ((!ueLocationTimestampIsSet() && !rhs.ueLocationTimestampIsSet()) ||
       (ueLocationTimestampIsSet() && rhs.ueLocationTimestampIsSet() &&
        getUeLocationTimestamp() == rhs.getUeLocationTimestamp())) &&

      ((!geographicalInformationIsSet() &&
        !rhs.geographicalInformationIsSet()) ||
       (geographicalInformationIsSet() && rhs.geographicalInformationIsSet() &&
        getGeographicalInformation() == rhs.getGeographicalInformation())) &&

      ((!geodeticInformationIsSet() && !rhs.geodeticInformationIsSet()) ||
       (geodeticInformationIsSet() && rhs.geodeticInformationIsSet() &&
        getGeodeticInformation() == rhs.getGeodeticInformation())) &&

      ((!globalGnbIdIsSet() && !rhs.globalGnbIdIsSet()) ||
       (globalGnbIdIsSet() && rhs.globalGnbIdIsSet() &&
        getGlobalGnbId() == rhs.getGlobalGnbId()))

          ;
}

bool NrLocation::operator!=(const NrLocation& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const NrLocation& o) {
  j         = nlohmann::json();
  j["tai"]  = o.m_Tai;
  j["ncgi"] = o.m_Ncgi;
  if (o.ageOfLocationInformationIsSet())
    j["ageOfLocationInformation"] = o.m_AgeOfLocationInformation;
  if (o.ueLocationTimestampIsSet())
    j["ueLocationTimestamp"] = o.m_UeLocationTimestamp;
  if (o.geographicalInformationIsSet())
    j["geographicalInformation"] = o.m_GeographicalInformation;
  if (o.geodeticInformationIsSet())
    j["geodeticInformation"] = o.m_GeodeticInformation;
  if (o.globalGnbIdIsSet()) j["globalGnbId"] = o.m_GlobalGnbId;
}

void from_json(const nlohmann::json& j, NrLocation& o) {
  j.at("tai").get_to(o.m_Tai);
  j.at("ncgi").get_to(o.m_Ncgi);
  if (j.find("ageOfLocationInformation") != j.end()) {
    j.at("ageOfLocationInformation").get_to(o.m_AgeOfLocationInformation);
    o.m_AgeOfLocationInformationIsSet = true;
  }
  if (j.find("ueLocationTimestamp") != j.end()) {
    j.at("ueLocationTimestamp").get_to(o.m_UeLocationTimestamp);
    o.m_UeLocationTimestampIsSet = true;
  }
  if (j.find("geographicalInformation") != j.end()) {
    j.at("geographicalInformation").get_to(o.m_GeographicalInformation);
    o.m_GeographicalInformationIsSet = true;
  }
  if (j.find("geodeticInformation") != j.end()) {
    j.at("geodeticInformation").get_to(o.m_GeodeticInformation);
    o.m_GeodeticInformationIsSet = true;
  }
  if (j.find("globalGnbId") != j.end()) {
    j.at("globalGnbId").get_to(o.m_GlobalGnbId);
    o.m_GlobalGnbIdIsSet = true;
  }
}

Tai NrLocation::getTai() const {
  return m_Tai;
}
void NrLocation::setTai(Tai const& value) {
  m_Tai = value;
}
Ncgi NrLocation::getNcgi() const {
  return m_Ncgi;
}
void NrLocation::setNcgi(Ncgi const& value) {
  m_Ncgi = value;
}
int32_t NrLocation::getAgeOfLocationInformation() const {
  return m_AgeOfLocationInformation;
}
void NrLocation::setAgeOfLocationInformation(int32_t const value) {
  m_AgeOfLocationInformation      = value;
  m_AgeOfLocationInformationIsSet = true;
}
bool NrLocation::ageOfLocationInformationIsSet() const {
  return m_AgeOfLocationInformationIsSet;
}
void NrLocation::unsetAgeOfLocationInformation() {
  m_AgeOfLocationInformationIsSet = false;
}
std::string NrLocation::getUeLocationTimestamp() const {
  return m_UeLocationTimestamp;
}
void NrLocation::setUeLocationTimestamp(std::string const& value) {
  m_UeLocationTimestamp      = value;
  m_UeLocationTimestampIsSet = true;
}
bool NrLocation::ueLocationTimestampIsSet() const {
  return m_UeLocationTimestampIsSet;
}
void NrLocation::unsetUeLocationTimestamp() {
  m_UeLocationTimestampIsSet = false;
}
std::string NrLocation::getGeographicalInformation() const {
  return m_GeographicalInformation;
}
void NrLocation::setGeographicalInformation(std::string const& value) {
  m_GeographicalInformation      = value;
  m_GeographicalInformationIsSet = true;
}
bool NrLocation::geographicalInformationIsSet() const {
  return m_GeographicalInformationIsSet;
}
void NrLocation::unsetGeographicalInformation() {
  m_GeographicalInformationIsSet = false;
}
std::string NrLocation::getGeodeticInformation() const {
  return m_GeodeticInformation;
}
void NrLocation::setGeodeticInformation(std::string const& value) {
  m_GeodeticInformation      = value;
  m_GeodeticInformationIsSet = true;
}
bool NrLocation::geodeticInformationIsSet() const {
  return m_GeodeticInformationIsSet;
}
void NrLocation::unsetGeodeticInformation() {
  m_GeodeticInformationIsSet = false;
}
GlobalRanNodeId NrLocation::getGlobalGnbId() const {
  return m_GlobalGnbId;
}
void NrLocation::setGlobalGnbId(GlobalRanNodeId const& value) {
  m_GlobalGnbId      = value;
  m_GlobalGnbIdIsSet = true;
}
bool NrLocation::globalGnbIdIsSet() const {
  return m_GlobalGnbIdIsSet;
}
void NrLocation::unsetGlobalGnbId() {
  m_GlobalGnbIdIsSet = false;
}

}  // namespace oai::amf::model
