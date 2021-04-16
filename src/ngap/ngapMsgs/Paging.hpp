#ifndef _PAGING_H_
#define _PAGING_H_

#include "NgapIEsStruct.hpp"

#include "MessageType.hpp"
#include "UEPagingIdentity.hpp"
#include "TAIListforPaging.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_Paging.h"
}

namespace ngap {

class PagingMsg {
 public:
  PagingMsg();
  virtual ~PagingMsg();

  void setMessageType();
  int encode2buffer(uint8_t* buf, int buf_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);

  void setUEPagingIdentity(std::string SetId, std::string Pointer, std::string tmsi);
  void getUEPagingIdentity(std::string& _5g_s_tmsi);
  void getUEPagingIdentity(std::string& setid,std::string& pointer,std::string& tmsi);

  void setTAIListForPaging(const std::vector<struct Tai_s> list);
  void getTAIListForPaging(std::vector<struct Tai_s>& list);

 private:
  Ngap_NGAP_PDU_t* pagingPdu;
  Ngap_Paging_t* pagingIEs;

  UEPagingIdentity* uePagingIdentity;
  TAIListForPaging* taIListForPaging;
};

}  // namespace ngap

#endif