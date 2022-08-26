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

#include "UEAssociatedLogicalNGConnectionList.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

#include <iostream>
using namespace std;

namespace ngap {

UEAssociatedLogicalNGConnectionList::UEAssociatedLogicalNGConnectionList() {
  ueAssociatedLogicalNGConnectionItem = nullptr;
  number_of_items                     = 0;
}

UEAssociatedLogicalNGConnectionList::~UEAssociatedLogicalNGConnectionList() {
  if (ueAssociatedLogicalNGConnectionItem)
    delete[] ueAssociatedLogicalNGConnectionItem;
}
//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::
    setUEAssociatedLogicalNGConnectionItem(
        UEAssociatedLogicalNGConnectionItem*
            m_UEAssociatedLogicalNGConnectionItem,
        int num) {
  ueAssociatedLogicalNGConnectionItem = m_UEAssociatedLogicalNGConnectionItem;
  number_of_items                     = num;
}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::
    getUEAssociatedLogicalNGConnectionItem(
        UEAssociatedLogicalNGConnectionItem*&
            m_UEAssociatedLogicalNGConnectionItem,
        int& num) {
  m_UEAssociatedLogicalNGConnectionItem = ueAssociatedLogicalNGConnectionItem;
  num                                   = number_of_items;
}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::
    setUEAssociatedLogicalNGConnectionItem(
        std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  number_of_items = list.size();
  ueAssociatedLogicalNGConnectionItem =
      new UEAssociatedLogicalNGConnectionItem[number_of_items]();
  for (int i = 0; i < number_of_items; i++) {
    ueAssociatedLogicalNGConnectionItem[i].set(list[i]);
  }
  return;
}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::
    getUEAssociatedLogicalNGConnectionItem(
        std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  for (int i = 0; i < number_of_items; i++) {
    list.push_back(ueAssociatedLogicalNGConnectionItem[i]);
  }
}

//------------------------------------------------------------------------------
bool UEAssociatedLogicalNGConnectionList::encode(
    Ngap_UE_associatedLogicalNG_connectionList_t*
        ue_associatedLogicalNG_connectionList) {
  for (int i = 0; i < number_of_items; i++) {
    Ngap_UE_associatedLogicalNG_connectionItem_t* item =
        (Ngap_UE_associatedLogicalNG_connectionItem_t*) calloc(
            1, sizeof(Ngap_UE_associatedLogicalNG_connectionItem_t));
    if (!item) return false;
    if (!ueAssociatedLogicalNGConnectionItem[i].encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&ue_associatedLogicalNG_connectionList->list, item) !=
        0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UEAssociatedLogicalNGConnectionList::decode(
    Ngap_UE_associatedLogicalNG_connectionList_t*
        ue_associatedLogicalNG_connectionList) {
  number_of_items = ue_associatedLogicalNG_connectionList->list.count;
  ueAssociatedLogicalNGConnectionItem =
      new UEAssociatedLogicalNGConnectionItem[number_of_items]();
  for (int i = 0; i < number_of_items; i++) {
    if (!ueAssociatedLogicalNGConnectionItem[i].decode(
            ue_associatedLogicalNG_connectionList->list.array[i]))
      return false;
  }
  return true;
}

}  // namespace ngap
