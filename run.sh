#!/bin/bash

# cleaning the existing output
rm -rf output/
rm -rf result_analyzer/all_results/

mkdir result_analyzer/all_results/

# run the havec with 2 input file
# in different configuration
# 10 MB
./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 10.0
./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 10.0
./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 10.0
./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 10.0
echo 'set 1/6 complete'

# 8MB
./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 8.0
./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 8.0
./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 8.0
./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 8.0
echo 'set 2/6 complete'

# 6MB
./Havec.out "input_files/sequential.fa" 32 8 C NP 1048576 6.0
./Havec.out "input_files/sequential.fa" 27 4 C NP 1048576 6.0
./Havec.out "input_files/sequential.fa" 32 8 NC NP 1048576 6.0
./Havec.out "input_files/sequential.fa" 27 4 NC NP 1048576 6.0
echo 'set 3/6 complete'

# 1 MB
./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 1.0
./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 1.0
./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 1.0
./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 1.0
echo 'set 4/6 complete'

# 0.8MB
./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 0.8
./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 0.8
./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 0.8
./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 0.8
echo 'set 5/6 complete'

# 0.6MB
./Havec.out "input_files/shortfile.fa" 32 8 C NP 1048576 0.6
./Havec.out "input_files/shortfile.fa" 27 4 C NP 1048576 0.6
./Havec.out "input_files/shortfile.fa" 32 8 NC NP 1048576 0.6
./Havec.out "input_files/shortfile.fa" 27 4 NC NP 1048576 0.6
echo 'set 6/6 complete'

echo 'moving log to parser'
mv output/* result_analyzer/all_results/

cd result_analyzer

sh parse.sh