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
#ifndef _SOR_TRANSPARENT_CONTAINER_H_
#define _SOR_TRANSPARENT_CONTAINER_H_

#include "Type6NasIe.hpp"
#include <stdint.h>

constexpr uint8_t kSorTransparentContainerMinimumLength   = 20;
constexpr uint8_t kSorTransparentContainerIeMinimumLength = 17;
constexpr auto kSorTransparentContainerIeName = "SOR Transparent Container";

namespace nas {

class SOR_Transparent_Container : Type6NasIe {
 public:
  SOR_Transparent_Container();
  SOR_Transparent_Container(uint8_t header, const uint8_t (&value)[16]);
  ~SOR_Transparent_Container();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void getValue(uint8_t (&value)[16]) const;

 private:
  uint8_t header_;
  uint8_t sor_mac_i_[16];
  std::optional<uint8_t> counter_;
  // Other IEs
};

}  // namespace nas

#endif
