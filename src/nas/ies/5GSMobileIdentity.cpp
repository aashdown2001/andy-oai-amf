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

#include "5GSMobileIdentity.hpp"

#include <math.h>

#include "3gpp_24.501.hpp"
#include "conversions.hpp"
#include "String2Value.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSMobileIdentity::_5GSMobileIdentity() {
  iei              = 0;
  _5g_guti         = std::nullopt;
  _imei            = std::nullopt;
  supi_format_imsi = std::nullopt;
  _5g_s_tmsi       = std::nullopt;
  _IMEISV          = std::nullopt;
  length           = 0;
  typeOfIdentity   = 0;
}

//------------------------------------------------------------------------------
_5GSMobileIdentity::_5GSMobileIdentity(uint8_t _iei) : iei(_iei) {
  _5g_guti         = std::nullopt;
  _imei            = std::nullopt;
  supi_format_imsi = std::nullopt;
  _5g_s_tmsi       = std::nullopt;
  _IMEISV          = std::nullopt;
  length           = 0;
  typeOfIdentity   = 0;
}
//------------------------------------------------------------------------------
_5GSMobileIdentity::~_5GSMobileIdentity() {}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::setIEI(uint8_t _iei) {
  iei = _iei;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::_5g_s_tmsi_encode2buffer(uint8_t* buf, int len) {
  if (!_5g_s_tmsi.has_value()) return KEncodeDecodeError;

  int encoded_size = 0;
  if (iei) {
    ENCODE_U8(buf + encoded_size, iei, encoded_size);
  }

  // LENGTH
  ENCODE_U8(buf + encoded_size, 0x00, encoded_size);
  ENCODE_U8(buf + encoded_size, 0x07, encoded_size);

  // Type of identity
  ENCODE_U8(buf + encoded_size, 0xf0 | _5G_S_TMSI, encoded_size);

  // AMF Set ID and AMF Pointer
  ENCODE_U8(
      buf + encoded_size, ((_5g_s_tmsi.value().amf_set_id) & 0x03fc) >> 8,
      encoded_size);
  ENCODE_U8(
      buf + encoded_size,
      (((_5g_s_tmsi.value().amf_set_id) & 0x0003) << 6) |
          ((_5g_s_tmsi.value().amf_pointer) & 0x3f),
      encoded_size);

  // 5G-TMSI
  int tmsi = fromString<int>(_5g_s_tmsi.value()._5g_tmsi);
  ENCODE_U32(buf + encoded_size, tmsi, encoded_size);

  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::_5g_s_tmsi_decodefrombuffer(uint8_t* buf, int len) {
  int decoded_size            = 0;
  _5G_S_TMSI_t _5g_s_tmsi_tmp = {};

  decoded_size++;  // type of identity

  // AMF Set ID and AMF Pointer
  uint8_t octet = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _5g_s_tmsi_tmp.amf_set_id = 0x0000 | ((uint16_t) octet) << 2;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _5g_s_tmsi_tmp.amf_set_id |= (octet & 0xc0) >> 6;
  _5g_s_tmsi_tmp.amf_pointer = octet & 0x3f;

  // 5G TMSI
  uint32_t tmsi = 0;
  DECODE_U32(buf + decoded_size, tmsi, decoded_size);
  _5g_s_tmsi_tmp._5g_tmsi = conv::tmsi_to_string(tmsi);
  _5g_s_tmsi              = std::optional<_5G_S_TMSI_t>(_5g_s_tmsi_tmp);

  return decoded_size;
}

//------------------------------------------------------------------------------
bool _5GSMobileIdentity::get5G_S_TMSI(
    uint16_t& amfSetId, uint8_t& amfPointer, std::string& tmsi) {
  if (!_5g_s_tmsi.has_value()) return false;

  amfSetId   = _5g_s_tmsi.value().amf_set_id;
  amfPointer = _5g_s_tmsi.value().amf_pointer;
  tmsi       = _5g_s_tmsi.value()._5g_tmsi;
  return true;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::set5G_S_TMSI(
    const uint16_t amfSetId, const uint8_t amfPointer, const std::string tmsi) {
  _5G_S_TMSI_t _5g_s_tmsi_tmp = {};
  typeOfIdentity              = _5G_S_TMSI;
  _5g_s_tmsi_tmp.amf_set_id   = amfSetId;
  _5g_s_tmsi_tmp.amf_pointer  = amfPointer;
  _5g_s_tmsi_tmp._5g_tmsi     = tmsi;

  _5g_s_tmsi = std::optional<_5G_S_TMSI_t>(_5g_s_tmsi_tmp);

  // TODO: length

  // Clear the other types
  _5g_guti         = std::nullopt;
  _imei            = std::nullopt;
  supi_format_imsi = std::nullopt;
  _IMEISV          = std::nullopt;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::set5GGUTI(
    const std::string& mcc, const std::string& mnc,
    const uint8_t& amf_region_id, const uint16_t& amf_set_id,
    const uint8_t& amf_pointer, const uint32_t& _5g_tmsi) {
  typeOfIdentity    = _5G_GUTI;
  _5G_GUTI_t tmp    = {};
  tmp.mcc           = mcc;
  tmp.mnc           = mnc;
  tmp.amf_region_id = amf_region_id;
  tmp.amf_set_id    = amf_set_id;
  tmp.amf_pointer   = amf_pointer;
  tmp._5g_tmsi      = _5g_tmsi;
  _5g_guti          = std::optional<_5G_GUTI_t>(tmp);
  length            = 14;  // TODO: define const

  // Clear the other types
  _5g_s_tmsi       = std::nullopt;
  _imei            = std::nullopt;
  supi_format_imsi = std::nullopt;
  _IMEISV          = std::nullopt;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::get5GGUTI(std::optional<_5G_GUTI_t>& guti) const {
  guti = _5g_guti;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::setSuciWithSupiImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routingInd, uint8_t protection_sch_id,
    const std::string& msin) {
  SUCI_imsi_t supi_format_imsi_tmp = {};

  iei                              = 0;
  typeOfIdentity                   = SUCI;
  supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;
  supi_format_imsi_tmp.mcc         = mcc;
  supi_format_imsi_tmp.mnc         = mnc;
  supi_format_imsi_tmp.routingIndicator =
      std::optional<std::string>(routingInd);
  supi_format_imsi_tmp.protectionSchemeId = protection_sch_id;
  supi_format_imsi_tmp.homeNetworkPKI     = HOME_NETWORK_PKI_0_WHEN_PSI_0;
  supi_format_imsi_tmp.msin               = msin;
  length                                  = 10 + ceil(msin.length() / 2);

  supi_format_imsi = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);

  // Clear the other types
  _5g_s_tmsi = std::nullopt;
  _5g_guti   = std::nullopt;
  _imei      = std::nullopt;
  _IMEISV    = std::nullopt;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::setSuciWithSupiImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routingInd, uint8_t protecSchId, uint8_t home_pki,
    const std::string& msin_digits) {
  SUCI_imsi_t supi_format_imsi_tmp = {};

  supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;
  supi_format_imsi_tmp.mcc         = mcc;
  supi_format_imsi_tmp.mnc         = mnc;
  supi_format_imsi = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);

  // Clear the other types
  _5g_s_tmsi = std::nullopt;
  _5g_guti   = std::nullopt;
  _imei      = std::nullopt;
  _IMEISV    = std::nullopt;
}

//------------------------------------------------------------------------------
bool _5GSMobileIdentity::getSuciWithSupiImsi(SUCI_imsi_t& ptr) {
  if (!supi_format_imsi.has_value()) return false;
  ptr = supi_format_imsi.value();
  return true;
}

//------------------------------------------------------------------------------
void _5GSMobileIdentity::setIMEISV(const IMEISV_t& imeisv) {
  typeOfIdentity      = IMEISV;
  length              = blength(imeisv.identity) - 1 + 4;
  IMEISV_t IMEISV_tmp = {};
  IMEISV_tmp.identity = bstrcpy(imeisv.identity);
  IMEISV_tmp.identity->data[blength(imeisv.identity) - 1] |= 0xf0;
  _IMEISV = std::optional<IMEISV_t>(IMEISV_tmp);

  // Clear the other types
  _5g_s_tmsi       = std::nullopt;
  _5g_guti         = std::nullopt;
  _imei            = std::nullopt;
  supi_format_imsi = std::nullopt;
}

//------------------------------------------------------------------------------
bool _5GSMobileIdentity::getIMEISV(IMEISV_t& imeisv) {
  if (!_IMEISV.has_value()) return false;
  imeisv.identity = bstrcpy(_IMEISV.value().identity);
  return true;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::encode2Buffer(uint8_t* buf, int len) {
  switch (typeOfIdentity) {
    case SUCI: {
      return suci_encode2buffer(buf, len);
    } break;
    case _5G_GUTI: {
      return _5g_guti_encode2buffer(buf, len);
    } break;
    case IMEISV: {
      return imeisv_encode2buffer(buf, len);
    } break;
    case _5G_S_TMSI: {
      return _5g_s_tmsi_encode2buffer(buf, len);
    } break;
    default: {
      Logger::nas_mm().debug("Unknown Type of Identity  0x%x", typeOfIdentity);
    }
  }
  return KEncodeDecodeError;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::suci_encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SUCI IEI 0x%x", iei);

  if (!supi_format_imsi.has_value()) return KEncodeDecodeError;
  if (len < length) {
    Logger::nas_mm().debug("error: len is less than %d", length);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  if (iei) {
    Logger::nas_mm().debug(
        "Decoding 5GSMobilityIdentity IEI 0x%x", typeOfIdentity);
    ENCODE_U8(buf + encoded_size, iei, encoded_size);
  }

  encoded_size += 2;  // For encode Lengh later on

  // SUPI format + Type of Identity
  ENCODE_U8(
      buf + encoded_size, (0x70 & (SUPI_FORMAT_IMSI << 4)) | (0x07 & SUCI),
      encoded_size);
  // MCC/MNC
  encoded_size += encodeMccMnc2buffer(
      supi_format_imsi.value().mcc, supi_format_imsi.value().mnc,
      buf + encoded_size);

  // Routing Indicator
  encoded_size += encodeRoutid2buffer(
      supi_format_imsi.value().routingIndicator, buf + encoded_size);
  // Protection Scheme
  ENCODE_U8(
      buf + encoded_size, 0x0f & supi_format_imsi.value().protectionSchemeId,
      encoded_size);
  // Home network public key identifier
  ENCODE_U8(
      buf + encoded_size, supi_format_imsi.value().homeNetworkPKI,
      encoded_size);

  // Encode Length
  int encoded_len_ie = 0;
  if (!iei) {
    ENCODE_U16(buf, encoded_size - 2, encoded_len_ie);
  } else {
    ENCODE_U16(buf + 1, encoded_size - 3, encoded_len_ie);
  }
  Logger::nas_mm().debug("Encoded SUCI IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::_5g_guti_encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding 5G-GUTI IEI 0x%x", iei);
  if (len < length)
    Logger::nas_mm().debug("Error: len is less than %d", length);
  int encoded_size = 0;
  if (iei) {
    Logger::nas_mm().debug(
        "Encoding 5GSMobilityIdentity type 0x%x", typeOfIdentity);
    ENCODE_U8(buf + encoded_size, iei, encoded_size);
  }
  encoded_size += 2;  // store length, do it later

  ENCODE_U8(
      buf + encoded_size, 0xf0 | _5G_GUTI, encoded_size);  // Type of Identity
  encoded_size += encodeMccMnc2buffer(
      _5g_guti.value().mcc, _5g_guti.value().mnc, buf + encoded_size);
  ENCODE_U8(buf + encoded_size, _5g_guti.value().amf_region_id, encoded_size);
  ENCODE_U8(
      buf + encoded_size, ((_5g_guti.value().amf_set_id & 0x03fc) >> 2),
      encoded_size);
  ENCODE_U8(
      buf + encoded_size,
      ((_5g_guti.value().amf_pointer & 0x3f) |
       ((_5g_guti.value().amf_set_id & 0x0003) << 6)),
      encoded_size);

  // TMSI: 4 octets
  uint32_t tmsi = _5g_guti.value()._5g_tmsi;
  ENCODE_U32(buf + encoded_size, tmsi, encoded_size);

  // Encode Len
  uint8_t ie_len           = 0;
  uint8_t encoded_size_tmp = 0;
  if (!iei) {
    ie_len           = encoded_size - 2;
    encoded_size_tmp = 0;
  } else {
    ie_len           = encoded_size - 3;
    encoded_size_tmp = 1;
  }
  ENCODE_U16(buf + encoded_size_tmp, ie_len, encoded_size_tmp);

  Logger::nas_mm().debug("Encoded 5G-GUTI IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::encodeMccMnc2buffer(
    const std::string& mcc_str, const std::string& mnc_str, uint8_t* buf) {
  int encoded_size = 0;
  uint8_t value    = 0;
  int mcc          = fromString<int>(mcc_str);
  int mnc          = fromString<int>(mnc_str);

  value = (0x0f & (mcc / 100)) | ((0x0f & ((mcc % 100) / 10)) << 4);
  ENCODE_U8(buf + encoded_size, value, encoded_size);

  Logger::nas_mm().debug("MNC digit 1: %d", mnc / 100);
  if (!(mnc / 100)) {
    Logger::nas_mm().debug("Encoding MNC 2 digits");
    value = (0x0f & (mcc % 10)) | 0xf0;
    Logger::nas_mm().debug("Buffer 0x%x", value);
    ENCODE_U8(buf + encoded_size, value, encoded_size);

    value = (0x0f & ((mnc % 100) / 10)) | ((0x0f & (mnc % 10)) << 4);
    ENCODE_U8(buf + encoded_size, value, encoded_size);

  } else {
    Logger::nas_mm().debug("Encoding MNC 3 digits");

    value = (0x0f & (mcc % 10)) | ((0x0f & (mnc % 10)) << 4);
    Logger::nas_mm().debug("Buffer 0x%x", value);
    ENCODE_U8(buf + encoded_size, value, encoded_size);

    value = ((0x0f & ((mnc % 100) / 10)) << 4) | (0x0f & (mnc / 100));
    Logger::nas_mm().debug("Buffer 0x%x", value);
    ENCODE_U8(buf + encoded_size, value, encoded_size);
  }
  Logger::nas_mm().debug("MCC %s, MNC %s", mcc_str.c_str(), mnc_str.c_str());
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::encodeRoutid2buffer(
    std::optional<std::string> routing_indicator, uint8_t* buf) {
  int encoded_size = 0;
  if (!routing_indicator.has_value()) {
    Logger::nas_mm().debug("No Routing Indicator is configured, encoding");
    ENCODE_U8(buf + encoded_size, 0xf0, encoded_size);
    ENCODE_U8(buf + encoded_size, 0xff, encoded_size);
    return encoded_size;
  }

  Logger::nas_mm().debug(
      "Routing Indicator (%s)", routing_indicator.value().c_str());
  int rooutid = fromString<int>(routing_indicator.value());
  switch (routing_indicator.value().length()) {
    case 1: {
      *buf = 0xf0 | (0x0f & rooutid);
      encoded_size++;
      *(buf + encoded_size) = 0xff;
      encoded_size++;
    } break;
    case 2: {
      *buf = (0x0f & (rooutid / 10)) | ((0x0f & (rooutid % 10)) << 4);
      encoded_size++;
      *(buf + encoded_size) = 0xff;
      encoded_size++;
    } break;
    case 3: {
      *buf = (0x0f & (rooutid / 100)) | ((0x0f & ((rooutid % 100) / 10)) << 4);
      encoded_size++;
      *(buf + encoded_size) = 0xf0 | (0x0f & (rooutid % 10));
      encoded_size++;
    } break;
    case 4: {
      *buf =
          (0x0f & (rooutid / 1000)) | ((0x0f & ((rooutid % 1000) / 100)) << 4);
      encoded_size++;
      *(buf + encoded_size) =
          (0x0f & ((rooutid % 100) / 10)) | ((0x0f & (rooutid % 10)) << 4);
      encoded_size++;
    } break;
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::encodeMSIN2buffer(std::string msinstr, uint8_t* buf) {
  return 1;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::imeisv_encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding IMEISV IE IEI 0x%x", iei);
  if (!_IMEISV.has_value()) return KEncodeDecodeError;
  if (len < length)
    Logger::nas_mm().debug("Error: len is less than %d", length);
  int encoded_size = 0;
  if (iei) {
    Logger::nas_mm().debug(
        "Encoding 5GSMobilityIdentity IEI 0x%x", typeOfIdentity);
    ENCODE_U8(buf + encoded_size, iei, encoded_size);
  }

  encoded_size += 2;  // Skip len for now

  int size = encode_bstring(
      _IMEISV.value().identity, (buf + encoded_size), len - encoded_size);
  encoded_size += size;
  // Update Type of identity (3 bits of Octet 4)
  *(buf + 3) |= (0x01 << 3) | IMEISV;
  // TODO: odd/even indic

  // Encode length
  int encoded_len = 0;
  if (!iei) {
    ENCODE_U16(buf, encoded_size - 2, encoded_len);
  } else {
    ENCODE_U16(buf + 1, encoded_size - 3, encoded_len);
  }

  Logger::nas_mm().debug("Encoded IMEISV IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::decodeFromBuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity");
  int decoded_size = 0;
  if (is_option) {
    DECODE_U8(buf + decoded_size, iei, decoded_size);
  }

  DECODE_U16(buf + decoded_size, length, decoded_size);

  Logger::amf_n1().debug("Decoded 5GSMobilityIdentity IE length %d", length);
  int decoded_size_tmp = 0;
  uint8_t octet        = 0;
  DECODE_U8(
      buf + decoded_size, octet,
      decoded_size_tmp);  // Decode but don't increase decoded_size

  switch (octet & 0x07) {
    case SUCI: {
      typeOfIdentity = SUCI;
      decoded_size +=
          suci_decodefrombuffer(buf + decoded_size, len - decoded_size, length);
      Logger::nas_mm().debug("Decoded SUCI (%d octets)", decoded_size);
      return decoded_size;
    } break;
    case _5G_GUTI: {
      typeOfIdentity = _5G_GUTI;
      decoded_size +=
          _5g_guti_decodefrombuffer(buf + decoded_size, len - decoded_size);
      return decoded_size;
    } break;
    // TODO: IMEI
    case _5G_S_TMSI: {
      typeOfIdentity = _5G_S_TMSI;
      decoded_size +=
          _5g_s_tmsi_decodefrombuffer(buf + decoded_size, len - decoded_size);
      return decoded_size;
    } break;
    case IMEISV: {
      typeOfIdentity = IMEISV;
      decoded_size += imeisv_decodefrombuffer(buf + decoded_size, length);
      return decoded_size;
    } break;
    // TODO: MAC Address
    default: {
      // TODO:
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::suci_decodefrombuffer(
    uint8_t* buf, int len, int ie_len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity SUCI");
  int decoded_size = 0;
  uint8_t octet    = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  switch ((octet & 0x70) >> 4) {
    case SUPI_FORMAT_IMSI: {
      SUCI_imsi_t supi_format_imsi_tmp = {};
      supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;

      DECODE_U8(buf + decoded_size, octet, decoded_size);
      int mcc = 0, mnc = 0;
      mcc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);

      DECODE_U8(buf + decoded_size, octet, decoded_size);
      mcc += (octet & 0x0f);
      if ((octet & 0xf0) == 0xf0) {
        DECODE_U8(buf + decoded_size, octet, decoded_size);
        mnc += ((octet & 0x0f) * 10 + ((octet & 0xf0) >> 4));
      } else {
        mnc += ((octet & 0xf0) >> 4);
        DECODE_U8(buf + decoded_size, octet, decoded_size);
        mnc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);
      }
      std::string mnc_str = std::to_string(mnc);
      if (mnc < 10) {
        mnc_str = "0" + mnc_str;
      }
      std::string mcc_str = std::to_string(mcc);
      if (mcc < 10) {
        mcc_str = "00" + mcc_str;
      } else if (mcc < 100) {
        mcc_str = "0" + mcc_str;
      }

      Logger::nas_mm().debug(
          "MCC %s, MNC %s", mcc_str.c_str(), mnc_str.c_str());
      supi_format_imsi_tmp.mcc = (const std::string)(mcc_str);
      supi_format_imsi_tmp.mnc = (const std::string)(mnc_str);

      // Routing Indicator
      uint8_t digit[4];
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      digit[0] = octet & 0x0f;
      digit[1] = (octet & 0xf0) >> 4;

      DECODE_U8(buf + decoded_size, octet, decoded_size);
      digit[2] = octet & 0x0f;
      digit[3] = (octet & 0xf0) >> 4;
      if (!digit[0] && digit[1] == 0x0f && digit[2] == 0x0f &&
          digit[3] == 0x0f) {
        supi_format_imsi_tmp.routingIndicator =
            std::nullopt;  // No Routing Indicator is configured
      } else {
        std::string result = {};
        for (int i = 0; i < 4; i++) {
          if (digit[i] >= 0x00 && digit[i] <= 0x09)
            result += (const std::string)(std::to_string(digit[i]));
          else if (digit[i] == 0x0f)
            break;
          else
            Logger::nas_mm().error(
                "Decoded Routing Indicator is not BCD coding");
        }
        supi_format_imsi_tmp.routingIndicator =
            std::optional<std::string>(result);
        Logger::nas_mm().debug(
            "Decoded Routing Indicator %s",
            supi_format_imsi_tmp.routingIndicator.value().c_str());
      }
      // Protection scheme Id
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      supi_format_imsi_tmp.protectionSchemeId = 0x0f & octet;
      // Home network public key identifier
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      supi_format_imsi_tmp.homeNetworkPKI = octet;

      // MSIN
      std::string msin = {};
      // TODO: get MSIN according to Protection Scheme ID to support
      // ECIES scheme profile A/B
      int digit_low = 0, digit_high = 0, numMsin = ie_len - decoded_size;
      for (int i = 0; i < numMsin; i++) {
        DECODE_U8(buf + decoded_size, octet, decoded_size);
        digit_high = (octet & 0xf0) >> 4;
        digit_low  = octet & 0x0f;
        msin +=
            ((const std::string)(std::to_string(digit_low)) +
             (const std::string)(std::to_string(digit_high)));
      }
      supi_format_imsi_tmp.msin = msin;
      Logger::nas_mm().debug("Decoded MSIN %s", supi_format_imsi->msin.c_str());

      supi_format_imsi = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);
      Logger::nas_mm().debug(
          "Decoded 5GSMobilityIdentity SUCI SUPI format IMSI (len %d)",
          decoded_size);
      return decoded_size;
    } break;
    case SUPI_FORMAT_NETWORK_SPECIFIC_IDENTIFIER: {
      // TODO:
    } break;
    default: {
      // TODO:
    } break;
  }
  return 0;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::_5g_guti_decodefrombuffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity 5G-GUTI");
  // Starting from Octet 3 (Type of Identity)
  int decoded_size = 0;
  uint8_t octet    = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);  // Type of Identity

  _5G_GUTI_t tmp = {};
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  int mcc = 0, mnc = 0;
  mcc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  mcc += (octet & 0x0f);
  Logger::nas_mm().debug("MCC %s", std::to_string(mcc).c_str());
  Logger::nas_mm().debug("Buffer 0x%x", octet);
  if ((octet & 0xf0) == 0xf0) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    mnc += ((octet & 0x0f) * 10 + ((octet & 0xf0) >> 4));
    Logger::nas_mm().debug("MNC (2 digits): %s", std::to_string(mnc).c_str());
  } else {
    mnc += ((octet & 0xf0) >> 4);
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    Logger::nas_mm().debug("Buffer 0x%x", octet);

    mnc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);
    Logger::nas_mm().debug("MNC (3 digits): %s", std::to_string(mnc).c_str());
  }

  std::string mnc_str = std::to_string(mnc);
  if (mnc < 10) {
    mnc_str = "0" + mnc_str;
  }

  Logger::nas_mm().debug(
      "MCC %s, MNC %s", std::to_string(mcc).c_str(), mnc_str.c_str());
  tmp.mcc = (const std::string)(std::to_string(mcc));
  tmp.mnc = (const std::string)(mnc_str);

  DECODE_U8(buf + decoded_size, tmp.amf_region_id, decoded_size);
  DECODE_U8(buf + decoded_size, tmp.amf_set_id, decoded_size);
  DECODE_U8(buf + decoded_size, tmp.amf_pointer, decoded_size);

  // TMSI, 4 octets
  DECODE_U32(buf + decoded_size, tmp._5g_tmsi, decoded_size);

  Logger::nas_mm().debug("TMSI 0x%x", tmp._5g_tmsi);
  _5g_guti = std::optional<_5G_GUTI_t>(tmp);
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity 5G-GUTI");
  return decoded_size;
}

//------------------------------------------------------------------------------
int _5GSMobileIdentity::imeisv_decodefrombuffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity IMEISV");
  int decoded_size    = 0;
  IMEISV_t IMEISV_tmp = {};
  decode_bstring(
      &(IMEISV_tmp.identity), len, (buf + decoded_size), len - decoded_size);
  decoded_size += len;
  for (int i = 0; i < len; i++) {
    Logger::nas_mm().debug(
        "Decoded 5GSMobilityIdentity IMEISV value(0x%x)",
        (uint8_t) IMEISV_tmp.identity->data[i]);
  }
  _IMEISV = std::optional<IMEISV_t>(IMEISV_tmp);
  Logger::nas_mm().debug(
      "decoded 5GSMobilityIdentity IMEISV len (%d)", decoded_size);
  return decoded_size;
}
