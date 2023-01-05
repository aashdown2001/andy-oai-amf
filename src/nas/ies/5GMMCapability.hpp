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

#ifndef _5GMM_CAPABILITY_H_
#define _5GMM_CAPABILITY_H_

#include "Type4NasIe.hpp"

#include <optional>
#include <stdint.h>

constexpr uint8_t k5gmmCapabilityMinimumLength = 3;
constexpr uint8_t k5gmmCapabilityMaximumLength = 15;
constexpr auto k5gmmCapabilityIeName           = "5GMM Capability";

namespace nas {

class _5GMMCapability : public Type4NasIe {
 public:
  _5GMMCapability();
  _5GMMCapability(const uint8_t iei, uint8_t octet3);
  ~_5GMMCapability();

  void setOctet3(const uint8_t iei, uint8_t octet3);
  uint8_t getOctet3() const;

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option = true);

 private:
  uint8_t octet3_;  // minimum length of 3 octets
  std::optional<uint8_t> octet4_;
  std::optional<uint8_t> octet5_;
};

}  // namespace nas

#endif
