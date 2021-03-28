#!/bin/bash

THIS_SCRIPT_PATH=$(dirname $(readlink -f $0))
echo "$THIS_SCRIPT_PATH"

source ${THIS_SCRIPT_PATH}/build_install_package.sh

function build_docker()
{
	docker build -t amf:v1.0 -f ${THIS_SCRIPT_PATH}/../../docker/Dockerfile $1/
	if [[ $? -eq 0 ]]; then
		return 0
	else
		return 1
	fi
}
function main()
{
	local -i apk=0
	local -i docker=0

	until [ -z "$1" ]
    	do
    	case "$1" in
			-g | --generate-type)
				if [ "$2" == "Apk" ]; then
					apk=1
				elif [ "$2" == "Docker" ]; then
					apk=1
					docker=1
				else
					echo "Unknown option \"Apk\" Or \"Docker\""
					return 1
				fi
				shift 2;
				;;
			*)
        		echo "Unknown option $1"
        		return 1
        		;;
		esac
	done

	if [ $apk -ne 0 ]; then
		echo "Generating a green installation package"
		generate_install_package ${THIS_SCRIPT_PATH}/../amf/build/amf ${THIS_SCRIPT_PATH}/../../etc/amf_docker.conf ${THIS_SCRIPT_PATH}/../amf-install-packer
		if [[ $? -eq 0 ]]; then
			echo "Green installation package was generated successfully"
		else
			echo "Green installation package generation failed"
			return 0
		fi
	fi

	if [ $docker -ne 0 ]; then
		echo "Creating a Docker image"
		build_docker ${THIS_SCRIPT_PATH}/../amf-install-packer
		if [[ $? -eq 0 ]]; then
			echo "Docker image created successfully"
		else
			echo "Docker image creation failed"
			return 0
		fi
	fi

}

main "$@"
