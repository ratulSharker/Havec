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
