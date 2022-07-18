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

#ifndef _STRUCT_H_
#define _STRUCT_H_
#include <stdint.h>

#include <iostream>
#include <tuple>
#include <string>
#include <vector>

#include "logger.hpp"
#include "spdlog/fmt/fmt.h"

#include "3gpp_23.003.h"

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}
using namespace std;
namespace nas {

typedef struct SNSSAI_s {
 private:
  uint8_t m_sst;
  std::pair<bool, int32_t> m_sd;
  std::pair<bool, int8_t> m_mHplmnSst;
  std::pair<bool, int32_t> m_mHplmnSd;
  uint8_t m_length;

 public:
  static constexpr uint8_t kSNSSAIMinSize      = 2;
  static constexpr uint8_t kSNSSAIValueMinSize = 1;
  SNSSAI_s()
      : m_sst(),
        m_sd(false, SD_NO_VALUE),
        m_mHplmnSst(),
        m_mHplmnSd(false, SD_NO_VALUE),
        m_length(kSNSSAIValueMinSize) {}
  SNSSAI_s(const uint8_t& sst)
      : m_sst(sst),
        m_sd(false, SD_NO_VALUE),
        m_mHplmnSst(),
        m_mHplmnSd(false, SD_NO_VALUE),
        m_length(kSNSSAIValueMinSize) {}
  SNSSAI_s(const uint8_t& sst, const uint8_t& mhplmnsst)
      : m_sst(sst),
        m_sd(false, SD_NO_VALUE),
        m_mHplmnSst(true, mhplmnsst),
        m_mHplmnSd(false, SD_NO_VALUE),
        m_length(2) {}
  SNSSAI_s(const uint8_t& sst, const int32_t& sd)
      : m_sst(sst),
        m_sd(true, sd),
        m_mHplmnSst(),
        m_mHplmnSd(false, SD_NO_VALUE),
        m_length(4) {}
  SNSSAI_s(const uint8_t& sst, const int32_t& sd, const uint8_t& mhplmnsst)
      : m_sst(sst),
        m_sd(true, sd),
        m_mHplmnSst(true, mhplmnsst),
        m_mHplmnSd(false, SD_NO_VALUE),
        m_length(5) {}
  SNSSAI_s(
      const uint8_t& sst, const int32_t& sd, const uint8_t& mhplmnsst,
      const int32_t& mhplmnsd)
      : m_sst(sst),
        m_sd(true, sd),
        m_mHplmnSst(true, mhplmnsst),
        m_mHplmnSd(true, mhplmnsd),
        m_length(8) {}

  SNSSAI_s& operator=(const struct SNSSAI_s& s) {
    m_sst       = s.m_sst;
    m_sd        = s.m_sd;
    m_mHplmnSst = s.m_mHplmnSst;
    m_mHplmnSd  = s.m_mHplmnSd;
    m_length    = s.m_length;
    return *this;
  }

  void SetSst(const uint8_t& v) { m_sst = v; }

  void SetSd(const int32_t& v) {
    if (!m_sd.first) m_length += 3;
    m_sd.first  = true;
    m_sd.second = v;
  }

  void SetMhplmnSst(const uint8_t& v) {
    if (!m_mHplmnSst.first) m_length += 1;
    m_mHplmnSst.first  = true;
    m_mHplmnSst.second = v;
  }

  void SetMhplmnSd(const int32_t& v) {
    if (!m_mHplmnSd.first) m_length += 3;
    m_mHplmnSd.first  = true;
    m_mHplmnSd.second = v;
  }

  // length of IE content, meaning no IE, length included
  // return  m_length+1, +1 for length encoding
  uint8_t Length() const { return m_length + 1; }

  uint8_t Sst() const { return m_sst; }

  int32_t Sd() const {
    if (m_sd.first) {
      return m_sd.second;
    }
    return SD_NO_VALUE;
  }

  bool Sd(int32_t& v) const {
    if (m_sd.first) {
      v = m_sd.second;
      return true;
    }
    return false;
  }

  bool MhplmnSst(uint8_t& v) const {
    if (m_mHplmnSst.first) {
      v = m_mHplmnSst.second;
      return true;
    }
    return false;
  }

  bool MhplmnSd(int32_t& v) const {
    if (m_mHplmnSd.first) {
      v = m_mHplmnSd.second;
      return true;
    }
    return false;
  }

  std::string ToString() {
    std::string s;
    s.append(fmt::format("SST {:#x}", m_sst));
    int32_t sdv = 0;
    if (Sd(sdv)) {
      s.append(fmt::format(" SD {:#x}", sdv));
    }
    uint8_t mhplmn_sstv = 0;
    if (MhplmnSst(mhplmn_sstv)) {
      s.append(fmt::format(" M-HPLMN SST {:#x}", mhplmn_sstv));
    }
    int32_t mhplmn_sdv = 0;
    if (MhplmnSd(mhplmn_sdv)) {
      s.append(fmt::format(" M-HPLMN SD {:#x}", mhplmn_sdv));
    }
    return s;
  }

  int Encode2Buffer(uint8_t* buf, int len, bool is_option = true) const {
    Logger::nas_mm().debug("Encoding S_NSSAI V");
    int encoded_size = 0;
    if (is_option) {
      if (len < (m_length + 2)) {
        Logger::nas_mm().error(
            "Out of buffer space for encoding len %d is less than %u", len,
            m_length + 2);
        return 0;
      }
    } else if (len < (m_length + 1)) {
      Logger::nas_mm().error(
          "Out of buffer space for encoding len %d is less than %u", len,
          m_length + 1);
      return 0;
    }
    *(buf + encoded_size) = m_length;
    encoded_size++;
    *(buf + encoded_size) = m_sst;
    encoded_size++;
    int32_t sd = 0;
    if (Sd(sd)) {
      *(buf + encoded_size) = (sd & 0x00ff0000) >> 16;
      encoded_size++;
      *(buf + encoded_size) = (sd & 0x0000ff00) >> 8;
      encoded_size++;
      *(buf + encoded_size) = sd & 0x000000ff;
      encoded_size++;
    }
    uint8_t mhplmn_sst = 0;
    if (MhplmnSst(mhplmn_sst)) {
      *(buf + encoded_size) = mhplmn_sst;
      encoded_size++;
    }
    int32_t mhplmn_sd = 0;
    if (MhplmnSd(mhplmn_sd)) {
      *(buf + encoded_size) = (mhplmn_sd & 0x00ff0000) >> 16;
      encoded_size++;
      *(buf + encoded_size) = (mhplmn_sd & 0x0000ff00) >> 8;
      encoded_size++;
      *(buf + encoded_size) = mhplmn_sd & 0x000000ff;
      encoded_size++;
    }
    Logger::nas_mm().debug("encoded S_NSSAI len (%d)", encoded_size);
    return encoded_size;
  }

  int DecodeFromBuffer(uint8_t* buf, int len, const bool is_option) {
    Logger::nas_mm().debug("decoding S_NSSAI");
    int decoded_size = 0;
    if (len >= (kSNSSAIValueMinSize + 1)) {
      uint8_t ie_size = *(buf + decoded_size);
      decoded_size++;
      if (len >= ie_size) {
        if (ie_size >= 1) {
          SetSst(*(buf + decoded_size));
          decoded_size++;
          if (ie_size >= 4) {
            int32_t sd = *(buf + decoded_size);
            decoded_size++;
            sd = sd << 8;
            sd |= *(buf + decoded_size);
            decoded_size++;
            sd = sd << 8;
            sd |= *(buf + decoded_size);
            sd &= 0x00ffffff;
            decoded_size++;
            SetSd(sd);
          }
        }
        switch (ie_size) {
          case 1:
            break;
          case 2: {
            SetMhplmnSst(*(buf + decoded_size));
            decoded_size++;
          } break;
          case 4:
            break;
          case 5: {
            SetMhplmnSst(*(buf + decoded_size));
            decoded_size++;
          } break;
          case 8: {
            int32_t sd = *(buf + decoded_size);
            decoded_size++;
            sd = sd << 8;
            sd |= *(buf + decoded_size);
            decoded_size++;
            sd = sd << 8;
            sd |= *(buf + decoded_size);
            sd &= 0x00ffffff;
            decoded_size++;
            SetMhplmnSd(sd);
          } break;
          default:  // Error
                    ;
        }
        Logger::nas_mm().debug(
            "Decoded S_NSSAI len %d %s", decoded_size, ToString().c_str());
      }
    }
    return decoded_size;
  }
} SNSSAI_t;

typedef struct {
  uint8_t ie_type;
  uint8_t ie_len;
  bstring ie_value;
} IE_t;
typedef struct {
  uint8_t ie_type;
  uint16_t ie_len;
  bstring ie_value;
} IE_t_E;
typedef struct {
  uint8_t payloadContainerType : 4;
  std::vector<IE_t> optionalIE;
} PayloadContainerEntry;
typedef struct IMEISV_s {
  bstring identity;
} IMEISV_t;

typedef struct {
  string mcc;
  string mnc;
} nas_plmn_t;

typedef struct {
  uint8_t type;
  std::vector<nas_plmn_t> plmn_list;
  std::vector<uint32_t> tac_list;
} p_tai_t;

}  // namespace nas

#endif
