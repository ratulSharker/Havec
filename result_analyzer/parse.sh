# This is a script for build the latest
# resultParser.ResultParser java to 
# compile & parse all the result in
# result_($date).html file

# This step is only requires once
# once the ResultParser.class file is 
# available, no need to run following command
echo 'building the parser'
javac resultparser/ResultParser.java
echo 'building complete'
echo

echo 'running the parser'
outputFile="result_`date`.html"
java resultparser.ResultParser all_results > $outputFile
echo 'parsing done'
echo 
echo 'see the result : ' $outputFile 
