
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
/*
 * TransData.h
 *
 *
 */

#ifndef _TRANSDATA_H_
#define _TRANSDATA_H_

#include <string>
#include <vector>

#include <stdlib.h>

#include <nlohmann/json.hpp>
#include <iostream>
namespace oai {
namespace amf {
namespace model {

class TransData {
 public:
  TransData();
  virtual ~TransData();

  friend void to_json(nlohmann::json& j, const TransData& o);
  friend void from_json(const nlohmann::json& j, TransData& o);
  std::string get_amf_data();
 protected:
  std::string m_transdata;

};

}  // namespace model
}  // namespace 
}  // namespace oai

#endif /* TransData_H_ */