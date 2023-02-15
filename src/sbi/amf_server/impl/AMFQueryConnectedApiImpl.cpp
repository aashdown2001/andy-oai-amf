#include "AMFQueryConnectedApiImpl.h"

#include <nlohmann/json.hpp>

#include "amf_statistics.hpp"

extern statistics stacs;

#define GNBS_NUM_MAX 20
#define UES_NUM_MAX 20

namespace oai {
namespace amf {
namespace api {

AMFQueryConnectedApiImpl::AMFQueryConnectedApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr,
    amf_application::amf_app* amf_app_inst)
    : AMFQueryConnectedApi(rtr), m_amf_app(amf_app_inst) {}

void AMFQueryConnectedApiImpl::gnbs_get_am_data(
    Pistache::Http::ResponseWriter& response) {
  response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
  nlohmann::json response_data_json = {};
  nlohmann::json json_array         = nlohmann::json::array();

  std::vector<gnb_infos> gnbs_info;
  stacs.get_gnb_list(gnbs_info);

  if (gnbs_info.size() > 0) {
    for (int i = 0; i < gnbs_info.size(); i++) {
      json_array[i]["gnb_name"] = gnbs_info[i].gnb_name.c_str();
      json_array[i]["gnb_plmn"] = (gnbs_info[i].mcc + gnbs_info[i].mnc).c_str();
      json_array[i]["gnb_tac"]  = gnbs_info[i].tac;
      if (GNBS_NUM_MAX == i) {
        break;
      }
    };
    response_data_json["code"] = 200;
    response_data_json["msg"]  = "success";
    response_data_json["data"] = json_array;
  } else {
    response_data_json["code"] = 201;
    response_data_json["msg"]  = "No data";
    response_data_json["data"] = NULL;
  }
  try {
    response.send(Pistache::Http::Code::Ok, response_data_json.dump());
  } catch (nlohmann::json::exception& e) {
    response.send(Pistache::Http::Code::Not_Found, response_data_json.dump());
  }

  gnbs_info.clear();
}

void AMFQueryConnectedApiImpl::ues_get_am_data(
    Pistache::Http::ResponseWriter& response) {
  response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
  nlohmann::json response_data_json = {};
  nlohmann::json json_array         = nlohmann::json::array();
  // Logger::amf_server().info("=====impl========ues==========");

  std::vector<ue_info_t> ues_info;
  stacs.get_ue_list(ues_info);

  if (ues_info.size() > 0) {
    response_data_json["code"] = 200;
    response_data_json["msg"]  = "success";

    for (int i = 0; i < ues_info.size(); i++) {
      json_array[i]["ue_Imsi"]       = ues_info[i].imsi.c_str();
      json_array[i]["ue_5GMM-state"] = ues_info[i].registerStatus.c_str();
      if (UES_NUM_MAX == i) {
        break;
      }
    }
    response_data_json["code"] = 200;
    response_data_json["msg"]  = "success";
    response_data_json["data"] = json_array;
  } else {
    response_data_json["code"] = 201;
    response_data_json["msg"]  = "No data";
    response_data_json["data"] = NULL;
  }
  try {
    response.send(Pistache::Http::Code::Ok, response_data_json.dump());
  } catch (nlohmann::json::exception& e) {
    response.send(Pistache::Http::Code::Not_Found, response_data_json.dump());
  }

  ues_info.clear();
}

}  // namespace api
}  // namespace amf
}  // namespace oai