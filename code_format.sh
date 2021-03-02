#!/bin/bash

#**************************************
License_name="/home/root1/desktop/license.txt"
paper=1
str='\n'
#**************************************
until [ -z "$1" ]
do
	case "$1" in
		-p | --file-path-name)
			echo "waiting......."
			filelist=`find $2 -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.c" -o -iname "*.h"`
			for filename in $filelist;do
				if grep -q "OpenAirInterface" $filename
				then
					echo "There's already license in $filename"
				else
					paper=1
					cat $License_name|while read LINE
				do
					if [ -z "$LINE" ]
					then
						sed -i "${paper}i\\$str" $filename
					else
						sed -i "${paper}i\\$LINE" $filename
					fi
					paper=`expr $paper + 1`
				done
				echo "add license successfully to $filename"
			fi
			clang-format -style=Google -i $filename
		done
		echo "clang-format and add license successfully"
		break
		;;
		-f | --file-name)
			echo "file name is $2"
			if grep -q "OpenAirInterface" $2 
			then
				echo "There's already license in $2"
			else
				cat $License_name|while read LINE
			do
				if [ -z "$LINE" ]
				then
					sed -i "${paper}i\\$str" $2
				else
					sed -i "${paper}i\\$LINE" $2						
				fi
				paper=`expr $paper + 1`
			done
			echo "add license successfully to $2"
		fi
		clang-format -style=Google -i $2
		echo "clang-format and add license successfully"
		break
		;;
	*)
		echo "unknown option $1" 
		break
		;;
esac
done
