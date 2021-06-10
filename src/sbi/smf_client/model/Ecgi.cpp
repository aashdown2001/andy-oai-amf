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

#include "Ecgi.h"

namespace oai {
namespace smf {
namespace model {

Ecgi::Ecgi() {
  m_EutraCellId = utility::conversions::to_string_t("");
}

Ecgi::~Ecgi() {}

void Ecgi::validate() {
  // TODO: implement validation
}

web::json::value Ecgi::toJson() const {
  web::json::value val = web::json::value::object();

  val[utility::conversions::to_string_t("plmnId")] =
      ModelBase::toJson(m_PlmnId);
  val[utility::conversions::to_string_t("eutraCellId")] =
      ModelBase::toJson(m_EutraCellId);

  return val;
}

void Ecgi::fromJson(const web::json::value& val) {
  std::shared_ptr<PlmnId> newPlmnId(new PlmnId());
  newPlmnId->fromJson(val.at(utility::conversions::to_string_t("plmnId")));
  setPlmnId(newPlmnId);
  setEutraCellId(ModelBase::stringFromJson(
      val.at(utility::conversions::to_string_t("eutraCellId"))));
}

void Ecgi::toMultipart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) const {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }

  m_PlmnId->toMultipart(
      multipart, utility::conversions::to_string_t("plmnId."));
  multipart->add(ModelBase::toHttpContent(
      namePrefix + utility::conversions::to_string_t("eutraCellId"),
      m_EutraCellId));
}

void Ecgi::fromMultiPart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }

  std::shared_ptr<PlmnId> newPlmnId(new PlmnId());
  newPlmnId->fromMultiPart(
      multipart, utility::conversions::to_string_t("plmnId."));
  setPlmnId(newPlmnId);
  setEutraCellId(ModelBase::stringFromHttpContent(
      multipart->getContent(utility::conversions::to_string_t("eutraCellId"))));
}

std::shared_ptr<PlmnId> Ecgi::getPlmnId() const {
  return m_PlmnId;
}

void Ecgi::setPlmnId(const std::shared_ptr<PlmnId>& value) {
  m_PlmnId = value;
}

utility::string_t Ecgi::getEutraCellId() const {
  return m_EutraCellId;
}

void Ecgi::setEutraCellId(const utility::string_t& value) {
  m_EutraCellId = value;
}

}  // namespace model
}  // namespace smf
}  // namespace oai
