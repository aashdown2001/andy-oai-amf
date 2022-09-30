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

#ifndef _5GS_MOBILE_IDENTITY_H_
#define _5GS_MOBILE_IDENTITY_H_

#include <stdint.h>
#include <optional>

#include "struct.hpp"
extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}
using namespace std;

namespace nas {

// 5G-GUTI
typedef struct _5G_GUTI_s {
  std::string mcc;
  std::string mnc;
  uint8_t amf_region_id;
  uint8_t amf_set_id;
  uint16_t amf_pointer;
  uint32_t _5g_tmsi;
} _5G_GUTI_t;

// IMEI or IMEISV
typedef struct IMEI_or_IMEISV_s {
  uint8_t typeOfIdentity : 3;
  bool odd_even_indic;  // for imei, even means bits 5 to 8 of last octet is
                        // "1111", for imeisv, bits 5 to 8 of last octet is
                        // "1111"
  string identity;      // "46011000001"
} IMEI_IMEISV_t;

// SUCI and SUPI format IMSI
typedef struct SUCI_imsi_s {
  uint8_t supi_format : 3;
  string mcc;
  string mnc;
  string routingIndicator;         //"1234"
  uint8_t protectionSchemeId : 4;  // 0000
  uint8_t homeNetworkPKI;          // 00000000
  string msin;                     // two types of coding; BCD & hexadecimal
} SUCI_imsi_t;                     // SUPI format "IMSI"

// TODO: SUCI and SUPI format "Network specific identifier"
// 5G-S-TMSI
typedef struct _5G_S_TMSI_s {
  uint16_t amf_set_id;
  uint8_t amf_pointer;
  string _5g_tmsi;
} _5G_S_TMSI_t;

// TODO: 5GS mobile identity information element for type of identity "MAC
// address"

class _5GSMobileIdentity {
 public:
  _5GSMobileIdentity(
      uint8_t _iei, const uint16_t amfSetId, const uint8_t amfPointer,
      const string tmsi);  // 5G-S-TMSI

  _5GSMobileIdentity();
  ~_5GSMobileIdentity();

  // Common
  uint8_t getTypeOfIdentity() const { return typeOfIdentity; };

  void setIEI(uint8_t _iei);

  int encode2Buffer(uint8_t* buf, int len);
  int decodeFromBuffer(uint8_t* buf, int len, bool is_option);

  int encodeMccMnc2buffer(
      const std::string& mcc_str, const std::string& mnc_str, uint8_t* buf);

  // 5G GUTI
  int _5g_guti_decodefrombuffer(uint8_t* buf, int len);
  int _5g_guti_encode2buffer(uint8_t* buf, int len);
  void set5GGUTI(
      const string& mcc, const string& mnc, const uint8_t& amf_region_id,
      const uint16_t& amf_set_id, const uint8_t& amf_pointer,
      const uint32_t& _5g_tmsi);
  void get5GGUTI(std::optional<_5G_GUTI_t>&) const;

  // SUCI
  int suci_decodefrombuffer(uint8_t* buf, int len, int length);
  int suci_encode2buffer(uint8_t* buf, int len);

  void setSuciWithSupiImsi(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id,
      const string msin);  // TODO: SetSUCI, SUCI and SUPI format IMSI

  void setSuciWithSupiImsi(
      const string& mcc, const string& mnc, const string& routingInd,
      uint8_t protecSchId, uint8_t home_pki, const string& msin_digits);
  /*
  void setSuciWithSupiImsi(
      const string& mcc, const string& mnc, const string& routingInd,
      uint8_t protecSchId, const string& msin_digits);
      */
  bool getSuciWithSupiImsi(SUCI_imsi_t&);

  int encodeRoutid2buffer(string routid, uint8_t* buf);
  int encodeMSIN2buffer(string msinstr, uint8_t* buf);

  // TMSI
  int _5g_s_tmsi_encode2buffer(uint8_t* buf, int len);
  int _5g_s_tmsi_decodefrombuffer(uint8_t* buf, int len);
  bool get5G_S_TMSI(uint16_t& amfSetId, uint8_t& amfPointer, string& tmsi);

  // IMEISV
  int imeisv_encode2buffer(uint8_t* buf, int len);
  int imeisv_decodefrombuffer(uint8_t* buf, int len);

  void setIMEISV(const IMEISV_t& imeisv);
  bool getIMEISV(IMEISV_t& imeisv);

 private:
  uint8_t iei;
  uint16_t length;
  uint8_t typeOfIdentity : 3;

  std::optional<SUCI_imsi_t> supi_format_imsi;
  std::optional<_5G_GUTI_t> _5g_guti;
  std::optional<IMEI_IMEISV_t> _imei;  // TODO:
  std::optional<IMEISV_t> _IMEISV;

  std::optional<_5G_S_TMSI_t> _5g_s_tmsi;
};

}  // namespace nas

#endif
