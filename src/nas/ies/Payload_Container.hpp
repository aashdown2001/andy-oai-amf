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

#ifndef __Payload_Container_H_
#define __Payload_Container_H_
#include <stdint.h>

#include <iostream>
#include <vector>

#include "NasIeHeader.hpp"
extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}

constexpr uint8_t kPayloadContainerMinimumLength  = 4;
constexpr uint32_t kPayloadContainerMaximumLength = 65538;

namespace nas {
class Payload_Container {
 public:
  Payload_Container();
  Payload_Container(uint8_t iei);
  Payload_Container(uint8_t iei, bstring b);
  Payload_Container(
      const uint8_t iei, std::vector<PayloadContainerEntry> content);
  ~Payload_Container();
  void setValue(uint8_t iei, uint8_t value);
  int encode2Buffer(uint8_t* buf, int len, uint8_t type);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option, uint8_t type);
  bool getValue(std::vector<PayloadContainerEntry>& content);
  bool getValue(bstring& cnt);

 private:
  uint8_t _iei;
  uint16_t length;
  std::optional<bstring> content;
  std::optional<std::vector<PayloadContainerEntry>> CONTENT;
};

}  // namespace nas

#endif
