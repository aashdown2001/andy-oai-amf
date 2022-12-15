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

#ifndef _AMF_CONFIG_H_
#define _AMF_CONFIG_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <boost/algorithm/string.hpp>
#include <libconfig.h++>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "amf.hpp"
#include "amf_config.hpp"
#include "common_defs.h"
#include "if.hpp"
#include "pdu_session_context.hpp"
#include "string.hpp"
#include "thread_sched.hpp"

#define AMF_CONFIG_STRING_AMF_CONFIG "AMF"
#define AMF_CONFIG_STRING_INSTANCE_ID "INSTANCE_ID"
#define AMF_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define AMF_CONFIG_STRING_AMF_NAME "AMF_NAME"
#define AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY "RELATIVE_CAPACITY"
#define AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL "STATISTICS_TIMER_INTERVAL"

#define AMF_CONFIG_STRING_GUAMI "GUAMI"
#define AMF_CONFIG_STRING_SERVED_GUAMI_LIST "SERVED_GUAMI_LIST"
#define AMF_CONFIG_STRING_TAC "TAC"
#define AMF_CONFIG_STRING_MCC "MCC"
#define AMF_CONFIG_STRING_MNC "MNC"
#define AMF_CONFIG_STRING_RegionID "RegionID"
#define AMF_CONFIG_STRING_AMFSetID "AMFSetID"
#define AMF_CONFIG_STRING_AMFPointer "AMFPointer"
#define AMF_CONFIG_STRING_PLMN_SUPPORT_LIST "PLMN_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SLICE_SUPPORT_LIST "SLICE_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SST "SST"
#define AMF_CONFIG_STRING_SD "SD"

#define AMF_CONFIG_STRING_INTERFACES "INTERFACES"
#define AMF_CONFIG_STRING_INTERFACE_NGAP_AMF "NGAP_AMF"
#define AMF_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define AMF_CONFIG_STRING_PORT "PORT"
#define AMF_CONFIG_STRING_PPID "PPID"
#define AMF_CONFIG_STRING_SBI_HTTP2_PORT "HTTP2_PORT"
#define AMF_CONFIG_STRING_API_VERSION "API_VERSION"

#define AMF_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"

#define AMF_CONFIG_STRING_INTERFACE_SBI "SBI"
#define AMF_CONFIG_STRING_SMF_INSTANCES_POOL "SMF_INSTANCES_POOL"
#define AMF_CONFIG_STRING_SMF_INSTANCE_ID "SMF_INSTANCE_ID"
#define AMF_CONFIG_STRING_SMF_INSTANCE_PORT "PORT"
#define AMF_CONFIG_STRING_SMF_INSTANCE_VERSION "VERSION"
#define AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED "SELECTED"

#define AMF_CONFIG_STRING_SMF "SMF"
#define AMF_CONFIG_STRING_NRF "NRF"
#define AMF_CONFIG_STRING_AUSF "AUSF"
#define AMF_CONFIG_STRING_UDM "UDM"
#define AMF_CONFIG_STRING_NSSF "NSSF"
#define AMF_CONFIG_STRING_NF_URI "URI"

#define AMF_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define AMF_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"

#define AMF_CONFIG_STRING_CORE_CONFIGURATION "CORE_CONFIGURATION"
#define AMF_CONFIG_STRING_EMERGENCY_SUPPORT "EMERGENCY_SUPPORT"
#define AMF_CONFIG_STRING_AUTHENTICATION "AUTHENTICATION"
#define AMF_CONFIG_STRING_AUTH_MYSQL_SERVER "MYSQL_server"
#define AMF_CONFIG_STRING_AUTH_MYSQL_USER "MYSQL_user"
#define AMF_CONFIG_STRING_AUTH_MYSQL_PASS "MYSQL_pass"
#define AMF_CONFIG_STRING_AUTH_MYSQL_DB "MYSQL_db"
#define AMF_CONFIG_STRING_AUTH_RANDOM "RANDOM"
#define AMF_CONFIG_STRING_NAS "NAS"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES "SUPPORT_FEATURES"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_NF_REGISTRATION "NF_REGISTRATION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_NRF_SELECTION "NRF_SELECTION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_NRF "EXTERNAL_NRF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION "SMF_SELECTION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF "EXTERNAL_AUSF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM "EXTERNAL_UDM"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_NSSF "EXTERNAL_NSSF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_FQDN_DNS "USE_FQDN_DNS"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_HTTP2 "USE_HTTP2"

#define AMF_CONFIG_STRING_FQDN_DNS "FQDN"

using namespace libconfig;

namespace config {

typedef struct auth_conf_s {
  std::string mysql_server;
  std::string mysql_user;
  std::string mysql_pass;
  std::string mysql_db;
  std::string random;
  nlohmann::json to_json() const {
    nlohmann::json json_data  = {};
    json_data["mysql_server"] = this->mysql_server;
    json_data["mysql_user"]   = this->mysql_user;
    json_data["mysql_pass"]   = this->mysql_pass;
    json_data["mysql_db"]     = this->mysql_db;
    json_data["random"]       = this->random;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mysql_server = json_data["mysql_server"].get<std::string>();
    this->mysql_user   = json_data["mysql_user"].get<std::string>();
    this->mysql_pass   = json_data["mysql_pass"].get<std::string>();
    this->mysql_db     = json_data["mysql_db"].get<std::string>();
    this->random       = json_data["random"].get<std::string>();
  }

} auth_conf_t;

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["if_name"]     = this->if_name;
    json_data["addr4"]       = inet_ntoa(this->addr4);
    json_data["network4"]    = inet_ntoa(this->network4);
    char str_addr6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &this->addr6, str_addr6, sizeof(str_addr6));
    json_data["addr6"] = str_addr6;
    json_data["mtu"]   = this->mtu;
    json_data["port"]  = this->port;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->if_name         = json_data["if_name"].get<std::string>();
    std::string addr4_str = {};
    addr4_str             = json_data["addr4"].get<std::string>();

    if (boost::iequals(addr4_str, "read")) {
      if (get_inet_addr_infos_from_iface(
              this->if_name, this->addr4, this->network4, this->mtu)) {
        Logger::amf_app().error(
            "Could not read %s network interface configuration", this->if_name);
        return;
      }
    } else {
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(addr4_str).c_str(), this->addr4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");

      std::string network4_str = json_data["network4"].get<std::string>();
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(network4_str).c_str(), this->network4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");
      // TODO: addr6
      this->mtu  = json_data["mtu"].get<int>();
      this->port = json_data["port"].get<int>();
    }
  }

} interface_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

class amf_config {
 public:
  amf_config();
  ~amf_config();

  /*
   * Load AMF configuration file
   * @param [const std::string&] config_file: Configuration file
   * @return RETURNclear/RETURNerror/RETURNok
   */
  int load(const std::string& config_file);

  /*
   * Read the network interface configuration
   * @param [const Setting&] if_cfg: Configuration for the network interface
   * @param [interface_cfg_t&] cfg: Interface config
   * @return RETURNclear/RETURNerror/RETURNok
   */
  int load_interface(const Setting& if_cfg, interface_cfg_t& cfg);

  void get_nf_info(const Setting& nf_cfg, nf_addr_t& nf_addr);

  bool resolve_fqdn(const std::string& fqdn, struct in_addr& ipv4_addr);

  /*
   * Get the URI of AMF N1N2MessageSubscribe
   * @param [const std::string&] ue_cxt_id: UE Context Id
   * @return URI in string format
   */
  std::string get_amf_n1n2_message_subscribe_uri(const std::string& ue_cxt_id);

  /*
   * Get the URI of NRF NF Discovery Service
   * @param void
   * @return URI in string format
   */
  std::string get_nrf_nf_discovery_service_uri();

  /*
   * Get the URI of NRF NF Registration Service
   * @param [const std::string&] nf_instance_id: NF instance ID
   * @return URI in string format
   */
  std::string get_nrf_nf_registration_uri(const std::string& nf_instance_id);

  /*
   * Get the URI of UDM Slice Selection Subscription Data Retrieval Service
   * @param [const std::string&] supi: UE SUPI
   * @return URI in string format
   */
  std::string get_udm_slice_selection_subscription_data_retrieval_uri(
      const std::string& supi);

  /*
   * Get the URI of NSSF Network Slice Selection Information Service
   * @param void
   * @return URI in string format
   */
  std::string get_nssf_network_slice_selection_information_uri();

  /*
   * Get the URI of AUSF UE Authentication Service
   * @param void
   * @return URI in string format
   */
  std::string get_ausf_ue_authentications_uri();

  /*
   * Get the URI of SMF PDU Session Service
   * @param [const std::shared_ptr<pdu_session_context>&] psc: pointer to the
   * PDU Session Context
   * @param [std::string&] smf_uri: based URI of Nsmf_PDUSession Services
   * @return true if can get the URI. otherwise false
   */
  bool get_smf_pdu_session_context_uri(
      const std::shared_ptr<pdu_session_context>& psc, std::string& smf_uri);

  /*
   * Get the URI of SMF Services
   * @param [const std::string&] smf_addr: SMF's Addr in String representation
   * @param [const std::string&] smf_port: SMF's port in String representation
   * @param [const std::string&] smf_api_version: SMF's API version in String
   * representation
   * @return URI in string format
   */
  std::string get_smf_pdu_session_base_uri(
      const std::string& smf_addr, const std::string& smf_port,
      const std::string& smf_api_version);

  /*
   * Display the AMF configuration parameters
   * @param void
   * @return void
   */
  void display();

  /*
   * Represent AMF's config as json object
   * @param [nlohmann::json &] json_data: Json data
   * @return void
   */
  void to_json(nlohmann::json& json_data) const;

  /*
   * Update AMF's config from Json
   * @param [nlohmann::json &] json_data: Updated configuration in json format
   * @return true if success otherwise return false
   */
  bool from_json(nlohmann::json& json_data);

  unsigned int instance;
  std::string pid_dir;
  interface_cfg_t n2;
  interface_cfg_t sbi;
  itti_cfg_t itti;
  std::string sbi_api_version;
  unsigned int sbi_http2_port;

  unsigned int statistics_interval;
  std::string amf_name;
  guami_t guami;
  std::vector<guami_t> guami_list;
  unsigned int relative_amf_capacity;
  std::vector<plmn_item_t> plmn_list;
  std::string is_emergency_support;
  auth_conf_t auth_para;
  nas_conf_t nas_cfg;

  struct {
    bool enable_nf_registration;
    bool enable_nrf_selection;
    bool enable_external_nrf;
    bool enable_smf_selection;
    bool enable_external_ausf;
    bool enable_external_udm;
    bool enable_external_nssf;
    bool use_fqdn_dns;
    bool use_http2;
    nlohmann::json to_json() const {
      nlohmann::json json_data            = {};
      json_data["enable_nf_registration"] = this->enable_nf_registration;
      json_data["enable_nrf_selection"]   = this->enable_nrf_selection;
      json_data["enable_external_nrf"]    = this->enable_external_nrf;
      json_data["enable_smf_selection"]   = this->enable_smf_selection;
      json_data["enable_external_ausf"]   = this->enable_external_ausf;
      json_data["enable_external_udm"]    = this->enable_external_udm;
      json_data["enable_external_nssf"]   = this->enable_external_nssf;
      json_data["use_fqdn_dns"]           = this->use_fqdn_dns;
      json_data["use_http2"]              = this->use_http2;
      return json_data;
    }

    void from_json(nlohmann::json& json_data) {
      this->enable_nf_registration =
          json_data["enable_nf_registration"].get<bool>();
      this->enable_external_nrf = json_data["enable_external_nrf"].get<bool>();
      this->enable_smf_selection =
          json_data["enable_smf_selection"].get<bool>();
      this->enable_external_nrf = json_data["enable_external_nrf"].get<bool>();
      this->enable_external_ausf =
          json_data["enable_external_ausf"].get<bool>();
      this->enable_external_udm = json_data["enable_external_udm"].get<bool>();
      this->enable_external_nssf =
          json_data["enable_external_nssf"].get<bool>();
      this->use_fqdn_dns = json_data["use_fqdn_dns"].get<bool>();
      this->use_http2    = json_data["use_http2"].get<bool>();
    }

  } support_features;

  nf_addr_t smf_addr;
  nf_addr_t nrf_addr;
  nf_addr_t ausf_addr;
  nf_addr_t udm_addr;
  nf_addr_t nssf_addr;
};

}  // namespace config

#endif
