#include "UEPagingIdentity.hpp"

namespace ngap {
UEPagingIdentity::UEPagingIdentity(){}
UEPagingIdentity::~UEPagingIdentity(){}

void UEPagingIdentity::setUEPagingIdentity(std::string& setid,std::string& pointer,std::string& tmsi)
{
  fiveGSTmsi.setValue(setid, pointer, tmsi);
}
void UEPagingIdentity::getUEPagingIdentity(std::string& _5g_s_tmsi)
{
  fiveGSTmsi.getValue(_5g_s_tmsi);
}

void UEPagingIdentity::getUEPagingIdentity(std::string& setid,std::string& pointer,std::string& tmsi)
{
  fiveGSTmsi.getValue(setid, pointer, tmsi);
}
bool UEPagingIdentity::encode2pdu(Ngap_UEPagingIdentity_t* pdu)
{
  pdu->present = Ngap_UEPagingIdentity_PR_fiveG_S_TMSI;
  Ngap_FiveG_S_TMSI_t* ie = (Ngap_FiveG_S_TMSI_t *)calloc(1,sizeof(Ngap_FiveG_S_TMSI_t));
  pdu->choice.fiveG_S_TMSI = ie;
  if(!fiveGSTmsi.encode2pdu(pdu->choice.fiveG_S_TMSI)) return false;

  return true;
}
bool UEPagingIdentity::decodefrompdu(Ngap_UEPagingIdentity_t pdu)
{
  if(pdu.present != Ngap_UEPagingIdentity_PR_fiveG_S_TMSI) return false;
  if(!fiveGSTmsi.decodefrompdu(*pdu.choice.fiveG_S_TMSI)) return false;

  return true;
}
}

