#ifndef _UEPAGINGIDENTITY_H_
#define _UEPAGINGIDENTITY_H_

#include "FiveGSTmsi.hpp"

extern "C" {
#include "Ngap_UEPagingIdentity.h"
}

namespace ngap {

class UEPagingIdentity {
 public:
  UEPagingIdentity();
  virtual ~UEPagingIdentity();

  void setUEPagingIdentity(std::string& setid,std::string& pointer,std::string& tmsi);
  void getUEPagingIdentity(std::string& _5g_s_tmsi);
  void getUEPagingIdentity(std::string& setid,std::string& pointer,std::string& tmsi);
  bool encode2pdu(Ngap_UEPagingIdentity_t* pdu);
  bool decodefrompdu(Ngap_UEPagingIdentity_t pdu);

 private:
  FiveGSTmsi fiveGSTmsi;
};

}  // namespace ngap



#endif

