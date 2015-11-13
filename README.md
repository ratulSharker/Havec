# Havec

This is a implementation of <b>
Efficient de Bruijn graph construction for genome assembly using a hash table and auxiliary vector data structures</b>. 

## Pre-requisite
To use this software you need <b>g++</b> (GCC 4.7 or later to support -std=c++11) must be pre-installed. For more information about the compiler [here](https://gcc.gnu.org/projects/cxx0x.html).

## Build instruction
Necessary script are already written to compile this software. Just run the [build.sh](https://github.com/ratulSharker/Havec/blob/master/build.sh) script. It will generate the executable <b>Havec.out</b> for you in the same directory.

```shell
  cd Havec
  sh build.sh
```

## Supported parameter
This software accept a number of parameter to run on different data-set. They all are listed following. Those who are impatient, can have a look at the following,

```
  ./Havec.out [file-path] [k-size] [Number of hash function] [track-cutoff] [print-verbose] [Hashtable min size] [Multiplying-factor]
```

All the parameter ar listed following
  1. File-path
  2. k-size
  3. Number of Hash function
  4. Track Cutoff
  5. Print-verbose
  6. Hashtable min size
  7. Multiplying factor

**1. File-path** Input sequence file in fasta or fastq format <br/>
**2. k-size**   The size of the khmer. Until now we support upto 32.<br/>
**3. Number of Hash function** To insert the khmer in our Hashtable data-structure we use a number of hash function upon collision. As the number of this hash function is higher, less collision are assumed to occur. Until now we support upto value 8.<br/>
**4. Track-Cutoff** In genome assembly, some algorithm requires the cutoff value of a khmer, some doesn't. To track the cut-off value this parameter is responsible. To enable Cutoff tracking use **C**, otherwise use **NC**.<br/>
**5. Print-verbose** To print out the activity enable this. But this will flood you with the messages. Turn it on for the devlopment purpose & for extra log. To enable use **P** otherhand use **NP**.<br/>
**6. Hashtable min size** This is the value which will be the used for the hashtable. Exactly this value will not be used, next prime of this value will be used as the hash-table size for more collision proof.<br/>
**7. Multiplying Factor** This value is in aid to take as a multiplying factor in hashtable size. This value will be Multiplied with the 6th parameter to increase the hashtable size. This value is just for ease of usage.


## Sample Test run

Some of the test run are given following

```shell
./Havec.out dataset_1_7GB.fa 32 8 NC NP 138568143 1.4
./Havec.out dataset_1_9GB.fa 32 8 NC NP 113775137 1.4
./Havec.out Ecoli_MG1655_s_6_1_bfast.fasta 32 8 NC NP 13795561 1.4
./Havec.out Ecoli_MG1655_s_6_2_bfast.fasta 32 8 NC NP 133958572 1.4
./Havec.out NA19240_GAIIx_100_chr21.fasta 32 8 NC NP 113775137 1.4
./Havec.out Human1_95G_CASAVA1.8a2_NCBI37_18Jan11_chr19.sorted.fasta 32 8 NC NP 222896820 1.4
./Havec.out Human1_95G_CASAVA1.8a2_NCBI37_18Jan11_chr21.sorted.fasta 32 8 NC NP 138568143 1.4
```

## Using the Result Analyzer

Running the <b>Havec</b> tool will output the result in a human readable text file (file with a txt extension). Created output filename will follow the input parameter. Each run of the tool single text formatted output. In each output file there will be verbose data, to sort-out the exact very necessary data a [result_analyzer](https://github.com/ratulSharker/Havec/tree/master/result_analyzer) tool is attached with the repository.

To use the [result_analyzer](https://github.com/ratulSharker/Havec/tree/master/result_analyzer) tool first make a <b>all_results</b> empty directory inside the [result_analyzer](https://github.com/ratulSharker/Havec/tree/master/result_analyzer) directory. Then put all the resulted output text file into this directory & run the [parse.sh](https://github.com/ratulSharker/Havec/blob/master/result_analyzer/parse.sh) tool using the following command

```shell
sh parse.sh
```

This [parse.sh](https://github.com/ratulSharker/Havec/blob/master/result_analyzer/parse.sh) will create a html file named after the current system date & time. This html file will contain a table with all the data in the <b>all_results</b> directory.

## A brief story of the Result Analyzer

The result analyzer actually a java program, which parse out all the results inside a directory. It parses every single line inside the text file which is the output of the <b>Havec</b> executable. So the input to the java programme is a directory containing all the resulted text file. The input directory in our case is <b>all_results</b> directory. You can configure that on your wish, just need to edit the [parse.sh](https://github.com/ratulSharker/Havec/blob/master/result_analyzer/parse.sh) file. Open it in your favorite editor, search for <b>all_results</b> and rename it as your choice. Another thing to share that, we compile the java program (generate .class file) everytime we call the [parse.sh](https://github.com/ratulSharker/Havec/blob/master/result_analyzer/parse.sh) which is completely unnecessary, but we need that because that script is from our [result_analyzer's](https://github.com/ratulSharker/Havec/tree/master/result_analyzer) devlopment cycle. Each time we made a change in the parser program we need to compile to get a glance in the output. 
