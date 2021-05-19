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

/*
 * GNbId.h
 *
 *
 */

#ifndef OAI_SMF_MODEL_GNbId_H_
#define OAI_SMF_MODEL_GNbId_H_

#include "../ModelBase.h"

#include <cpprest/details/basic_types.h>

namespace oai {
namespace smf {
namespace model {

/// <summary>
///
/// </summary>
class GNbId : public ModelBase {
 public:
  GNbId();
  virtual ~GNbId();

  /////////////////////////////////////////////
  /// ModelBase overrides

  void validate() override;

  web::json::value toJson() const override;
  void fromJson(const web::json::value& json) override;

  void toMultipart(
      std::shared_ptr<MultipartFormData> multipart,
      const utility::string_t& namePrefix) const override;
  void fromMultiPart(
      std::shared_ptr<MultipartFormData> multipart,
      const utility::string_t& namePrefix) override;

  /////////////////////////////////////////////
  /// GNbId members

  /// <summary>
  ///
  /// </summary>
  int32_t getBitLength() const;

  void setBitLength(int32_t value);

  /// <summary>
  ///
  /// </summary>
  utility::string_t getGNBValue() const;

  void setGNBValue(const utility::string_t& value);

 protected:
  int32_t m_BitLength;
  utility::string_t m_GNBValue;
};

}  // namespace model
}  // namespace smf
}  // namespace oai

#endif /* OAI_SMF_MODEL_GNbId_H_ */
