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

#ifndef _ConfigurationUpdateCommand_H_
#define _ConfigurationUpdateCommand_H_

#include "nas_ie_header.hpp"

namespace nas {

class ConfigurationUpdateCommand {
 public:
  ConfigurationUpdateCommand();
  ~ConfigurationUpdateCommand();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);

 public:
  NasMmPlainHeader* plain_header;
  // Configuration update indication
  // 5G-GUTI
  // TAI list
  // Allowed NSSAI
  // Service area list

  Network_Name* full_network_name;   // Full name for network
  Network_Name* short_network_name;  // Short name for network
  Time_Zone* local_time_zone;
  // Universal time and local time zone
  Daylight_Saving_Time network_dst;  // Network Daylight saving time
  // LADN information
  // MICO indication
  // Network slicing indication
  // Configured NSSAI
  // Rejected NSSAI
  // Operator-defined access category definitions
  // SMS indication
  // T3447 value
};

}  // namespace nas

#endif
