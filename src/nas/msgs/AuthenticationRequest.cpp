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

#include "AuthenticationRequest.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationRequest::AuthenticationRequest()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_REQUEST) {
  ie_authentication_parameter_rand = std::nullopt;
  ie_authentication_parameter_autn = std::nullopt;
  ie_eap_message                   = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationRequest::~AuthenticationRequest() {}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI.Set(false);  // 4 lower bits
  ie_ngKSI.SetNasKeyIdentifier(key_set_id);
  ie_ngKSI.SetTypeOfSecurityContext(tsc);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba.Set(length, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setAuthentication_Parameter_RAND(
    uint8_t value[kAuthenticationParameterRandValueLength]) {
  ie_authentication_parameter_rand =
      std::make_optional<Authentication_Parameter_RAND>(
          kIeiAuthenticationParameterRand, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setAuthentication_Parameter_AUTN(
    uint8_t value[kAuthenticationParameterAutnValueLength]) {
  ie_authentication_parameter_autn =
      std::make_optional<Authentication_Parameter_AUTN>(
          kIeiAuthenticationParameterAutn, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetEapMessage(bstring eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
int AuthenticationRequest::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationRequest message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  int size = ie_ngKSI.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_ngKSI error");
    return KEncodeDecodeError;
  }
  // Spare half octet
  encoded_size++;  // 1/2 octet + 1/2 octet from ie_ngKSI

  // ABBA
  size = ie_abba.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_abba error");
    return KEncodeDecodeError;
  }

  // Authentication parameter RAND
  if (!ie_authentication_parameter_rand.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_authentication_parameter_rand is not available");
  } else {
    int size = ie_authentication_parameter_rand.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_authentication_parameter_rand error");
      return KEncodeDecodeError;
    }
  }
  // Authentication parameter AUTN
  if (!ie_authentication_parameter_autn.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_authentication_parameter_autn is not available");
  } else {
    int size = ie_authentication_parameter_autn.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_authentication_parameter_autn error");
      return KEncodeDecodeError;
    }
  }
  // EAP message
  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    int size =
        ie_eap_message.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationRequest message (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationRequest::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationReject message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // NgKSI
  decoded_size += ie_ngKSI.Decode(
      buf + decoded_size, len - decoded_size, false,
      false);      // length 1/2, low position
  decoded_size++;  // 1/2 octet from ie_ngKSI, 1/2 from Spare half octet
  // ABBA
  decoded_size += ie_abba.Decode(buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size %d", decoded_size);

  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI 0x%x", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiAuthenticationParameterRand: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiAuthenticationParameterRand);
        Authentication_Parameter_RAND ie_authentication_parameter_rand_tmp = {};
        if ((decoded_result = ie_authentication_parameter_rand_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_authentication_parameter_rand =
            std::optional<Authentication_Parameter_RAND>(
                ie_authentication_parameter_rand_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiAuthenticationParameterAutn: {
        Authentication_Parameter_AUTN ie_authentication_parameter_autn_tmp = {};

        if ((decoded_result = ie_authentication_parameter_autn_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_authentication_parameter_autn =
            std::optional<Authentication_Parameter_AUTN>(
                ie_authentication_parameter_autn_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
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
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationRequest message (len %d)", decoded_size);
  return 1;
}
