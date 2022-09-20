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

#include "NasMessageType.hpp"

#include "3gpp_ts24501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NasMessageType::NasMessageType() {}

//------------------------------------------------------------------------------
NasMessageType::~NasMessageType() {}

//------------------------------------------------------------------------------
void NasMessageType::Set(const uint8_t& message_type) {
  message_type_ = message_type;
}

//------------------------------------------------------------------------------
void NasMessageType::Get(uint8_t& message_type) const {
  message_type = message_type_;
}

//------------------------------------------------------------------------------
uint8_t NasMessageType::Get() const {
  return message_type_;
}

//------------------------------------------------------------------------------
uint32_t NasMessageType::Encode(uint8_t* buf, uint32_t len) {
  if (len < kNasMessageTypeIeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than %d octet", kNasMessageTypeIeSize);
    return KEncodeDecodeError;
  }
  uint32_t encoded_size = 0;
  ENCODE_U8(buf, message_type_, encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
uint32_t NasMessageType::Decode(const uint8_t* const buf, uint32_t len) {
  if (len < kNasMessageTypeIeSize) {
    Logger::nas_mm().error(
        "Buffer length is less than %s octet", kNasMessageTypeIeSize);
    return KEncodeDecodeError;
  }
  uint32_t decoded_size = 0;
  DECODE_U8(buf, message_type_, decoded_size);
  return decoded_size;
}
