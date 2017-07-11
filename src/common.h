#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


#ifndef COMMON_H
#define COMMON_H


//////////////////////////////////////////////
//NECESSARY CONSTANTS
/////////////////////////////////////////////
#define LIST 0
#define MEM_BLOCK 1
#define MEM_BLOCK_FOUND 2
#define ERROR 3
#define FOUND 4
#define NOT_FOUND 5
#define LIST_FOUND 6
#define N_FOUND_IN_NEUCLEOTIDE 7	//newly added in purpose of detecting N in the read
#define NEUCLEOTIDE_CHAIN_IS_OK 8	//newly added in purpose of detecting N in the read

#define OUTPUT_DIRECTORY_NAME	"output/"

// variables
extern unsigned int LAST_HASH_FUNC;
extern unsigned char two_bit_repr[24];
extern unsigned int neucleotide_status;	//newly added in purpose of detecting N in the read

/*
* some necessary structures & some of their quantity variables
 */
extern unsigned long long num_of_vector;

struct classcomp {
  bool operator() (unsigned long long lhs, unsigned long long rhs)
  {return lhs<rhs;}
};

#define rev_two_bit_repr(n,i) ( (i)==0 && (n)==0 ? 'A' : \
							(i)==0 && (n)==1 ? 'T' : \
							(i)==0 && (n)==2 ? 'G' : \
							(i)==0 && (n)==3 ? 'C' : \
							(i)==1 && (n)==0 ? 'C' : \
							(i)==1 && (n)==1 ? 'A' : \
							(i)==1 && (n)==2 ? 'T' : \
							(i)==1 && (n)==3 ? 'G' : \
							(i)==2 && (n)==0 ? 'G' : \
							(i)==2 && (n)==1 ? 'C' : \
							(i)==2 && (n)==2 ? 'A' : \
							(i)==2 && (n)==3 ? 'T' : \
							(i)==3 && (n)==0 ? 'T' : \
							(i)==3 && (n)==1 ? 'G' : \
							(i)==3 && (n)==2 ? 'C' : \
							(i)==3 && (n)==3 ? 'A' : 'Z' )


/*
 * some common functionality :)
 */							
void process_mem_usage(double& vm_usage, double& resident_set);

bool is_prime(unsigned long long n);
unsigned long long get_a_prime_above_x(unsigned long long x);

//////////////////////////////////////////////
// NECESSARY FUNCTIONALITY
//////////////////////////////////////////////
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
void currentDateTime();

/*
 * this function return the highest
 * unsigned long long number ever can be produced.
 * runtime O(sizeof(unsigned long long))
 */
unsigned long long highestULongLongNumb();

/*
 * populate two_bit_repr hash mapping
 */
void populateTwoBitReprHashMap();

/*
 * if you give the full khmer, it will return the 64bit hashval. runtime O(size of khmer)
 */
unsigned long long fullHasher(	string khmer,
								unsigned int whichHashFunc);


/*
 * if you give the hashed value of previous khmer & the next neucleotide
 * it will return the 64bit hash value of the current khmer.
 * runtime O(1)
 */
unsigned long long partialHasher(unsigned long long hashVal,
								 char lastNeucleotide,
								 unsigned short int kSize,
								 unsigned int whichHashFunc);

/*****************************************/
//check rev_hash for the correct working
/*****************************************/
string _revhash(unsigned long long hashed,
				unsigned short hashFunc,
				unsigned int kSize);


/********************************************/
// create a directory with the name provided
/********************************************/
bool createDirectory(const char* directoryName);

#endif
