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

#include "3gpp_ts24501.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "NasMmPlainHeader.hpp"

using namespace nas;

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetEpd(const uint8_t epd) {
  epd_.Set(epd);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetEpd() {
  return epd_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetSecurityHeaderType(const uint8_t type) {
  secu_header_type_.Set(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetSecurityHeaderType() {
  return secu_header_type_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetMessageType(const uint8_t type) {
  msg_type_.Set(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetMessageType() {
  return msg_type_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::setHeader(
    const uint8_t& epd, const uint8_t& security_header_type,
    const uint8_t& msg_type) {
  epd_.Set(epd);
  secu_header_type_.Set(security_header_type);
  msg_type_.Set(msg_type);
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NasMmPlainHeader");
  if (len < kNasMmPlainHeaderLength) {
    Logger::nas_mm().error("buffer length is less than 3 octets");
    return KEncodeDecodeError;
  } else {
    uint32_t encoded_size = 0;
    uint32_t size         = 0;
    if ((size = epd_.Encode(buf + encoded_size, len - encoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;

    if ((size = secu_header_type_.Encode(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;

    if ((size = msg_type_.Encode(buf + encoded_size, len - encoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;
    Logger::nas_mm().debug(
        "Encoded NasMmPlainHeader (len %d octets)", encoded_size);
    return encoded_size;
  }
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::decodefrombuffer(const uint8_t* const buf, int len) {
  Logger::nas_mm().debug("Decoding NasMmPlainHeader");
  uint32_t decoded_size = 0;
  if (len < kNasMmPlainHeaderLength) {
    Logger::nas_mm().error("Buffer length is less than 3 octets");
    return KEncodeDecodeError;
  } else {
    uint32_t size = 0;
    if ((size = epd_.Decode(buf + decoded_size, len - decoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Decode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    decoded_size += size;

    if ((size = secu_header_type_.Decode(
             buf + decoded_size, len - decoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("Decode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    decoded_size += size;

    if ((size = msg_type_.Decode(buf + decoded_size, len - decoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Decode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    decoded_size += size;
  }
  Logger::nas_mm().debug(
      "decoded NasMmPlainHeader len (%d octets)", decoded_size);
  return decoded_size;
}
