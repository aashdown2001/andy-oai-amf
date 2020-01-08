/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice -gen-PER -D src`
 */

#ifndef	_Ngap_SliceOverloadList_H_
#define	_Ngap_SliceOverloadList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Ngap_SliceOverloadItem;

/* Ngap_SliceOverloadList */
typedef struct Ngap_SliceOverloadList {
	A_SEQUENCE_OF(struct Ngap_SliceOverloadItem) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Ngap_SliceOverloadList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Ngap_SliceOverloadList;
extern asn_SET_OF_specifics_t asn_SPC_Ngap_SliceOverloadList_specs_1;
extern asn_TYPE_member_t asn_MBR_Ngap_SliceOverloadList_1[1];
extern asn_per_constraints_t asn_PER_type_Ngap_SliceOverloadList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _Ngap_SliceOverloadList_H_ */
#include <asn_internal.h>
