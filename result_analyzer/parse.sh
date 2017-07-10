#!/bin/bash

# This is a script for build the latest
# resultParser.ResultParser java to 
# compile & parse all the result in
# result_($date).html file

# This step is only requires once
# once the ResultParser.class file is 
# available, no need to run following command
if ! [ -e "./resultparser/ResultParser.class" ]
then
	echo 'building the parser'
	javac resultparser/ResultParser.java
	echo 'building complete'
else
	echo 'parser already present'
fi
echo


# running the parser
echo 'running the parser'
outputFile="result_`date`.html"
java resultparser.ResultParser all_results > $outputFile
echo 'parsing done'
echo 
echo 'see the result : ' $outputFile 
