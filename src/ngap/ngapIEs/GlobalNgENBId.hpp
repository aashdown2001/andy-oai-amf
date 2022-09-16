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

#ifndef _GLOBAL_NG_ENB_ID_H
#define _GLOBAL_NG_ENB_ID_H

#include "NgENBId.hpp"
#include "PlmnId.hpp"

extern "C" {
#include "Ngap_GlobalNgENB-ID.h"
}

namespace ngap {

class GlobalNgENBId {
 public:
  GlobalNgENBId();
  virtual ~GlobalNgENBId();

  bool encode(Ngap_GlobalNgENB_ID_t&);
  bool decode(Ngap_GlobalNgENB_ID_t&);

  void set(const PlmnId& plmn, const NgENB_ID& ng_enb_id);
  void get(PlmnId& plmn, NgENB_ID& ng_enb_id) const;

 private:
  PlmnId plmn_id_;      // Mandatory
  NgENB_ID ng_enb_id_;  // Mandatory
};

}  // namespace ngap

#endif
