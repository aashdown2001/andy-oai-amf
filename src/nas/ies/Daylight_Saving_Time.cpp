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

#include "Daylight_Saving_Time.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Daylight_Saving_Time::Daylight_Saving_Time(uint8_t iei) : _value() {
  _iei = iei;
}

//------------------------------------------------------------------------------
Daylight_Saving_Time::Daylight_Saving_Time(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
Daylight_Saving_Time::Daylight_Saving_Time() : _iei(), _value() {}

//------------------------------------------------------------------------------
Daylight_Saving_Time::~Daylight_Saving_Time() {}

//------------------------------------------------------------------------------
uint8_t Daylight_Saving_Time::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int Daylight_Saving_Time::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Daylight_Saving_Time IEI (0x%x)", _iei);
  if (len < 3) {
    Logger::nas_mm().error("len is less than 3");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 3;
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;
    return encoded_size;
  } else {
    *(buf + encoded_size) = 3;
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;
    return encoded_size;
  }
  Logger::nas_mm().debug("Encoded Daylight_Saving_Time len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Daylight_Saving_Time::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding Daylight_Saving_Time IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {  // IEI
    decoded_size++;
  }
  decoded_size++;  // Length
  _value = *(buf + decoded_size);
  decoded_size++;

  Logger::nas_mm().debug("Decoded Daylight_Saving_Time value (0x%2x)", _value);

  Logger::nas_mm().debug("Decoded Daylight_Saving_Time len (%d)", decoded_size);
  return decoded_size;
}
