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

#include "NgApCause.h"

namespace oai {
namespace amf {
namespace model {

NgApCause::NgApCause() {
  m_Group = 0;
  m_Value = 0;
}

NgApCause::~NgApCause() {}

void NgApCause::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const NgApCause& o) {
  j          = nlohmann::json();
  j["group"] = o.m_Group;
  j["value"] = o.m_Value;
}

void from_json(const nlohmann::json& j, NgApCause& o) {
  j.at("group").get_to(o.m_Group);
  j.at("value").get_to(o.m_Value);
}

int32_t NgApCause::getGroup() const {
  return m_Group;
}
void NgApCause::setGroup(int32_t const value) {
  m_Group = value;
}
int32_t NgApCause::getValue() const {
  return m_Value;
}
void NgApCause::setValue(int32_t const value) {
  m_Value = value;
}

}  // namespace model
}  // namespace amf
}  // namespace oai
