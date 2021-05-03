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

#include "Time_Zone.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Time_Zone::Time_Zone(uint8_t iei) {
  _iei   = iei;
  _value = 0;
}

//------------------------------------------------------------------------------
Time_Zone::Time_Zone(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
Time_Zone::Time_Zone() : _iei(), _value() {}

//------------------------------------------------------------------------------
Time_Zone::~Time_Zone() {}

//------------------------------------------------------------------------------
void Time_Zone::setIEI(uint8_t iei) {
  _iei = iei;
}

//------------------------------------------------------------------------------
void Time_Zone::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t Time_Zone::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int Time_Zone::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Time_Zone IEI (0x%x)", _iei);
  if (len < 3) {
    Logger::nas_mm().error("Length is less than 3");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 1;  // Length
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;
    Logger::nas_mm().debug(
        "Encoded Time_Zone _value (0x%x), IEI (0x%x)",
        *(buf + encoded_size - 1), _iei);
  } else {
    *(buf + encoded_size) = 1;  // Length
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;

    Logger::nas_mm().debug("Encoded Time_Zone _value (0x%x)", _value);
  }
  Logger::nas_mm().debug("Encoded Time_Zone len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Time_Zone::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding Time_Zone IEI (0x%x)", *buf);
  int decoded_size = 0;
  _value           = 0x00;

  if (is_option) {
    _iei = *buf;
    decoded_size++;
  }
  decoded_size++;  // Length
  _value = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "Decoded Time_Zone _value(0x%x), IEI (0x%x)", _value, _iei);
  Logger::nas_mm().debug("Decoded Time_Zone len (%d)", decoded_size);
  return decoded_size;
}
