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

#include "StandardL3NasIe.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
StandardL3NasIe::StandardL3NasIe() : NasIe() {
  iei_ = std::nullopt;
}

//------------------------------------------------------------------------------
StandardL3NasIe::StandardL3NasIe(const uint8_t& iei) : NasIe() {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
StandardL3NasIe::~StandardL3NasIe() {}

//------------------------------------------------------------------------------
void StandardL3NasIe::SetIei(const uint8_t& iei) {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
bool StandardL3NasIe::Validate(const int& len) const {
  int ie_len = GetIeLength();  // Length of the content + IEI/Len
  if (len < ie_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet(s))",
        ie_len);
    return false;
  }
  return true;
}
