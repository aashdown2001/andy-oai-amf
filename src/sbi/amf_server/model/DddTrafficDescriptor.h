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
 * DddTrafficDescriptor.h
 *
 *
 */

#ifndef DddTrafficDescriptor_H_
#define DddTrafficDescriptor_H_

#include <string>
#include "Ipv6Addr.h"
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class DddTrafficDescriptor {
 public:
  DddTrafficDescriptor();
  virtual ~DddTrafficDescriptor();

  void validate();

  /////////////////////////////////////////////
  /// DddTrafficDescriptor members

  /// <summary>
  ///
  /// </summary>
  std::string getIpv4Addr() const;
  void setIpv4Addr(std::string const& value);
  bool ipv4AddrIsSet() const;
  void unsetIpv4Addr();
  /// <summary>
  ///
  /// </summary>
  Ipv6Addr getIpv6Addr() const;
  void setIpv6Addr(Ipv6Addr const& value);
  bool ipv6AddrIsSet() const;
  void unsetIpv6Addr();
  /// <summary>
  ///
  /// </summary>
  int32_t getPort() const;
  void setPort(int32_t const value);
  bool portIsSet() const;
  void unsetPort();

  friend void to_json(nlohmann::json& j, const DddTrafficDescriptor& o);
  friend void from_json(const nlohmann::json& j, DddTrafficDescriptor& o);

 protected:
  std::string m_Ipv4Addr;
  bool m_Ipv4AddrIsSet;
  Ipv6Addr m_Ipv6Addr;
  bool m_Ipv6AddrIsSet;
  int32_t m_Port;
  bool m_PortIsSet;
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* DddTrafficDescriptor_H_ */
