#include "TAIListforPaging.hpp"

extern "C" {
#include "Ngap_TAIListForPagingItem.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
TAIListForPaging::TAIListForPaging() {
  tai         = NULL;
  numOftai    = 0;
}

//------------------------------------------------------------------------------
TAIListForPaging::~TAIListForPaging() {
  if(!tai) delete [] tai;
}

//------------------------------------------------------------------------------
bool TAIListForPaging::encode2TAIListForPaging(Ngap_TAIListForPaging_t* pdu) {
  cout << "TAIListForPaging::numberOfItem  "<< numOftai << endl;
  for (int i = 0; i < numOftai; i++) {
    Ngap_TAIListForPagingItem_t* ta =
        (Ngap_TAIListForPagingItem_t*) calloc(1, sizeof(Ngap_TAIListForPagingItem_t));
    if (!tai[i].encode2TAI(&ta->tAI)) return false;
    if (ASN_SEQUENCE_ADD(&pdu->list, ta) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool TAIListForPaging::decodefromTAIListForPaging(Ngap_TAIListForPaging_t *pdu) {
  numOftai = pdu->list.count;
  tai         = new TAI[numOftai];
  for (int i = 0; i < numOftai; i++) {
    if (!tai[i].decodefromTAI(&pdu->list.array[i]->tAI))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void TAIListForPaging::setTAIListForPaging(TAI* m_tai, int numOfItem) {
  tai         = m_tai;
  numOftai = numOfItem;
}

//------------------------------------------------------------------------------
void TAIListForPaging::getTAIListForPaging(TAI*& m_tai, int& numOfItem) {
  m_tai = tai;
  numOfItem         = numOftai;
}

}  // namespace ngap

