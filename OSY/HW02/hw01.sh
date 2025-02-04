#!/usr/bin/env bash
declare -a filePaths
declare numberOfLines
declare firstLine
declare targetPath
makeTar=false
display_description(){
	cat << EOF
This bash script will will read your input in format "PATH path/to/file/or/dir/or/link and will print out information about object you gave path to.

This program also can make tar package of files.

	To do so start program with -z switch. 

To open description again start program with -h switch.

Usage:

./hw01.sh -z

EOF
}
while getopts "hz" opt; do 
	case $opt in
		h)
			display_description
			exit 0
			;;
		z)
			makeTar=true
			;;
		\?)
			exit 2
			;;
	esac
done
returnValue=0
while IFS= read -r line;
do
        if [ -z "$line" ]; then
               	break
        else
              	if [[ ! "$line" =~ ^"PATH " ]]; then
                       	continue
               	fi
        	path="${line#* }"
		if [[ ! -e "$path" ]] && [[ ! -L "$path" ]]; then
                	returnValue=1
			echo "ERROR ""'$path'""" >&2
			continue
        	fi
		if [[ -L "$path" ]] || [[ ! -a "$path" ]]; then
			targetPath=$(readlink "$path")
			echo "LINK '$path' '$targetPath'"
		elif [ -f "$path" ]; then
			if [[ ! -r "$path" ]]; then
				exit 2
			fi
			filePaths+=("$path")
			numberOfLines=$(wc -l < "$path")
			if [ "$numberOfLines" = 0 ]; then
				firstLine=""
			else
				firstLine=$(head -n 1 "$path")
			fi
			echo "FILE '$path' $numberOfLines '$firstLine'"
		elif [ -d "$path" ]; then
                        echo "DIR '$path'"

		else
                	returnValue=1
                	echo "ERROR '$path'" >&2
        	fi
	fi
done
if [ "$makeTar" = true ]; then
	if [ ! "${filePaths[*]}" = "" ]; then
		#if ! tar czf output.tgz "${filePaths[@]}" 2> tar.txt; then
		#	errors=$(<tar.txt)
		#	if [[ $errors == *"Cannot open: Permission denied"* ]]; then
		#		exit 2
		#	else
		#		exit 0
		#	fi
		#else
		#	echo "TAR ERROR" >&2
		#	exit 2
		#fi
		declare -a absolutePaths=""
		#echo filePaths: "${filePaths[@]}"
		for relativePath in "${filePaths[@]}"; do
			if [ ! "$relativePath" = "" ];then
				absolutePaths=("$(realpath "$relativePath")")
			fi
		done
		#echo absolutePaths: "${absolutePaths[@]}"
		tar -czf output.tgz "${absolutePaths[*]}"
		tarExitCode=$?
		echo "$tarExitCode"
		if [ "$tarExitCode" = "0" ]; then
			exit 0
		else
			echo "TAR ERROR" >&2
			exit 2
		fi
	fi
fi
exit "$returnValue"
