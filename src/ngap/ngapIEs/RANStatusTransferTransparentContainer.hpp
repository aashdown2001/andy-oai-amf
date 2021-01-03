/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
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
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#ifndef _RANSTATUSTRANSFERTRANSPATENTCONTAINER_H_
#define _RANSTATUSTRANSFERTRANSPATENTCONTAINER_H_
#include "dRBsSubjectToStatusTransferList.hpp"
#include "dRBsSubjectToStatusTransferItem.hpp"
extern "C" {
#include "Ngap_RANStatusTransfer-TransparentContainer.h"
}
namespace ngap {
class RANStatusTransferTransparentContainer {
 private:
  /* data */
  dRBSubjectList *drb_sub_list;

 public:
  RANStatusTransferTransparentContainer(/* args */);
  virtual ~RANStatusTransferTransparentContainer();
  void getdRBSubject_list(dRBSubjectList *&drblist);
  void setdRBSubject_list(dRBSubjectList *drblist);
  bool encoderanstatustransfer_transparentcontainer(
      Ngap_RANStatusTransfer_TransparentContainer_t *ranstatustransfer_transparentcontainer);
  bool decoderanstatustransfer_transparentcontainer(
      Ngap_RANStatusTransfer_TransparentContainer_t *ranstatustransfer_transparentcontainer);
};

}  // namespace ngap
#endif
