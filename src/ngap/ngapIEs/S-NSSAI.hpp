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

#ifndef _S_NSSAI_H_
#define _S_NSSAI_H_

#include <optional>

extern "C" {
#include "Ngap_S-NSSAI.h"
#include "Ngap_SD.h"
#include "Ngap_SST.h"
}

namespace ngap {

class S_NSSAI {
 public:
  S_NSSAI();
  virtual ~S_NSSAI();
  bool EncodeSD(Ngap_SD_t*);
  bool DecodeSD(Ngap_SD_t*);
  void setSst(const std::string& sst);
  void setSst(const uint8_t& sst);
  void getSst(std::string& sst) const;
  std::string getSst() const;
  void setSd(const std::string& sd_str);
  void setSd(const uint32_t& sd);
  bool getSd(std::string& sd) const;
  std::string getSd() const;
  bool getSd(uint32_t& s_nssaiSd) const;
  bool encode(Ngap_S_NSSAI_t*);
  bool decode(Ngap_S_NSSAI_t*);

 private:
  uint8_t sst_;  // mandatory  OCTET_STRING(SIZE(1))
                 //  uint32_t sd;  // OCTET_STRING(SIZE(3))
  std::optional<uint32_t> sd_;
  //  bool sdIsSet;
};

}  // namespace ngap

#endif
