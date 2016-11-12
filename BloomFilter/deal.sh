#!/bin/bash
>1.txt
dos2unix data.txt
cat data.txt | while read line
do
	echo "${line}tail" >> 1.txt
done	
>data.txt
cat 1.txt>data.txt
rm -rf 1.txt
