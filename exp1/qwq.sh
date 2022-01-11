#!/bin/bash
str="#LXY# $USER `date +%Y-%m-%d,%H:%M:%S`"
for filename in $1/*.txt
do
	if [ 0 -eq `grep -c  "#LXY#" $filename` ];then
		echo $str >> $filename
	else 
		sed -i "/#LXY#/c$str" $filename
	fi
done

