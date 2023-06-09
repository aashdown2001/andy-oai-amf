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

#ifndef _SECURITYRESULT_H_
#define _SECURITYRESULT_H_

#include "ConfidentialityProtectionResult.hpp"
#include "IntegrityProtectionResult.hpp"

extern "C" {
#include "Ngap_SecurityResult.h"
}

namespace ngap {

class SecurityResult {
 public:
  SecurityResult();
  virtual ~SecurityResult();

  void setSecurityResult(
      IntegrityProtectionResult* m_integrityProtectionResult,
      ConfidentialityProtectionResult* m_confidentialityProtectionResult);
  bool getSecurityResult(
      IntegrityProtectionResult*& m_integrityProtectionResult,
      ConfidentialityProtectionResult*& m_confidentialityProtectionResult);

  bool encode2SecurityResult(Ngap_SecurityResult_t*);
  bool decodefromSecurityResult(Ngap_SecurityResult_t*);

 private:
  IntegrityProtectionResult* integrityProtectionResult;
  ConfidentialityProtectionResult* confidentialityProtectionResult;
};

}  // namespace ngap

#endif
