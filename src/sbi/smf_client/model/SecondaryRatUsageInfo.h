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
 * SecondaryRatUsageInfo.h
 *
 *
 */

#ifndef OAI_SMF_MODEL_SecondaryRatUsageInfo_H_
#define OAI_SMF_MODEL_SecondaryRatUsageInfo_H_

#include "../ModelBase.h"

#include "VolumeTimedReport.h"
#include "QosFlowUsageReport.h"
#include "RatType.h"
#include <vector>

namespace oai {
namespace smf {
namespace model {

/// <summary>
///
/// </summary>
class SecondaryRatUsageInfo : public ModelBase {
 public:
  SecondaryRatUsageInfo();
  virtual ~SecondaryRatUsageInfo();

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
  /// SecondaryRatUsageInfo members

  /// <summary>
  ///
  /// </summary>
  std::shared_ptr<RatType> getSecondaryRatType() const;

  void setSecondaryRatType(const std::shared_ptr<RatType>& value);

  /// <summary>
  ///
  /// </summary>
  std::vector<std::shared_ptr<QosFlowUsageReport>>& getQosFlowsUsageData();
  bool qosFlowsUsageDataIsSet() const;
  void unsetQosFlowsUsageData();

  void setQosFlowsUsageData(
      const std::vector<std::shared_ptr<QosFlowUsageReport>>& value);

  /// <summary>
  ///
  /// </summary>
  std::vector<std::shared_ptr<VolumeTimedReport>>& getPduSessionUsageData();
  bool pduSessionUsageDataIsSet() const;
  void unsetPduSessionUsageData();

  void setPduSessionUsageData(
      const std::vector<std::shared_ptr<VolumeTimedReport>>& value);

 protected:
  std::shared_ptr<RatType> m_SecondaryRatType;
  std::vector<std::shared_ptr<QosFlowUsageReport>> m_QosFlowsUsageData;
  bool m_QosFlowsUsageDataIsSet;
  std::vector<std::shared_ptr<VolumeTimedReport>> m_PduSessionUsageData;
  bool m_PduSessionUsageDataIsSet;
};

}  // namespace model
}  // namespace smf
}  // namespace oai

#endif /* OAI_SMF_MODEL_SecondaryRatUsageInfo_H_ */
