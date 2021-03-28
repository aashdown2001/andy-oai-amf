#!/bin/bash

function generate_install_package()
{
	local lib_dir=""
	local libs_dir=""
	local res=""
	local res_new=""
	local result=false
	
	echo "[Executable File]$1"
	echo "[Config File] $2"
	echo "[Generate Dir] $3"

	if [ -n "$1" ] && [ -n "$2" ] && [ -n "$3" ]; then
		if [ -f "$1" ] && [ -f "$2" ];then
			res="$(ldd $1)"
			for ((i=1; i<=100; i++))
			do
				res_new="${res#*/}"
				if [ "$res_new" == "$res" ]; then
					result=true
					break
				fi
				lib_dir="/${res_new%%(*} "
				libs_dir+="$lib_dir "
				res="${res_new#* }"
			done
		else
	
			echo "\"$1\" Or \"$2\" does not exist!"
		fi
	
		if [ $result == true ]; then
			if [ -d "$3" ]; then
				rm -r $3
			fi
			
			mkdir $3 $3/libs
			cp $libs_dir $3/libs/
			cp $1 $3
			cp $2 $3/default.conf
	
			cd $3
			cat>start.sh<<EOF
#!/bin/bash
cd \`dirname \$0\`
ROOT_DIR=\$(pwd)

if [ -f "\$ROOT_DIR/external.conf" ]; then
    echo -e "\E[33mUsing external configuration \E[00m"
    \$ROOT_DIR/libs/ld-linux-x86-64.so.2 --library-path \$ROOT_DIR/libs/ \$ROOT_DIR/$(basename $1) -c \$ROOT_DIR/external.conf -o
else
    echo -e "\E[33mUsing default configuration \E[00m"
    \$ROOT_DIR/libs/ld-linux-x86-64.so.2 --library-path \$ROOT_DIR/libs/ \$ROOT_DIR/$(basename $1) -c \$ROOT_DIR/default.conf -o
fi
EOF
	
			chmod 777 start.sh

			return 0
		else
			echo "Extract library failed!"
			return 1
		fi
	else
	    echo "$0 [Executable File] [Config File] [Generate Dir]"
	fi
}


#generate_install_package "$@"

