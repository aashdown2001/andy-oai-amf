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

#ifndef __5GS_TRACKING_AREA_IDENTITY_H_
#define __5GS_TRACKING_AREA_IDENTITY_H_

#include <stdint.h>
#include <string>

constexpr uint8_t k5gsTrackingAreaIdentityLength = 7;

namespace nas {

class _5GS_Tracking_Area_Identity {
 public:
  _5GS_Tracking_Area_Identity();
  _5GS_Tracking_Area_Identity(uint8_t iei);
  _5GS_Tracking_Area_Identity(
      const uint8_t iei, const std::string& mcc, const std::string& mnc,
      const uint32_t& tac);

  ~_5GS_Tracking_Area_Identity();

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);

  void setTAC(const uint32_t& value);
  uint32_t getTAC() const;
  void getTAC(uint32_t& value) const;

  void setMcc(const std::string& mcc);
  void getMcc(std::string& mcc) const;

  void setMnc(const std::string& mnc);
  void getMnc(std::string& mnc) const;

  /*
//TODO: to be removed
  void setMNC_MCC1(uint8_t iei, uint8_t value);
  uint8_t getMNC_MCC1();

  void setMNC_MCC2(uint8_t iei, uint8_t value);
  uint8_t getMNC_MCC2();

  void setMNC_MCC3(uint8_t iei, uint8_t value);
  uint8_t getMNC_MCC3();
*/

 private:
  uint8_t _iei;
  /*uint8_t _MNC_MCC1;
  uint8_t _MNC_MCC2;
  uint8_t _MNC_MCC3;
  uint8_t _TAC1;
  uint8_t _TAC2;
  uint8_t _TAC3;
*/

  std::string mcc_;
  std::string mnc_;
  uint32_t tac_;
};
}  // namespace nas

#endif
