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
 * N2InformationTransferResult.h
 *
 *
 */

#ifndef N2InformationTransferResult_H_
#define N2InformationTransferResult_H_

#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class N2InformationTransferResult {
 public:
  N2InformationTransferResult();
  virtual ~N2InformationTransferResult();

  void validate();

  /////////////////////////////////////////////
  /// N2InformationTransferResult members

  friend void to_json(nlohmann::json& j, const N2InformationTransferResult& o);
  friend void from_json(
      const nlohmann::json& j, N2InformationTransferResult& o);

 protected:
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* N2InformationTransferResult_H_ */
