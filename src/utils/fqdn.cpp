/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "fqdn.hpp"
#include "logger.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#define MAX_NB_RESOLVE_TRIES 4
#define TIME_BETWEEN_TRIES 2

//------------------------------------------------------------------------------
bool fqdn::resolve(
    const std::string& host_name, std::string& address, uint32_t& port,
    FqdnAddressType& addr_type, const std::string& protocol) {
  int tries = 0;
  Logger::amf_app().debug("Resolving a DNS (name %s)", host_name.c_str());
  while (tries < MAX_NB_RESOLVE_TRIES) {
    try {
      boost::asio::io_context io_context = {};
      Logger::amf_app().debug("Resolving DNS Try #%u", tries + 1);

      boost::asio::ip::tcp::resolver resolver{io_context};
      boost::asio::ip::tcp::resolver::results_type endpoints =
          resolver.resolve(host_name, protocol);

      addr_type = FqdnAddressType::kIPv4;  // IPv4 by default
      for (auto it = endpoints.cbegin(); it != endpoints.cend(); it++) {
        // get the first Endpoint
        boost::asio::ip::tcp::endpoint endpoint = *it;
        address = endpoint.address().to_string();
        port    = endpoint.port();
        Logger::amf_app().debug(
            "Resolved a DNS (name %s, protocol %s): Ip Addr %s, port %u",
            host_name.c_str(), protocol.c_str(), address.c_str(), port);
        if (endpoint.address().is_v4())
          addr_type = FqdnAddressType::kIPv4;
        else
          addr_type = FqdnAddressType::kIPv6;
        return true;
      }
    } catch (std::exception& e) {
      tries++;
      if (tries == MAX_NB_RESOLVE_TRIES) {
        throw std::runtime_error(
            "Cannot resolve a DNS name " + std::string(e.what()) + " after " +
            std::to_string(tries) + " tries");
        return false;
      }
      std::this_thread::sleep_for(std::chrono::seconds(TIME_BETWEEN_TRIES));
    }
  }

  return false;
}

//------------------------------------------------------------------------------
bool fqdn::resolve(nf_addr_t& nf_addr) {
  Logger::amf_app().debug("Resolving an FQDN Addr for a NF node");

  // Don't need to do if IP addr already available
  // May change later since we can do it multiple times in runtime
  if (nf_addr.ipv4_addr.s_addr != INADDR_ANY) {
    Logger::amf_app().debug(
        "IP Addr is already available, do not need to do resolve!");
    return true;
  }
  // Resolve if FQDN available
  if (nf_addr.fqdn.empty()) {
    Logger::amf_app().debug("FQDN info is not available!");
    return false;
  }

  Logger::amf_app().debug("FQDN %s", nf_addr.fqdn.c_str());
  std::string ip_addr_str   = {};
  uint32_t port             = {0};
  FqdnAddressType addr_type = FqdnAddressType::kIPv4;

  if (!fqdn::resolve(nf_addr.fqdn, ip_addr_str, port, addr_type)) {
    Logger::amf_app().warn(
        "Resolve FQDN %s: cannot resolve the hostname!", nf_addr.fqdn.c_str());
    return false;
  }
  switch (addr_type) {
    case FqdnAddressType::kIPv4: {
      // TODO: use a common function to convert from string to addr
      unsigned char buf_in_addr[sizeof(struct in6_addr)];  // you never know...
      if (inet_pton(AF_INET, ip_addr_str.c_str(), buf_in_addr) == 1) {
        memcpy(&nf_addr.ipv4_addr, buf_in_addr, sizeof(struct in_addr));
        Logger::amf_app().debug(
            "Resolve FQDN %s, IP Addr %s", nf_addr.fqdn.c_str(),
            ip_addr_str.c_str());
        return true;
      }
    } break;
    case FqdnAddressType::kIPv6: {
      // TODO
      Logger::amf_app().warn(
          "Resolve FQDN: %s. IPv6 Addr, this mode has not been "
          "supported yet!",
          nf_addr.fqdn.c_str());
      return false;
    } break;
    default:
      Logger::amf_app().warn("Unknown Address type");
      return false;
  }

  return true;
}
