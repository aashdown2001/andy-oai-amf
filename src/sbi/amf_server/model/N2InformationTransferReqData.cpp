/**
 * Namf_Communication
 * AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "N2InformationTransferReqData.h"

namespace oai {
namespace amf {
namespace model {

N2InformationTransferReqData::N2InformationTransferReqData() {
  m_TaiListIsSet           = false;
  m_RatSelectorIsSet       = false;
  m_EcgiListIsSet          = false;
  m_NcgiListIsSet          = false;
  m_GlobalRanNodeListIsSet = false;
  m_SupportedFeatures      = "";
  m_SupportedFeaturesIsSet = false;
}

N2InformationTransferReqData::~N2InformationTransferReqData() {}

void N2InformationTransferReqData::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const N2InformationTransferReqData& o) {
  j = nlohmann::json();
  if (o.taiListIsSet()) j["taiList"] = o.m_TaiList;
  if (o.ratSelectorIsSet()) j["ratSelector"] = o.m_RatSelector;
  if (o.ecgiListIsSet()) j["ecgiList"] = o.m_EcgiList;
  if (o.ncgiListIsSet()) j["ncgiList"] = o.m_NcgiList;
  if (o.globalRanNodeListIsSet())
    j["globalRanNodeList"] = o.m_GlobalRanNodeList;
  j["n2Information"] = o.m_N2Information;
  if (o.supportedFeaturesIsSet())
    j["supportedFeatures"] = o.m_SupportedFeatures;
}

void from_json(const nlohmann::json& j, N2InformationTransferReqData& o) {
  if (j.find("taiList") != j.end()) {
    j.at("taiList").get_to(o.m_TaiList);
    o.m_TaiListIsSet = true;
  }
  if (j.find("ratSelector") != j.end()) {
    j.at("ratSelector").get_to(o.m_RatSelector);
    o.m_RatSelectorIsSet = true;
  }
  if (j.find("ecgiList") != j.end()) {
    j.at("ecgiList").get_to(o.m_EcgiList);
    o.m_EcgiListIsSet = true;
  }
  if (j.find("ncgiList") != j.end()) {
    j.at("ncgiList").get_to(o.m_NcgiList);
    o.m_NcgiListIsSet = true;
  }
  if (j.find("globalRanNodeList") != j.end()) {
    j.at("globalRanNodeList").get_to(o.m_GlobalRanNodeList);
    o.m_GlobalRanNodeListIsSet = true;
  }
  j.at("n2Information").get_to(o.m_N2Information);
  if (j.find("supportedFeatures") != j.end()) {
    j.at("supportedFeatures").get_to(o.m_SupportedFeatures);
    o.m_SupportedFeaturesIsSet = true;
  }
}

std::vector<Tai>& N2InformationTransferReqData::getTaiList() {
  return m_TaiList;
}
bool N2InformationTransferReqData::taiListIsSet() const {
  return m_TaiListIsSet;
}
void N2InformationTransferReqData::unsetTaiList() {
  m_TaiListIsSet = false;
}
RatSelector N2InformationTransferReqData::getRatSelector() const {
  return m_RatSelector;
}
void N2InformationTransferReqData::setRatSelector(RatSelector const& value) {
  m_RatSelector      = value;
  m_RatSelectorIsSet = true;
}
bool N2InformationTransferReqData::ratSelectorIsSet() const {
  return m_RatSelectorIsSet;
}
void N2InformationTransferReqData::unsetRatSelector() {
  m_RatSelectorIsSet = false;
}
std::vector<Ecgi>& N2InformationTransferReqData::getEcgiList() {
  return m_EcgiList;
}
bool N2InformationTransferReqData::ecgiListIsSet() const {
  return m_EcgiListIsSet;
}
void N2InformationTransferReqData::unsetEcgiList() {
  m_EcgiListIsSet = false;
}
std::vector<Ncgi>& N2InformationTransferReqData::getNcgiList() {
  return m_NcgiList;
}
bool N2InformationTransferReqData::ncgiListIsSet() const {
  return m_NcgiListIsSet;
}
void N2InformationTransferReqData::unsetNcgiList() {
  m_NcgiListIsSet = false;
}
std::vector<GlobalRanNodeId>&
N2InformationTransferReqData::getGlobalRanNodeList() {
  return m_GlobalRanNodeList;
}
bool N2InformationTransferReqData::globalRanNodeListIsSet() const {
  return m_GlobalRanNodeListIsSet;
}
void N2InformationTransferReqData::unsetGlobalRanNodeList() {
  m_GlobalRanNodeListIsSet = false;
}
N2InfoContainer N2InformationTransferReqData::getN2Information() const {
  return m_N2Information;
}
void N2InformationTransferReqData::setN2Information(
    N2InfoContainer const& value) {
  m_N2Information = value;
}
std::string N2InformationTransferReqData::getSupportedFeatures() const {
  return m_SupportedFeatures;
}
void N2InformationTransferReqData::setSupportedFeatures(
    std::string const& value) {
  m_SupportedFeatures      = value;
  m_SupportedFeaturesIsSet = true;
}
bool N2InformationTransferReqData::supportedFeaturesIsSet() const {
  return m_SupportedFeaturesIsSet;
}
void N2InformationTransferReqData::unsetSupportedFeatures() {
  m_SupportedFeaturesIsSet = false;
}

}  // namespace model
}  // namespace amf
}  // namespace oai
