/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice
 * -gen-PER -D src`
 */

#ifndef _Ngap_BroadcastCompletedAreaList_H_
#define _Ngap_BroadcastCompletedAreaList_H_

#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Ngap_BroadcastCompletedAreaList_PR {
  Ngap_BroadcastCompletedAreaList_PR_NOTHING, /* No components present */
  Ngap_BroadcastCompletedAreaList_PR_cellIDBroadcastEUTRA,
  Ngap_BroadcastCompletedAreaList_PR_tAIBroadcastEUTRA,
  Ngap_BroadcastCompletedAreaList_PR_emergencyAreaIDBroadcastEUTRA,
  Ngap_BroadcastCompletedAreaList_PR_cellIDBroadcastNR,
  Ngap_BroadcastCompletedAreaList_PR_tAIBroadcastNR,
  Ngap_BroadcastCompletedAreaList_PR_emergencyAreaIDBroadcastNR,
  Ngap_BroadcastCompletedAreaList_PR_choice_Extensions
} Ngap_BroadcastCompletedAreaList_PR;

/* Forward declarations */
struct Ngap_CellIDBroadcastEUTRA;
struct Ngap_TAIBroadcastEUTRA;
struct Ngap_EmergencyAreaIDBroadcastEUTRA;
struct Ngap_CellIDBroadcastNR;
struct Ngap_TAIBroadcastNR;
struct Ngap_EmergencyAreaIDBroadcastNR;
struct Ngap_ProtocolIE_SingleContainer;

/* Ngap_BroadcastCompletedAreaList */
typedef struct Ngap_BroadcastCompletedAreaList {
  Ngap_BroadcastCompletedAreaList_PR present;
  union Ngap_BroadcastCompletedAreaList_u {
    struct Ngap_CellIDBroadcastEUTRA* cellIDBroadcastEUTRA;
    struct Ngap_TAIBroadcastEUTRA* tAIBroadcastEUTRA;
    struct Ngap_EmergencyAreaIDBroadcastEUTRA* emergencyAreaIDBroadcastEUTRA;
    struct Ngap_CellIDBroadcastNR* cellIDBroadcastNR;
    struct Ngap_TAIBroadcastNR* tAIBroadcastNR;
    struct Ngap_EmergencyAreaIDBroadcastNR* emergencyAreaIDBroadcastNR;
    struct Ngap_ProtocolIE_SingleContainer* choice_Extensions;
  } choice;

  /* Context for parsing across buffer boundaries */
  asn_struct_ctx_t _asn_ctx;
} Ngap_BroadcastCompletedAreaList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Ngap_BroadcastCompletedAreaList;
extern asn_CHOICE_specifics_t asn_SPC_Ngap_BroadcastCompletedAreaList_specs_1;
extern asn_TYPE_member_t asn_MBR_Ngap_BroadcastCompletedAreaList_1[7];
extern asn_per_constraints_t
    asn_PER_type_Ngap_BroadcastCompletedAreaList_constr_1;

#ifdef __cplusplus
}
#endif

#endif /* _Ngap_BroadcastCompletedAreaList_H_ */
#include <asn_internal.h>
