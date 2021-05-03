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

#include "Network_Name.hpp"

#include "logger.hpp"

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
}

using namespace nas;

//------------------------------------------------------------------------------
Network_Name::Network_Name(uint8_t iei) : _text_string() {
  _iei     = iei;
  length   = 1;
  _u1.oct3 = 0;
}

//------------------------------------------------------------------------------
Network_Name::Network_Name(const uint8_t iei, uint8_t oct3) : _text_string() {
  _iei     = iei;
  length   = 1;
  _u1.oct3 = oct3;
}

//------------------------------------------------------------------------------
Network_Name::Network_Name() : _iei(), _u1(), length{}, _text_string{} {}

//------------------------------------------------------------------------------
Network_Name::~Network_Name() {}

//------------------------------------------------------------------------------
void Network_Name::getValue(bstring& text_string) {
  text_string = _text_string;
}

//------------------------------------------------------------------------------
void Network_Name::getValue(uint8_t& oct3) {
  oct3 = _u1.oct3;
}

//------------------------------------------------------------------------------
int Network_Name::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Network_Name IEI (0x%x)", _iei);
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;

    // for Length: *(buf + encoded_size)
    encoded_size++;
    *(buf + encoded_size) = _u1.oct3;
    encoded_size++;
    int size =
        encode_bstring(_text_string, (buf + encoded_size), len - encoded_size);
    encoded_size += size;
    *(buf + 1) = encoded_size;  // length
  } else {
    encoded_size++;  // Length
    *(buf + encoded_size) = _u1.oct3;
    encoded_size++;
    int size =
        encode_bstring(_text_string, (buf + encoded_size), len - encoded_size);
    encoded_size += size;
    *(buf) = encoded_size;  // length
  }
  Logger::nas_mm().debug("Encoded Network_Name len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Network_Name::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding Network_Name IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {  // IEI
    _iei = *(buf + decoded_size);
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  _u1.oct3 = *(buf + decoded_size);
  decoded_size++;

  decode_bstring(
      &_text_string, length - 1, (buf + decoded_size), len - decoded_size);
  decoded_size += length - 1;
  for (int i = 0; i < blength(_text_string); i++) {
    Logger::nas_mm().debug(
        "Decoded Text String value: 0x%2x", (uint8_t) bdata(_text_string)[i]);
  }

  Logger::nas_mm().debug("Decoded Network_Name len (%d)", decoded_size);
  return decoded_size;
}
