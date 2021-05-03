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

/*! \file
 \brief
 \author
 \date 2021
 \email: contact@openairinterface.org
 */

#ifndef __Network_Name_H_
#define __Network_Name_H_

#include <stdint.h>

extern "C" {
#include "bstrlib.h"
}

namespace nas {

class Network_Name {
 public:
  Network_Name();
  Network_Name(uint8_t iei);
  Network_Name(const uint8_t iei, uint8_t oct3);
  ~Network_Name();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);

  void getValue(bstring& text_string);
  void getValue(uint8_t& oct3);

 private:
  uint8_t _iei;
  uint32_t length;
  union {
    struct {
      uint8_t ext : 1;
      uint8_t coding_scheme : 3;
      uint8_t add_CI : 1;
      uint8_t num_spare_bits : 3;
    } u_oct3;
    uint8_t oct3;
  } _u1;

  bstring _text_string;
};

}  // namespace nas

#endif
