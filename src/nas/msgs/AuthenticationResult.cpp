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
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "AuthenticationResult.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationResult::AuthenticationResult()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_RESULT) {
  ie_abba = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationResult::~AuthenticationResult() {}

//------------------------------------------------------------------------------
void AuthenticationResult::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI.Set(false);  // 4 lower bits
  ie_ngKSI.SetTypeOfSecurityContext(tsc);
  ie_ngKSI.SetNasKeyIdentifier(key_set_id);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba = std::make_optional<ABBA>(0x38, length, value);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetEapMessage(bstring eap) {
  ie_eap_message.SetValue(eap);
}

//------------------------------------------------------------------------------
int AuthenticationResult::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationResult message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // ngKSI
  int size = ie_ngKSI.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_ngKSI error");
    return KEncodeDecodeError;
  }
  // Spare half octet
  encoded_size++;  // 1/2 octet + 1/2 octet from ie_ngKSI

  // EAP message
  size = ie_eap_message.Encode(buf + encoded_size, len - encoded_size);

  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_eap_message error");
    return KEncodeDecodeError;
  }

  if (!ie_abba.has_value()) {
    Logger::nas_mm().warn("IE ie_abba is not available");
  } else {
    size = ie_abba.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_abba error");
      return KEncodeDecodeError;
    }
  }
  Logger::nas_mm().debug(
      "Encoded AuthenticationResult message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationResult::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationResult message");
  int decoded_size   = 0;
  int decoded_result = 0;
  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // NAS key set identifier
  decoded_result =
      ie_ngKSI.Decode(buf + decoded_size, len - decoded_size, false, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // EAP message
  decoded_result =
      ie_eap_message.Decode(buf + decoded_size, len - decoded_size, false);

  if (decoded_result == KEncodeDecodeError == KEncodeDecodeError)
    return decoded_result;
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiAbba: {
        Logger::nas_mm().debug("decoding IEI (0x38)");
        ABBA ie_abba_tmp = {};
        if ((decoded_result = ie_abba_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_abba = std::optional<ABBA>(ie_abba_tmp);
        octet   = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationResult message len (%d)", decoded_size);
  return decoded_size;
}
