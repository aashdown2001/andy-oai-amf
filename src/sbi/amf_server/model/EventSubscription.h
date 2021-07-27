/**
 * Namf_EventExposure
 * Session Management Event Exposure Service. � 2019, 3GPP Organizational
 * Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * EventSubscription.h
 *
 *
 */

#ifndef EventSubscription_H_
#define EventSubscription_H_

#include "DnaiChangeType.h"
#include "DddTrafficDescriptor.h"
#include "DddStatus.h"
#include "AmfEvent.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class EventSubscription {
 public:
  EventSubscription();
  virtual ~EventSubscription();

  void validate();

  /////////////////////////////////////////////
  /// EventSubscription members

  /// <summary>
  ///
  /// </summary>
  AmfEvent getEvent() const;
  void setEvent(AmfEvent const& value);
  /// <summary>
  ///
  /// </summary>
  DnaiChangeType getDnaiChgType() const;
  void setDnaiChgType(DnaiChangeType const& value);
  bool dnaiChgTypeIsSet() const;
  void unsetDnaiChgType();
  /// <summary>
  ///
  /// </summary>
  DddTrafficDescriptor getDddTraDes() const;
  void setDddTraDes(DddTrafficDescriptor const& value);
  bool dddTraDesIsSet() const;
  void unsetDddTraDes();
  /// <summary>
  ///
  /// </summary>
  std::vector<DddStatus>& getDddStati();
  void setDddStati(std::vector<DddStatus> const& value);
  bool dddStatiIsSet() const;
  void unsetDddStati();

  friend void to_json(nlohmann::json& j, const EventSubscription& o);
  friend void from_json(const nlohmann::json& j, EventSubscription& o);

 protected:
  AmfEvent m_Event;

  DnaiChangeType m_DnaiChgType;
  bool m_DnaiChgTypeIsSet;
  DddTrafficDescriptor m_DddTraDes;
  bool m_DddTraDesIsSet;
  std::vector<DddStatus> m_DddStati;
  bool m_DddStatiIsSet;
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* EventSubscription_H_ */
