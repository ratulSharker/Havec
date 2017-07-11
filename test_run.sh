#!/bin/bash

if [ -e Havec.out ];
then
	# cleaning the existing output
	rm -rf output/
	rm -rf result_analyzer/all_results/

	mkdir result_analyzer/all_results/

	# run the havec with 2 input file
	# in different configuration
	# 20 MB
	./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 20.0
	./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 20.0
	./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 20.0
	./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 20.0
	echo 'set 1/6 complete'

	# 16MB
	./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 16.0
	./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 16.0
	./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 16.0
	./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 16.0
	echo 'set 2/6 complete'

	# 12MB
	./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 12.0
	./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 12.0
	./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 12.0
	./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 12.0
	echo 'set 3/6 complete'

	# 8MB
	./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 8.0
	./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 8.0
	./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 8.0
	./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 8.0
	echo 'set 4/6 complete'

	# 6MB
	./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 6
	./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 6
	./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 6
	./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 6
	echo 'set 5/6 complete'

	# 4MB
	./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 4
	./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 4
	./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 4
	./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 4
	echo 'set 6/6 complete'

	echo 'moving log to parser'
	mv output/* result_analyzer/all_results/

	# parsing the result
	cd result_analyzer
	sh parse.sh
	mv *.html ../
	
	cd ..
else
	echo 'run `make` or `make havec` to build the havec first'
fi


