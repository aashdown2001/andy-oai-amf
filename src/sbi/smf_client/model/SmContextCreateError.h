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
 * SmContextCreateError.h
 *
 *
 */

#ifndef OAI_SMF_MODEL_SmContextCreateError_H_
#define OAI_SMF_MODEL_SmContextCreateError_H_

#include "../ModelBase.h"

#include "ProblemDetails_2.h"
#include "RefToBinaryData.h"
#include <cpprest/details/basic_types.h>

namespace oai {
namespace smf {
namespace model {

/// <summary>
///
/// </summary>
class SmContextCreateError : public ModelBase {
 public:
  SmContextCreateError();
  virtual ~SmContextCreateError();

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
  /// SmContextCreateError members

  /// <summary>
  ///
  /// </summary>
  std::shared_ptr<ProblemDetails_2> getError() const;

  void setError(const std::shared_ptr<ProblemDetails_2>& value);

  /// <summary>
  ///
  /// </summary>
  std::shared_ptr<RefToBinaryData> getN1SmMsg() const;
  bool n1SmMsgIsSet() const;
  void unsetN1SmMsg();

  void setN1SmMsg(const std::shared_ptr<RefToBinaryData>& value);

  /// <summary>
  ///
  /// </summary>
  utility::datetime getRecoveryTime() const;
  bool recoveryTimeIsSet() const;
  void unsetRecoveryTime();

  void setRecoveryTime(const utility::datetime& value);

 protected:
  std::shared_ptr<ProblemDetails_2> m_Error;
  std::shared_ptr<RefToBinaryData> m_N1SmMsg;
  bool m_N1SmMsgIsSet;
  utility::datetime m_RecoveryTime;
  bool m_RecoveryTimeIsSet;
};

}  // namespace model
}  // namespace smf
}  // namespace oai

#endif /* OAI_SMF_MODEL_SmContextCreateError_H_ */
