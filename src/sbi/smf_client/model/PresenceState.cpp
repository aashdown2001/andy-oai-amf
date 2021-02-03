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

#include "PresenceState.h"

namespace oai {
namespace smf {
namespace model {

PresenceState::PresenceState() {}

PresenceState::~PresenceState() {}

void PresenceState::validate() {
  // TODO: implement validation
}

web::json::value PresenceState::toJson() const {
  web::json::value val = web::json::value::object();

  return val;
}

void PresenceState::fromJson(const web::json::value& val) {}

void PresenceState::toMultipart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) const {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }
}

void PresenceState::fromMultiPart(
    std::shared_ptr<MultipartFormData> multipart,
    const utility::string_t& prefix) {
  utility::string_t namePrefix = prefix;
  if (namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) !=
                                   utility::conversions::to_string_t(".")) {
    namePrefix += utility::conversions::to_string_t(".");
  }
}

}  // namespace model
}  // namespace smf
}  // namespace oai
