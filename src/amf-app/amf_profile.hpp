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

/*! \file amf_profile.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2021
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_AMF_PROFILE_HPP_SEEN
#define FILE_AMF_PROFILE_HPP_SEEN

#include <arpa/inet.h>
#include <netinet/in.h>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <shared_mutex>
#include <utility>
#include <vector>

#include "logger.hpp"
//#include "smf.h"

namespace amf {

using namespace std;
class nf_profile : public std::enable_shared_from_this<nf_profile> {
 public:
  nf_profile()
      : nf_type("NF_TYPE_UNKNOWN"),
        heartBeat_timer(0),
        snssais(),
        ipv4_addresses(),
        priority(0),
        capacity(0) {
    nf_instance_name = "";
    nf_status        = "";
  }

  nf_profile(const std::string& id)
      : nf_instance_id(id),
        heartBeat_timer(0),
        snssais(),
        ipv4_addresses(),
        priority(0),
        capacity(0),
        nf_type("NF_TYPE_UNKNOWN") {
    nf_instance_name = "";
    nf_status        = "";
  }

  nf_profile& operator=(const nf_profile& s) {
    nf_instance_id   = s.nf_instance_id;
    heartBeat_timer  = s.heartBeat_timer;
    snssais          = s.snssais;
    ipv4_addresses   = s.ipv4_addresses;
    priority         = s.priority;
    capacity         = s.capacity;
    nf_type          = s.nf_type;
    nf_instance_name = s.nf_instance_name;
    nf_status        = s.nf_status;
    return *this;
  }
  // nf_profile(nf_profile &b) = delete;

  /*
   * Set NF instance ID
   * @param [const std::string &] instance_id: instance id
   * @return void
   */
  void set_nf_instance_id(const std::string& instance_id);
  /*
   * Get NF instance ID
   * @param [std::string &] instance_id: store instance id
   * @return void:
   */
  void get_nf_instance_id(std::string& instance_id) const;

  /*
   * Get NF instance ID
   * @param [std::string &] instance_id: store instance id
   * @return void:
   */
  std::string get_nf_instance_id() const;

  /*
   * Set NF instance name
   * @param [const std::string &] instance_name: instance name
   * @return void
   */
  void set_nf_instance_name(const std::string& instance_name);

  /*
   * Get NF instance ID
   * @param [std::string &] instance_name: store instance name
   * @return void:
   */
  void get_nf_instance_name(std::string& instance_name) const;

  /*
   * Get NF instance name
   * @param
   * @return [std::string] instance name
   */
  std::string get_nf_instance_name() const;

  /*
   * Set NF instance status
   * @param [const std::string &] status: instance status
   * @return void
   */
  void set_nf_status(const std::string& status);

  /*
   * Get NF instance status
   * @param [std::string &] status: store instance status
   * @return void:
   */
  void get_nf_status(std::string& status) const;

  /*
   * Get NF status
   * @param
   * @return [std::string] instance status
   */
  std::string get_nf_status() const;

  /*
   * Get NF type
   * @param
   * @return [std::string] nf type
   */
  std::string get_nf_type() const;

  /*
   * Set NF type
   * @param [const nf_type_t &] type: nf type
   * @return void
   */
  void set_nf_type(const std::string& type);

  /*
   * Set NF instance heartBeat_timer
   * @param [const std::string &] timer: heartBeat_timer
   * @return void
   */
  void set_nf_heartBeat_timer(const int32_t& timer);

  /*
   * Get NF instance heartBeat_timer
   * @param [std::string &] timer: store heartBeat_timer
   * @return void:
   */
  void get_nf_heartBeat_timer(int32_t& timer) const;

  /*
   * Get NF heartBeat_timer
   * @param void
   * @return heartBeat_timer
   */
  int32_t get_nf_heartBeat_timer() const;

  /*
   * Set NF instance priority
   * @param [const uint16_t] p: instance priority
   * @return void
   */
  void set_nf_priority(const uint16_t& p);

  /*
   * Get NF instance priority
   * @param [uint16_t] p: store instance priority
   * @return void:
   */
  void get_nf_priority(uint16_t& p) const;

  /*
   * Get NF instance priority
   * @param void
   * @return [uint16_t] instance priority
   */
  uint16_t get_nf_priority() const;

  /*
   * Set NF instance capacity
   * @param [const uint16_t] c: instance capacity
   * @return void
   */
  void set_nf_capacity(const uint16_t& c);

  /*
   * Get NF instance priority
   * @param [uint16_t ] c: store instance capacity
   * @return void:
   */
  void get_nf_capacity(uint16_t& c) const;

  /*
   * Get NF instance priority
   * @param void
   * @return [uint16_t ] instance capacity
   */
  uint16_t get_nf_capacity() const;

  /*
   * Set NF instance SNSSAIs
   * @param [std::vector<snssai_t> &] s: SNSSAIs
   * @return void
   */
  void set_nf_snssais(const std::vector<snssai_t>& s);

  /*
   * Add SNSSAI
   * @param [snssai_t &] s: SNSSAI
   * @return void
   */
  void add_snssai(const snssai_t& s);

  /*
   * Get NF instance SNSSAIs
   * @param [std::vector<snssai_t> &] s: store instance's SNSSAIs
   * @return void:
   */
  void get_nf_snssais(std::vector<snssai_t>& s) const;

  /*
   * Set NF instance ipv4_addresses
   * @param [std::vector<struct in_addr> &] a: ipv4_addresses
   * @return void
   */
  void set_nf_ipv4_addresses(const std::vector<struct in_addr>& a);

  /*
   * Add an IPv4 address to the list of addresses
   * @param [const struct in_addr &] a: ipv4_address
   * @return void
   */
  void add_nf_ipv4_addresses(const struct in_addr& a);

  /*
   * Get NF instance ipv4_addresses
   * @param [std::vector<struct in_addr> &] a: store instance's ipv4_addresses
   * @return void:
   */
  void get_nf_ipv4_addresses(std::vector<struct in_addr>& a) const;

  /*
   * Print related-information for NF profile
   * @param void
   * @return void:
   */
  virtual void display() const;

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  virtual void to_json(nlohmann::json& data) const;

  /*
   * Covert from a json represetation to amf profile
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  virtual void from_json(const nlohmann::json& data);

 protected:
  // From NFProfile (Section 6.1.6.2.2@3GPP TS 29.510 V16.0.0 (2019-06))
  std::string nf_instance_id;
  std::string nf_instance_name;
  std::string nf_type;
  std::string nf_status;
  int32_t heartBeat_timer;
  std::vector<snssai_t> snssais;
  std::vector<struct in_addr> ipv4_addresses;
  uint16_t priority;
  uint16_t capacity;
};

typedef struct nf_service_version_s {
  std::string api_version_in_uri;  // apiVersionInUri
  std::string api_full_version;    // apiFullVersion

  nf_service_version_s& operator=(const nf_service_version_s& s) {
    api_version_in_uri = s.api_version_in_uri;
    api_full_version   = s.api_full_version;
    return *this;
  }

  std::string to_string() const {
    std::string s = {};
    s.append(", Version (");
    s.append("apiVersionInUri: ");
    s.append(api_version_in_uri);
    s.append(", apiFullVersion: ");
    s.append(api_full_version);
    s.append(" )");
    return s;
  }
} nf_service_version_t;

typedef struct ip_endpoint_s {
  // struct in6_addr  ipv6_address;
  struct in_addr ipv4_address;
  std::string transport;  // TCP
  unsigned int port;
  std::string to_string() const {
    std::string s = {};
    s.append("Ipv4 Address: ");
    s.append(inet_ntoa(ipv4_address));
    s.append(", TransportProtocol: ");
    s.append(transport);
    s.append(", Port: ");
    s.append(std::to_string(port));
    return s;
  }
} ip_endpoint_t;

typedef struct nf_service_s {
  std::string service_instance_id;
  std::string service_name;
  std::vector<nf_service_version_t> versions;
  std::string scheme;
  std::string nf_service_status;
  std::vector<ip_endpoint_t> ip_endpoints;

  std::string to_string() const {
    std::string s = {};
    s.append("Service Instance ID: ");
    s.append(service_instance_id);
    s.append(", Service name: ");
    s.append(service_name);
    for (auto v : versions) {
      s.append(v.to_string());
    }
    s.append(", Scheme: ");
    s.append(scheme);
    s.append(", Service status: ");
    s.append(nf_service_status);
    s.append(",  IpEndPoints: ");
    for (auto endpoint : ip_endpoints) {
      s.append(endpoint.to_string());
    }
    return s;
  }
} nf_service_t;

typedef struct dnn_amf_info_item_s {
  std::string dnn;
} dnn_amf_info_item_t;

typedef struct snssai_amf_info_item_s {
  snssai_t snssai;
  std::vector<dnn_amf_info_item_t> dnn_amf_info_list;
} snssai_amf_info_item_t;

typedef struct amf_info_s {
  std::vector<snssai_amf_info_item_t> snssai_amf_info_list;
} amf_info_t;

class amf_profile : public nf_profile {
 public:
  amf_profile() : nf_profile() { custom_info = {}; }

  amf_profile(const std::string& id) : nf_profile(id) { custom_info = {}; }

  amf_profile& operator=(const amf_profile& s) {
    nf_instance_id   = s.nf_instance_id;
    heartBeat_timer  = s.heartBeat_timer;
    snssais          = s.snssais;
    ipv4_addresses   = s.ipv4_addresses;
    priority         = s.priority;
    capacity         = s.capacity;
    nf_type          = s.nf_type;
    nf_instance_name = s.nf_instance_name;
    nf_status        = s.nf_status;
    custom_info      = s.custom_info;
    amf_info         = s.amf_info;
    nf_services      = s.nf_services;
    return *this;
  }
  // amf_profile(amf_profile &b) = delete;

  //virtual ~amf_profile() {
   ~amf_profile() {
    Logger::amf_app().debug("Delete amf Profile instance...");
  }

  /*
   * Set NF instance services
   * @param [std::vector<nf_service_t> &] n: nf_service
   * @return void
   */
  void set_nf_services(const std::vector<nf_service_t>& n);

  /*
   * Add nf service
   * @param [snssai_t &] n: nf service
   * @return void
   */
  void add_nf_service(const nf_service_t& n);

  /*
   * Get NF services
   * @param [std::vector<snssai_t> &] n: store instance's nf services
   * @return void:
   */
  void get_nf_services(std::vector<nf_service_t>& n) const;

  /*
   * Set custom info
   * @param [const nlohmann::json &] c: custom info to be set
   * @return void
   */
  void set_custom_info(const nlohmann::json& c);

  /*
   * Get custom info
   * @param [nlohmann::json &] c: Store custom info
   * @return void
   */
  void get_custom_info(nlohmann::json& c) const;

  /*
   * Set amf info
   * @param [amf_info_t &] s: amf info
   * @return void
   */
  void set_amf_info(const amf_info_t& s);

  /*
   * Add an snssai_amf_info_item to the amf info
   * @param [const snssai_amf_info_item_t &] s: snssai_amf_info_item
   * @return void
   */
  void add_amf_info_item(const snssai_amf_info_item_t& s);

  /*
   * Get NF instance amf info
   * @param [amf_info_t &] s: store instance's amf info
   * @return void:
   */
  void get_amf_info(amf_info_t& s) const;

  /*
   * Print related-information for NF profile
   * @param void
   * @return void:
   */
  void display() const;

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void to_json(nlohmann::json& data) const;

  /*
   * Covert from a json represetation to amf profile
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void from_json(const nlohmann::json& data);

  /*
   * Handle heartbeart timeout event
   * @param [uint64_t] ms: current time
   * @return void
   */
  void handle_heartbeart_timeout(uint64_t ms);

 protected:
  std::vector<nf_service_t> nf_services;
  nlohmann::json custom_info;  // store extra json data
  amf_info_t amf_info;
};

// class upf_profile : public nf_profile {
//  public:
//   upf_profile() : nf_profile() {  // custom_info = {};
//     nf_type = "UPF";
//   }

//   upf_profile(const std::string& id) : nf_profile(id) {  // custom_info = {};
//   }

//   upf_profile& operator=(const upf_profile& s) {
//     nf_instance_id   = s.nf_instance_id;
//     heartBeat_timer  = s.heartBeat_timer;
//     snssais          = s.snssais;
//     ipv4_addresses   = s.ipv4_addresses;
//     priority         = s.priority;
//     capacity         = s.capacity;
//     nf_type          = s.nf_type;
//     nf_instance_name = s.nf_instance_name;
//     nf_status        = s.nf_status;
//     // custom_info = s.custom_info;
//     upf_info = s.upf_info;
//     return *this;
//   }
//   // upf_profile(upf_profile &b) = delete;

//   virtual ~upf_profile() {
//     Logger::amf_app().debug("Delete UPF Profile instance...");
//   }

//   /*
//    * Set upf info
//    * @param [upf_info_t &] s: upf info
//    * @return void
//    */
//   void set_upf_info(const upf_info_t& s);

//   /*
//    * Add an snssai_upf_info_item to the upf info
//    * @param [const snssai_upf_info_item_t &] s: snssai_upf_info_item
//    * @return void
//    */
//   void add_upf_info_item(const snssai_upf_info_item_t& s);

//   /*
//    * Get NF instance upf info
//    * @param [upf_info_t &] s: store instance's upf info
//    * @return void:
//    */
//   void get_upf_info(upf_info_t& s) const;

//   /*
//    * Print related-information for NF profile
//    * @param void
//    * @return void:
//    */
//   void display() const;

//   /*
//    * Represent NF profile as json object
//    * @param [nlohmann::json &] data: Json data
//    * @return void
//    */
//   void to_json(nlohmann::json& data) const;

//   /*
//    * Covert from a json represetation to amf profile
//    * @param [nlohmann::json &] data: Json data
//    * @return void
//    */
//   void from_json(const nlohmann::json& data);

//   /*
//    * Handle heartbeart timeout event
//    * @param [uint64_t] ms: current time
//    * @return void
//    */
//   void handle_heartbeart_timeout(uint64_t ms);

//  protected:
//   nlohmann::json custom_info;  // store extra json data
//   upf_info_t upf_info;
// };

}  // namespace amf

#endif
