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

#ifndef _SECURITY_HEADER_TYPE_H_
#define _SECURITY_HEADER_TYPE_H_

#include <stdint.h>

namespace nas {

class SecurityHeaderType {
 public:
  SecurityHeaderType();
  virtual ~SecurityHeaderType();

  void Set(const uint8_t& secu_header_type, const uint8_t& spare = 0);
  void Get(uint8_t& secu_header_type) const;
  uint8_t Get() const;

  int Encode(uint8_t* buf, const uint32_t& len);
  int Decode(const uint8_t* const buf, const uint32_t& len);

 private:
  uint8_t spare_ : 4;
  uint8_t secu_header_type_ : 4;
};

}  // namespace nas

#endif
