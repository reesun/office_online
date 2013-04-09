#!/bin/sh

# Set the memory managment
# -m max memory (RSS)
# -v max virtual memory
ulimit -m 1024 -v 32768 -c 0

#check if anything in argument string. If not then don't try to render
if [ "$3" != "" ]
then
    # decide what to do based on the format
    if [ $3 = "4" ]
    then
	# Word doc
	#echo "Converting word file..."
	nice /usr/bin/wvHtml --targetdir=$1$2 "$1$2.infile" index.html >> /var/log/office/date.log

    elif [ $3 = "2" ] || [ $3 = "3" ]
    then
	# pdf or ps

	#after download - convert to pcx
	#echo "Converting ps or pdf..."
	nice /usr/bin/ghostscript -sDEVICE=pcx256 -r150x150 -sOutputFile="$1$2/tmp%d" "$1$2.infile" >> /var/log/office/date.log

	#from pcx->gif

	#echo "<br>" 
	for dff in $1$2/*
	do 
	    #echo "Converting $dff" 
	    nice /usr/bin/convert "$dff" "$dff".gif   >> /var/log/office/date.log
	    rm "$dff"
	done
    else
	# skip
	echo "skip"
    fi
else
    echo "<br>Internal error<br>"
    echo "$1, $2, $3"
fi




