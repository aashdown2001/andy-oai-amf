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

#include "AuthenticationFailure.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationFailure::AuthenticationFailure()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_FAILURE) {
  ie_authentication_failure_parameter = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationFailure::~AuthenticationFailure() {}

//------------------------------------------------------------------------------
void AuthenticationFailure::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationFailure::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause.SetValue(value);
}

//------------------------------------------------------------------------------
uint8_t AuthenticationFailure::get5GMmCause() {
  return ie_5gmm_cause.GetValue();
}

//------------------------------------------------------------------------------
void AuthenticationFailure::setAuthentication_Failure_Parameter(
    const bstring& auts) {
  ie_authentication_failure_parameter =
      std::make_optional<Authentication_Failure_Parameter>(0x30, auts);
}
bool AuthenticationFailure::getAutsInAuthFailPara(bstring& auts) {
  if (ie_authentication_failure_parameter.has_value()) {
    ie_authentication_failure_parameter.value().getValue(auts);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
int AuthenticationFailure::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationFailure message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  int size = ie_5gmm_cause.Encode(buf + encoded_size, len - encoded_size);

  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_5gmm_cause error");
    return KEncodeDecodeError;
  }

  if (!ie_authentication_failure_parameter.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_authentication_failure_parameter is not available");
  } else {
    size = ie_authentication_failure_parameter.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_authentication_failure_parameter error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationFailure message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationFailure::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationFailure message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // 5GMM Cause
  if ((decoded_result = ie_5gmm_cause.Decode(
           buf + decoded_size, len - decoded_size, false)) ==
      KEncodeDecodeError)
    return decoded_result;
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x30: {
        Logger::nas_mm().debug("Decoding IEI (0x30)");
        Authentication_Failure_Parameter
            ie_authentication_failure_parameter_tmp = {};
        if ((decoded_result = ie_authentication_failure_parameter_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_authentication_failure_parameter =
            std::optional<Authentication_Failure_Parameter>(
                ie_authentication_failure_parameter_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationFailure message len (%d)", decoded_size);
  return decoded_size;
}
