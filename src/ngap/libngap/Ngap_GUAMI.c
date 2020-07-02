/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice -gen-PER -D src`
 */

#include "Ngap_GUAMI.h"

#include "Ngap_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_Ngap_GUAMI_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_GUAMI, pLMNIdentity),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_PLMNIdentity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pLMNIdentity"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_GUAMI, aMFRegionID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_AMFRegionID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"aMFRegionID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_GUAMI, aMFSetID),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_AMFSetID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"aMFSetID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_GUAMI, aMFPointer),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_AMFPointer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"aMFPointer"
		},
	{ ATF_POINTER, 1, offsetof(struct Ngap_GUAMI, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_ProtocolExtensionContainer_175P59,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_Ngap_GUAMI_oms_1[] = { 4 };
static const ber_tlv_tag_t asn_DEF_Ngap_GUAMI_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Ngap_GUAMI_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pLMNIdentity */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* aMFRegionID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* aMFSetID */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* aMFPointer */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_Ngap_GUAMI_specs_1 = {
	sizeof(struct Ngap_GUAMI),
	offsetof(struct Ngap_GUAMI, _asn_ctx),
	asn_MAP_Ngap_GUAMI_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_Ngap_GUAMI_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Ngap_GUAMI = {
	"GUAMI",
	"GUAMI",
	&asn_OP_SEQUENCE,
	asn_DEF_Ngap_GUAMI_tags_1,
	sizeof(asn_DEF_Ngap_GUAMI_tags_1)
		/sizeof(asn_DEF_Ngap_GUAMI_tags_1[0]), /* 1 */
	asn_DEF_Ngap_GUAMI_tags_1,	/* Same as above */
	sizeof(asn_DEF_Ngap_GUAMI_tags_1)
		/sizeof(asn_DEF_Ngap_GUAMI_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Ngap_GUAMI_1,
	5,	/* Elements count */
	&asn_SPC_Ngap_GUAMI_specs_1	/* Additional specs */
};

