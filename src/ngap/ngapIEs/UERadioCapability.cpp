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

#include "UERadioCapability.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERadioCapability::UERadioCapability() {
  buffer_ = nullptr;
  size_   = -1;
}

//------------------------------------------------------------------------------
UERadioCapability::~UERadioCapability() {}

//------------------------------------------------------------------------------
bool UERadioCapability::encode(Ngap_UERadioCapability_t& ueRadioCapability) {
  int ret = OCTET_STRING_fromBuf(&ueRadioCapability, buffer_, size_);
  if (ret != 0) return false;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapability::decode(Ngap_UERadioCapability_t& ueRadioCapability) {
  buffer_ = (char*) ueRadioCapability.buf;
  size_   = ueRadioCapability.size;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapability::get(uint8_t*& buffer, size_t& size) {
  buffer = (uint8_t*) buffer_;
  size   = size_;
  if (!buffer_) return false;
  if (size_ < 0) return false;

  return true;
}

//------------------------------------------------------------------------------
void UERadioCapability::set(uint8_t* buffer, size_t size) {
  buffer_ = (char*) buffer;
  size_   = size;
}
}  // namespace ngap
