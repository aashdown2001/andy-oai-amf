#!/bin/bash
set -eo pipefail

STATUS=0
AMF_PORT_FOR_NGAP=38412
AMF_PORT_FOR_SBI_HTTP=80
AMF_IP_NGAP_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_NGAP | grep inet | awk {'print $2'})
AMF_IP_SBI_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_SBI | grep inet | awk {'print $2'})
N2_PORT_STATUS=$(netstat -Snpl | grep -o "$AMF_IP_NGAP_INTERFACE:$AMF_PORT_FOR_NGAP")
SBI_PORT_STATUS=$(netstat -tnpl | grep -o "$AMF_IP_SBI_INTERFACE:$AMF_PORT_FOR_SBI_HTTP")
#Check if entrypoint properly configured the conf file and no parameter is unset (optional)
NB_UNREPLACED_AT=`cat /openair-amf/etc/*.conf | grep -v contact@openairinterface.org | grep -c @ || true`

if [ $NB_UNREPLACED_AT -ne 0 ]; then
	STATUS=1
	echo "Healthcheck error: configuration file is not configured properly"
fi

if [[ -z $N2_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: N2 SCTP port $AMF_PORT_FOR_NGAP is not listening"
fi

if [[ -z $SBI_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: SBI TCP/HTTP port $AMF_PORT_FOR_SBI_HTTP is not listening"
fi

exit $STATUS
