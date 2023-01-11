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
  ie_abba                          = NULL;
  ie_authentication_parameter_rand = NULL;
  ie_authentication_parameter_autn = NULL;
  ie_eap_message                   = NULL;
}

//------------------------------------------------------------------------------
AuthenticationRequest::~AuthenticationRequest() {}

//------------------------------------------------------------------------------
void AuthenticationRequest::setHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI.Set(false);  // 4 lower bits
  ie_ngKSI.setNasKeyIdentifier(key_set_id);
  ie_ngKSI.setTypeOfSecurityContext(tsc);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setABBA(uint8_t length, uint8_t* value) {
  ie_abba = new ABBA(length, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setAuthentication_Parameter_RAND(uint8_t* value) {
  ie_authentication_parameter_rand =
      new Authentication_Parameter_RAND(0x21, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setAuthentication_Parameter_AUTN(uint8_t* value) {
  ie_authentication_parameter_autn =
      new Authentication_Parameter_AUTN(0x20, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::setEAP_Message(bstring eap) {
  ie_eap_message = new EAP_Message(0x78, eap);
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
    return 0;
  }
  // Spare half octet
  encoded_size++;  // 1/2 octet + 1/2 octet from ie_ngKSI

  if (!ie_abba) {
    Logger::nas_mm().warn("IE ie_abba is not available");
  } else {
    int size = ie_abba->Encode(buf + encoded_size, len - encoded_size);
    if (size != 0) {
      Logger::nas_mm().debug(
          "0x%x, 0x%x, 0x%x", (buf + encoded_size)[0], (buf + encoded_size)[1],
          (buf + encoded_size)[2]);
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_abba error");
      return 0;
    }
  }
  if (!ie_authentication_parameter_rand) {
    Logger::nas_mm().warn(
        "IE ie_authentication_parameter_rand is not available");
  } else {
    int size = ie_authentication_parameter_rand->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_authentication_parameter_rand error");
      return 0;
    }
  }
  if (!ie_authentication_parameter_autn) {
    Logger::nas_mm().warn(
        "IE ie_authentication_parameter_autn is not available");
  } else {
    int size = ie_authentication_parameter_autn->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_authentication_parameter_autn error");
      return 0;
    }
  }
  if (!ie_eap_message) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    int size = ie_eap_message->Encode(buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationRequest message (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationRequest::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationReject message");
  int decoded_size = 0;
  decoded_size     = NasMmPlainHeader::Decode(buf, len);

  decoded_size += ie_ngKSI.Decode(
      buf + decoded_size, len - decoded_size, false,
      false);      // length 1/2, low position
  decoded_size++;  // 1/2 octet from ie_ngKSI, 1/2 from Spare half octet
  ie_abba = new ABBA();
  decoded_size +=
      ie_abba->Decode(buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size %d", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI 0x%x", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x21: {
        Logger::nas_mm().debug("Decoding IEI(0x21)");
        ie_authentication_parameter_rand = new Authentication_Parameter_RAND();
        decoded_size += ie_authentication_parameter_rand->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x20: {
        ie_authentication_parameter_autn = new Authentication_Parameter_AUTN();
        decoded_size += ie_authentication_parameter_autn->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding IEI 0x78");
        ie_eap_message = new EAP_Message();
        decoded_size += ie_eap_message->Decode(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationRequest message (len %d)", decoded_size);
  return 1;
}
