/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice -gen-PER -D src`
 */

#include "Ngap_AMF-TNLAssociationToRemoveItem.h"

#include "Ngap_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_Ngap_AMF_TNLAssociationToRemoveItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_AMF_TNLAssociationToRemoveItem, aMF_TNLAssociationAddress),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_Ngap_CPTransportLayerInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"aMF-TNLAssociationAddress"
		},
	{ ATF_POINTER, 1, offsetof(struct Ngap_AMF_TNLAssociationToRemoveItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_ProtocolExtensionContainer_175P4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_Ngap_AMF_TNLAssociationToRemoveItem_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Ngap_AMF_TNLAssociationToRemoveItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* aMF-TNLAssociationAddress */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_Ngap_AMF_TNLAssociationToRemoveItem_specs_1 = {
	sizeof(struct Ngap_AMF_TNLAssociationToRemoveItem),
	offsetof(struct Ngap_AMF_TNLAssociationToRemoveItem, _asn_ctx),
	asn_MAP_Ngap_AMF_TNLAssociationToRemoveItem_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_Ngap_AMF_TNLAssociationToRemoveItem_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem = {
	"AMF-TNLAssociationToRemoveItem",
	"AMF-TNLAssociationToRemoveItem",
	&asn_OP_SEQUENCE,
	asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1,
	sizeof(asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1)
		/sizeof(asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1[0]), /* 1 */
	asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1)
		/sizeof(asn_DEF_Ngap_AMF_TNLAssociationToRemoveItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Ngap_AMF_TNLAssociationToRemoveItem_1,
	2,	/* Elements count */
	&asn_SPC_Ngap_AMF_TNLAssociationToRemoveItem_specs_1	/* Additional specs */
};

