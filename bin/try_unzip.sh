#!/bin/sh


#check if anything in argument string. If not then don't try to convert
if [ $1 != "" ]
then

    #check if downloaded is a gzipped file and unpack it
    if [ `file $1 | grep gzip|wc -l` = 1 ]
    then
	#echo "Compressed file. Unpacking..."
	mv $1 $1.gz
	nice gunzip $1.gz >> /var/log/office/date.log
    fi

    #check if downloaded is a compressed file and unpack it
    if [ `file $1 | grep compress|wc -l` = 1 ]
    then
	#echo "Compressed file. Unpacking..."
	mv $1 $1.gz
	nice gunzip $1.gz >> /var/log/office/date.log
    fi
else
    echo "<br>Internal error<br>"
    echo "$1"
fi




