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

#ifndef __AMF_HPP
#define __AMF_HPP

#include "3gpp_23.003.h"
#include "conversions.hpp"
#include "inttypes.h"
#include "stdio.h"
#include "string.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>

// for CURL
constexpr auto CURL_MIME_BOUNDARY = "----Boundary";
#define CURL_TIMEOUT_MS 1000L

#define BUFFER_SIZE_4096 4096
#define BUFFER_SIZE_2048 2048
#define BUFFER_SIZE_1024 1024
#define BUFFER_SIZE_512 512
#define BUFFER_SIZE_256 256

#define FUTURE_STATUS_TIMEOUT_MS 100

#define GNB_UE_NGAP_ID_FMT "%" PRIu32
#define AMF_UE_NGAP_ID_FMT "%" PRIu64

constexpr long INVALID_AMF_UE_NGAP_ID = -1;

// Event Subscription IDs)
typedef uint32_t evsub_id_t;
#define EVSUB_ID_FMT "0x%" PRIx32
#define EVSUB_ID_SCAN_FMT SCNx32
#define INVALID_EVSUB_ID ((evsub_id_t) 0x00000000)
#define UNASSIGNED_EVSUB_ID ((evsub_id_t) 0x00000000)

typedef uint32_t n1n2sub_id_t;

constexpr uint64_t SECONDS_SINCE_FIRST_EPOCH = 2208988800;

#define UE_AGGREGATE_MAXIMUM_BIT_RATE_DL 1000000000
#define UE_AGGREGATE_MAXIMUM_BIT_RATE_UL 1000000000

#define NAMF_COMMUNICATION_BASE "/namf-comm/"
#define NAMF_COMMUNICATION_N1N2_MESSAGE_TRANSFER_URL                           \
  "/ue-contexts/{}/n1-n2-messages"  // context id

#define NAS_MESSAGE_DOWNLINK 1
#define NAS_MESSAGE_UPLINK 0

typedef enum {
  PlainNasMsg                                              = 0x0,
  IntegrityProtected                                       = 0x1,
  IntegrityProtectedAndCiphered                            = 0x2,
  IntegrityProtectedWithNew5GNASSecurityContext            = 0x3,
  IntegrityProtectedAndCipheredWithNew5GNASSecurityContext = 0x4,
} SecurityHeaderType_t;

constexpr uint32_t DEFAULT_HTTP1_PORT  = 80;
constexpr uint32_t DEFAULT_HTTP2_PORT  = 8080;
constexpr auto DEFAULT_SBI_API_VERSION = "v1";

typedef struct guami_s {
  std::string mcc;
  std::string mnc;
  std::string regionID;
  std::string AmfSetID;
  std::string AmfPointer;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["mcc"]         = this->mcc;
    json_data["mnc"]         = this->mnc;
    json_data["regionID"]    = this->regionID;
    json_data["AmfSetID"]    = this->AmfSetID;
    json_data["AmfPointer"]  = this->AmfPointer;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mcc        = json_data["mcc"].get<std::string>();
    this->mnc        = json_data["mnc"].get<std::string>();
    this->regionID   = json_data["regionID"].get<std::string>();
    this->AmfSetID   = json_data["AmfSetID"].get<std::string>();
    this->AmfPointer = json_data["AmfPointer"].get<std::string>();
  }
} guami_t;

typedef struct slice_s {
  uint8_t sst;
  uint32_t sd;
  bool operator==(const struct slice_s& s) const {
    if ((s.sst == this->sst) && (s.sd == this->sd)) {
      return true;
    } else {
      return false;
    }
  }
  bool operator>(const struct slice_s& s) const {
    if (this->sst > s.sst) return true;
    if (this->sst == s.sst) {
      if (this->sd > s.sd) return true;
      if (this->sd <= s.sd) return false;
    }
  }
  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["sst"]         = this->sst;
    if (this->sd != SD_NO_VALUE) json_data["sd"] = this->sd;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->sst = json_data["sst"].get<int>();
    if (json_data.find("sd") != json_data.end()) {
      this->sd = json_data["sd"].get<int>();
    }
  }
} slice_t;

typedef struct plmn_support_item_s {
  std::string mcc;
  std::string mnc;
  uint32_t tac;
  std::vector<slice_t> slice_list;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["mcc"]         = this->mcc;
    json_data["mnc"]         = this->mnc;
    json_data["tac"]         = this->tac;
    json_data["slice_list"]  = nlohmann::json::array();
    for (auto s : slice_list) {
      json_data["slice_list"].push_back(s.to_json());
    }
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mcc = json_data["mcc"].get<std::string>();
    this->mnc = json_data["mnc"].get<std::string>();
    this->tac = json_data["tac"].get<int>();

    for (auto s : json_data["slice_list"]) {
      slice_t sl = {};
      sl.from_json(s);
      slice_list.push_back(sl);
    }
  }

} plmn_item_t;

typedef struct {
  uint8_t prefered_integrity_algorithm[8];
  uint8_t prefered_ciphering_algorithm[8];

  nlohmann::json to_json() const {
    nlohmann::json json_data                  = {};
    json_data["prefered_integrity_algorithm"] = nlohmann::json::array();
    json_data["prefered_ciphering_algorithm"] = nlohmann::json::array();
    for (auto s : this->prefered_integrity_algorithm) {
      json_data["prefered_integrity_algorithm"].push_back(s);
    }
    for (auto s : this->prefered_ciphering_algorithm) {
      json_data["prefered_ciphering_algorithm"].push_back(s);
    }
    return json_data;
  }
  /*
    void from_json(nlohmann::json& json_data) {
      uint8_t i = 0;
      for (auto s : json_data["prefered_integrity_algorithm"]) {
        uint8_t integ_alg               = s.get<int>();
        prefered_integrity_algorithm[i] = integ_alg;
        ++i;
      }
      i = 0;
      for (auto s : json_data["prefered_ciphering_algorithm"]) {
        uint8_t cipher_alg              = s.get<int>();
        prefered_ciphering_algorithm[i] = cipher_alg;
        ++i;
      }
    }*/
} nas_conf_t;

typedef struct {
  int id;
  std::string ipv4;
  std::string port;
  uint32_t http2_port;
  std::string version;
  bool selected;
  std::string fqdn;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["id"]          = this->id;
    json_data["ipv4"]        = this->ipv4;
    json_data["port"]        = this->port;
    json_data["http2_port"]  = this->http2_port;
    json_data["version"]     = this->version;
    json_data["selected"]    = this->selected;
    json_data["fqdn"]        = this->fqdn;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->id   = json_data["id"].get<int>();
    this->ipv4 = json_data["ipv4"].get<std::string>();
    this->port = json_data["port"]
                     .get<std::string>();  // TODO: use int instead of string
    this->http2_port = json_data["http2_port"].get<int>();
    this->version    = json_data["version"].get<std::string>();
    this->selected   = json_data["selected"].get<bool>();
    this->fqdn       = json_data["fqdn"].get<std::string>();
  }

} smf_inst_t;

typedef struct nf_addr_s {
  struct in_addr ipv4_addr;
  unsigned int port;
  // unsigned int http2_port;
  std::string api_version;
  std::string fqdn;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["ipv4_addr"]   = inet_ntoa(this->ipv4_addr);
    json_data["port"]        = this->port;
    // json_data["http2_port"]  = this->http2_port;
    json_data["api_version"] = this->api_version;
    json_data["fqdn"]        = this->fqdn;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    std::string ipv4_addr_str = json_data["ipv4_addr"].get<std::string>();
    this->ipv4_addr           = conv::fromString(ipv4_addr_str);
    this->port                = json_data["port"].get<int>();
    // this->http2_port  = json_data["http2_port"].get<int>();
    this->api_version = json_data["api_version"].get<std::string>();
    this->fqdn        = json_data["fqdn"].get<std::string>();
  }

} nf_addr_t;
#endif
