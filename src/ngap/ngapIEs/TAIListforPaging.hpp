#ifndef _TAILISTFORPAGING_H_
#define _TAILISTFORPAGING_H_

#include "TAI.hpp"

extern "C" {
#include "Ngap_TAIListForPaging.h"
}

namespace ngap {

class TAIListForPaging {
 public:
  TAIListForPaging();
  virtual ~TAIListForPaging();

  void setTAIListForPaging(TAI* m_tai, int numOfItem);
  void getTAIListForPaging(TAI*& m_tai, int& numOfItem);
  bool encode2TAIListForPaging(Ngap_TAIListForPaging_t* pdu);
  bool decodefromTAIListForPaging(Ngap_TAIListForPaging_t *pdu);

 private:
  TAI *tai;
  int numOftai;
};

}  // namespace ngap



#endif


