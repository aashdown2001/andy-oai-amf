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

#include "TimeToWait.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
TimeToWait::TimeToWait() {
  timeValue = -1;
}

//------------------------------------------------------------------------------
TimeToWait::~TimeToWait() {}

//------------------------------------------------------------------------------
void TimeToWait::setValue(e_Ngap_TimeToWait m_timeToWait) {
  timeValue = m_timeToWait;
}

//------------------------------------------------------------------------------
bool TimeToWait::encode2TimeToWait(Ngap_TimeToWait_t* timeToWait) {
  *timeToWait = timeValue;
  return true;
}

//------------------------------------------------------------------------------
bool TimeToWait::decodefromTimeToWait(Ngap_TimeToWait_t* pdu) {
  timeValue = *pdu;
  return true;
}

//------------------------------------------------------------------------------
long TimeToWait::getValue() {
  return timeValue;
}
}  // namespace ngap
