#!/usr/bin/env bash
declare prefix
display_description(){
	cat << EOF
This bash script can:
	- Find all PDF files in dirrectory.
	- Read input text and print all lines that starts with number (without this number).
	- Read input text and print all sentences in new line.
	- Change directive "#include" in C code so it would start with your text.

	To do so start program with switch -h, -a, -b, -c or -d.

Only one switch can be proccessed per one script run.

To open description again start program with -h switch.

Usage:

./hw02.sh [-h] [-a] [-b] [-c] [-d]

EOF
}


find_pdf(){
	files=$(find . -maxdepth 1 -type f -iname "*.pdf" -exec basename {} \; | sort)
	if [ -n "$files" ]; then
		echo "$files"
	fi
}


find_lines(){
	while IFS= read -r line;
	do
		if [ -z "$line" ]; then
			break
		fi
		if [[ "$line" =~ ^[+-]?[0-9]+ ]]; then
			echo "$line" | sed -n 's/^[+-]\{0,1\}[0-9]\{1,\}//p'
		fi
	done
}


find_quotes(){
	export LC_ALL=cs_CZ.UTF-8
	declare result
	while IFS= read -r line;
	do
		if [ -z "$line" ]; then
			break
		fi	
		line=$(echo "$line" | tr -d '\n')
		last_char=$(echo "$line" | tail -c 2)
		start=$(echo "$line" | cut -c 1)
		if [[ "$start" != " " ]]; then
 			line=" $line"
		fi
		if [[ "$last_char" != '.' && "$last_char" != '!' && "$last_char" != '?' ]]; then
			result+="$line"
		else
			result+="$line"
			if [[ $(echo "$result" | cut -c 1) = " " ]]; then
				result="${result:1}"
			fi
			echo "$result"
			result=""
		fi
	done
}


change_directive(){
	#code=$(cat)
	#modified=$(echo "$code" | sed -E "s|#include[[:space:]]*\"([^\"<>]*)\"|#include \"$prefix\1\"|g; s|#include[[:space:]]+<([^\"<>]*)>|#include <$prefix\1>|g")
	#echo "$modified"
	declare space
	while IFS= read -r line; 
	do
		if [[ "$line" =~ ^\#include ]]; then
			space=$(echo "$line" | sed -n 's/\#include\([[:blank:]]*\)\(.*\)/\1/p')			
			echo space: "$space"
		else
			space=$(echo "$line" | sed -n 's/ /p')  #change
			echo space: "$space"
		fi
		modified=$(echo "$line" | sed -E "s|#include[[:blank:]]*\"([^\"<>]*)\"|#include$space\"$prefix\1\"|g; s|#include[[:blank:]]*<([^\"<>]*)>|#include$space<$prefix\1>|g")
		echo "$modified"
	done
}


while getopts "habcd:" opt; do
	case $opt in
		h)
			display_description
			exit 0
			;;
		a)
			find_pdf
			exit 0
			;;
		b)
			find_lines
			exit 0
			;;
		c)
			find_quotes
			exit 0 
			;;
		d)
			prefix="$OPTARG"
			change_directive
			exit 0
			;;
		\?)
			display_description
			exit 1
			;;
	esac
done
