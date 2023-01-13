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

#include "AuthenticationReject.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationReject::AuthenticationReject()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_REJECT) {
  ie_eap_message = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationReject::~AuthenticationReject() {}

//------------------------------------------------------------------------------
void AuthenticationReject::setHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationReject::SetEapMessage(const bstring& eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
int AuthenticationReject::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationReject message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    if (int size = ie_eap_message.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded AuthenticationReject message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int AuthenticationReject::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationReject message");

  int decoded_size   = 0;
  int decoded_result = 0;
  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // IEIs
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        EapMessage ie_eap_message_tmp = {};
        if ((decoded_result = ie_eap_message_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationReject message len (%d)", decoded_size);
  return decoded_size;
}
