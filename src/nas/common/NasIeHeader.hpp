/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "../ies/5GSMobileIdentity.hpp"
#include "../ies/MICOIndication.hpp"
#include "../ies/NetworkSlicingIndication.hpp"
#include "../ies/PDUSessionStatus.hpp"
#include "../ies/UEStatus.hpp"
#include "NasMmPlainHeader.hpp"
#include "5GMMCapability.hpp"
#include "5GSRegistrationType.hpp"
#include "ABBA.hpp"
#include "Additional_5G_Security_Information.hpp"
#include "Additional_Information.hpp"
#include "AllowedPDUSessionStatus.hpp"
#include "Authentication_Failure_Parameter.hpp"
#include "Authentication_Parameter_AUTN.hpp"
#include "Authentication_Parameter_RAND.hpp"
#include "AuthenticationResponseParameter.hpp"
#include "DNN.hpp"
#include "EapMessage.hpp"
#include "EpsBearerContextStatus.hpp"
#include "EPS_NAS_Message_Container.hpp"
#include "EPS_NAS_Security_Algorithms.hpp"
#include "Extended_DRX_Parameters.hpp"
#include "GprsTimer2.hpp"
#include "GprsTimer3.hpp"
#include "IMEISV_Request.hpp"
#include "LADN_Indication.hpp"
#include "MA_PDU_Session_Information.hpp"
#include "NasMessageContainer.hpp"
#include "NasSecurityAlgorithms.hpp"
#include "NSSAI.hpp"
#include "NssaiInclusionMode.hpp"
#include "NasKeySetIdentifier.hpp"
#include "Non_3GPP_NW_Provided_Policies.hpp"
#include "PDU_Session_Identity_2.hpp"
#include "PDU_Session_Reactivation_Result.hpp"
#include "PDU_Session_Reactivation_Result_Error_Cause.hpp"
#include "PLMN_List.hpp"
#include "Payload_Container.hpp"
#include "PayloadContainerType.hpp"
#include "Rejected_NSSAI.hpp"
#include "Release_Assistance_Indication.hpp"
#include "Request_Type.hpp"
#include "S1_UE_Security_Capability.hpp"
#include "SOR_Transparent_Container.hpp"
#include "S_NSSAI.hpp"
#include "ServiceType.hpp"
#include "UENetworkCapability.hpp"
#include "UEUsageSetting.hpp"
#include "UESecurityCapability.hpp"
#include "UeRadioCapabilityId.hpp"
#include "UplinkDataStatus.hpp"
#include "_5GMM_Cause.hpp"
#include "_5GSDeregistrationType.hpp"
#include "_5GSTrackingAreaIdList.hpp"
#include "_5GS_DRX_Parameters.hpp"
#include "_5GS_Identity_Type.hpp"
#include "_5GS_Network_Feature_Support.hpp"
#include "_5GS_Registration_Result.hpp"
#include "_5GSTrackingAreaIdentity.hpp"
#include "_5gsUpdateType.hpp"
#include "struct.hpp"
#include "Ie_Const.hpp"
#include "NetworkName.hpp"
