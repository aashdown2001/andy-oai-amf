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

#include "EutraLocation.h"
#include "Helpers.h"

#include <sstream>

namespace oai::amf::model {

EutraLocation::EutraLocation() {
  m_IgnoreEcgi                    = false;
  m_IgnoreEcgiIsSet               = false;
  m_AgeOfLocationInformation      = 0;
  m_AgeOfLocationInformationIsSet = false;
  m_UeLocationTimestamp           = "";
  m_UeLocationTimestampIsSet      = false;
  m_GeographicalInformation       = "";
  m_GeographicalInformationIsSet  = false;
  m_GeodeticInformation           = "";
  m_GeodeticInformationIsSet      = false;
  m_GlobalNgenbIdIsSet            = false;
  m_GlobalENbIdIsSet              = false;
}

void EutraLocation::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw org::openapitools::server::helpers::ValidationException(msg.str());
  }
}

bool EutraLocation::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool EutraLocation::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "EutraLocation" : pathPrefix;

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

bool EutraLocation::operator==(const EutraLocation& rhs) const {
  return

      (getTai() == rhs.getTai()) &&

      (getEcgi() == rhs.getEcgi()) &&

      ((!ignoreEcgiIsSet() && !rhs.ignoreEcgiIsSet()) ||
       (ignoreEcgiIsSet() && rhs.ignoreEcgiIsSet() &&
        isIgnoreEcgi() == rhs.isIgnoreEcgi())) &&

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

      ((!globalNgenbIdIsSet() && !rhs.globalNgenbIdIsSet()) ||
       (globalNgenbIdIsSet() && rhs.globalNgenbIdIsSet() &&
        getGlobalNgenbId() == rhs.getGlobalNgenbId())) &&

      ((!globalENbIdIsSet() && !rhs.globalENbIdIsSet()) ||
       (globalENbIdIsSet() && rhs.globalENbIdIsSet() &&
        getGlobalENbId() == rhs.getGlobalENbId()))

          ;
}

bool EutraLocation::operator!=(const EutraLocation& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const EutraLocation& o) {
  j         = nlohmann::json();
  j["tai"]  = o.m_Tai;
  j["ecgi"] = o.m_Ecgi;
  if (o.ignoreEcgiIsSet()) j["ignoreEcgi"] = o.m_IgnoreEcgi;
  if (o.ageOfLocationInformationIsSet())
    j["ageOfLocationInformation"] = o.m_AgeOfLocationInformation;
  if (o.ueLocationTimestampIsSet())
    j["ueLocationTimestamp"] = o.m_UeLocationTimestamp;
  if (o.geographicalInformationIsSet())
    j["geographicalInformation"] = o.m_GeographicalInformation;
  if (o.geodeticInformationIsSet())
    j["geodeticInformation"] = o.m_GeodeticInformation;
  if (o.globalNgenbIdIsSet()) j["globalNgenbId"] = o.m_GlobalNgenbId;
  if (o.globalENbIdIsSet()) j["globalENbId"] = o.m_GlobalENbId;
}

void from_json(const nlohmann::json& j, EutraLocation& o) {
  j.at("tai").get_to(o.m_Tai);
  j.at("ecgi").get_to(o.m_Ecgi);
  if (j.find("ignoreEcgi") != j.end()) {
    j.at("ignoreEcgi").get_to(o.m_IgnoreEcgi);
    o.m_IgnoreEcgiIsSet = true;
  }
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
  if (j.find("globalNgenbId") != j.end()) {
    j.at("globalNgenbId").get_to(o.m_GlobalNgenbId);
    o.m_GlobalNgenbIdIsSet = true;
  }
  if (j.find("globalENbId") != j.end()) {
    j.at("globalENbId").get_to(o.m_GlobalENbId);
    o.m_GlobalENbIdIsSet = true;
  }
}

Tai EutraLocation::getTai() const {
  return m_Tai;
}
void EutraLocation::setTai(Tai const& value) {
  m_Tai = value;
}
Ecgi EutraLocation::getEcgi() const {
  return m_Ecgi;
}
void EutraLocation::setEcgi(Ecgi const& value) {
  m_Ecgi = value;
}
bool EutraLocation::isIgnoreEcgi() const {
  return m_IgnoreEcgi;
}
void EutraLocation::setIgnoreEcgi(bool const value) {
  m_IgnoreEcgi      = value;
  m_IgnoreEcgiIsSet = true;
}
bool EutraLocation::ignoreEcgiIsSet() const {
  return m_IgnoreEcgiIsSet;
}
void EutraLocation::unsetIgnoreEcgi() {
  m_IgnoreEcgiIsSet = false;
}
int32_t EutraLocation::getAgeOfLocationInformation() const {
  return m_AgeOfLocationInformation;
}
void EutraLocation::setAgeOfLocationInformation(int32_t const value) {
  m_AgeOfLocationInformation      = value;
  m_AgeOfLocationInformationIsSet = true;
}
bool EutraLocation::ageOfLocationInformationIsSet() const {
  return m_AgeOfLocationInformationIsSet;
}
void EutraLocation::unsetAgeOfLocationInformation() {
  m_AgeOfLocationInformationIsSet = false;
}
std::string EutraLocation::getUeLocationTimestamp() const {
  return m_UeLocationTimestamp;
}
void EutraLocation::setUeLocationTimestamp(std::string const& value) {
  m_UeLocationTimestamp      = value;
  m_UeLocationTimestampIsSet = true;
}
bool EutraLocation::ueLocationTimestampIsSet() const {
  return m_UeLocationTimestampIsSet;
}
void EutraLocation::unsetUeLocationTimestamp() {
  m_UeLocationTimestampIsSet = false;
}
std::string EutraLocation::getGeographicalInformation() const {
  return m_GeographicalInformation;
}
void EutraLocation::setGeographicalInformation(std::string const& value) {
  m_GeographicalInformation      = value;
  m_GeographicalInformationIsSet = true;
}
bool EutraLocation::geographicalInformationIsSet() const {
  return m_GeographicalInformationIsSet;
}
void EutraLocation::unsetGeographicalInformation() {
  m_GeographicalInformationIsSet = false;
}
std::string EutraLocation::getGeodeticInformation() const {
  return m_GeodeticInformation;
}
void EutraLocation::setGeodeticInformation(std::string const& value) {
  m_GeodeticInformation      = value;
  m_GeodeticInformationIsSet = true;
}
bool EutraLocation::geodeticInformationIsSet() const {
  return m_GeodeticInformationIsSet;
}
void EutraLocation::unsetGeodeticInformation() {
  m_GeodeticInformationIsSet = false;
}
GlobalRanNodeId EutraLocation::getGlobalNgenbId() const {
  return m_GlobalNgenbId;
}
void EutraLocation::setGlobalNgenbId(GlobalRanNodeId const& value) {
  m_GlobalNgenbId      = value;
  m_GlobalNgenbIdIsSet = true;
}
bool EutraLocation::globalNgenbIdIsSet() const {
  return m_GlobalNgenbIdIsSet;
}
void EutraLocation::unsetGlobalNgenbId() {
  m_GlobalNgenbIdIsSet = false;
}
GlobalRanNodeId EutraLocation::getGlobalENbId() const {
  return m_GlobalENbId;
}
void EutraLocation::setGlobalENbId(GlobalRanNodeId const& value) {
  m_GlobalENbId      = value;
  m_GlobalENbIdIsSet = true;
}
bool EutraLocation::globalENbIdIsSet() const {
  return m_GlobalENbIdIsSet;
}
void EutraLocation::unsetGlobalENbId() {
  m_GlobalENbIdIsSet = false;
}

}  // namespace oai::amf::model
