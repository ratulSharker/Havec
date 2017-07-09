#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <ios>
#include <cstddef> 

using namespace std;

/**
 * forward declaration of the khmerInfo5Byte & khmerInfo6Byte //newly changes
**/


////////////////////////////////////////////////
// NECESSARY CONSTANTS
///////////////////////////////////////////////
#define LIST 0
#define MEM_BLOCK 1
#define MEM_BLOCK_FOUND 2
#define ERROR 3
#define FOUND 4
#define NOT_FOUND 5
#define LIST_FOUND 6
#define N_FOUND_IN_NEUCLEOTIDE 7	//newly added in purpose of detecting N in the read
#define NEUCLEOTIDE_CHAIN_IS_OK 8	//newly added in purpose of detecting N in the read


//special -- defined value
//#define LAST_HASH_FUNC 3
unsigned int LAST_HASH_FUNC;
unsigned char two_bit_repr[24];
unsigned int neucleotide_status;	//newly added in purpose of detecting N in the read


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
void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


bool is_prime(unsigned long long n)
{
 //checks if a number is prime

   if (n < 2)
      return false;
   if (n == 2)
	   return true;
   if (n % 2 == 0)
      return false;
   for (unsigned long long x =  3 ; x < sqrt((double)n)+1; x+= 2)
   {
      if ( n % x == 0)
         return false;
   }
   return true;
}

unsigned long long get_a_prime_above_x(unsigned long long x)
{

  // steps backward until n primes (other than 2) have been
//   found that are smaller than x.
	  if(is_prime(x)){
		return x;
	  }
   unsigned long long i = x+1;
   if (i % 2 == 0)
      i += 1;
   while ( i > 0)
   {
      if (is_prime(i))
		  return i;
      i += 2;
   }
   return x;
}



/*
* some necessary structures & some of their quantity variables
 */
unsigned long long num_of_vector;
struct classcomp {
  bool operator() (unsigned long long lhs, unsigned long long rhs)
  {return lhs<rhs;}
};



//////////////////////////////////////////////
// NECESSARY FUNCTIONALITY
//////////////////////////////////////////////
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
void currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    cout<<std::endl<<"current time ::"<<buf<<endl;
}

/*
 * this function return the highest
 * unsigned long long number ever can be produced.
 * runtime O(sizeof(unsigned long long))
 */
unsigned long long highestULongLongNumb(){

	unsigned long long h_val = 0;
	int size = sizeof(unsigned long long) * 8; //into bit
	for(int i=0; i<size ; i++){
		h_val = (h_val << 1) | 1;
	}

	return h_val;
}

/*
 * populate two_bit_repr hash mapping
 */
void populateTwoBitReprHashMap(){
												//	A 	T 	G 	C
	/*two_bit_repr[0] 	= 0b 0001 1011;//0->hash 		00 	01 	10 	11
	two_bit_repr[1] 	= 0b0001 1110;//1->hash 
	two_bit_repr[2] 	= 0b0010 0111;//2->hash 
	two_bit_repr[3] 	= 0b0010 1101;//3->hash 
	two_bit_repr[4] 	= 0b0011 0110;//4->hash 
	two_bit_repr[5] 	= 0b0011 1001;//5->hash 
	two_bit_repr[6] 	= 0b0100 1011;//6->hash 
	two_bit_repr[7] 	= 0b0100 1110;//7->hash 
	two_bit_repr[8] 	= 0b0110 0011;//8->hash */
	
	two_bit_repr[0] 	= 0X1B;//0->hash 		00 	01 	10 	11
	two_bit_repr[1] 	= 0XE4;//1->hash 
	two_bit_repr[2] 	= 0X4E;//2->hash 
	two_bit_repr[3] 	= 0XB1;//3->hash 
	two_bit_repr[4] 	= 0X2D;//4->hash 
	two_bit_repr[5] 	= 0XC9;//5->hash 
	two_bit_repr[6] 	= 0X9C;//6->hash 
	two_bit_repr[7] 	= 0X78;//7->hash
	
	/*
	two_bit_repr[8] 	= 0X63;//8->hash 
	two_bit_repr[9] 	= 0b0110 1100;//9->hash 
	two_bit_repr[10] 	= 0b0111 0010;//10->hash 
	two_bit_repr[11] 	= 0b0111 1000;//11->hash 
	two_bit_repr[12] 	= 0b10010011;//12->hash 
	two_bit_repr[13] 	= 0b10011100;//13->hash 
	two_bit_repr[14] 	= 0b10000111;//14->hash 
	two_bit_repr[15] 	= 0b10001101;//15->hash 
	two_bit_repr[16] 	= 0b10110100;//16->hash 
	two_bit_repr[17] 	= 0b10110001;//17->hash 
	two_bit_repr[18] 	= 0b11001001;//18->hash 
	two_bit_repr[19] 	= 0b11000110;//19->hash 
	two_bit_repr[20] 	= 0b11100001;//20->hash 
	two_bit_repr[21] 	= 0b11100100;//21->hash 
	two_bit_repr[22] 	= 0b11010010;//22->hash 
	two_bit_repr[23] 	= 0b11011000;//23->hash */
}

/*
 * if you give the full khmer, it will return the 64bit hashval. runtime O(size of khmer)
 */
unsigned long long fullHasher(std::string khmer,unsigned int whichHashFunc){

	unsigned char map_byte = two_bit_repr[whichHashFunc]; // A T G C
	unsigned long long hashed_val = 0;
	
	//decide the hash values
	unsigned char hashA = ((map_byte & 0XC0)>>6),
				  hashT = ((map_byte & 0X30)>>4),
				  hashG = ((map_byte & 0X0C)>>2),
				  hashC = ((map_byte & 0X03));
	
	neucleotide_status = NEUCLEOTIDE_CHAIN_IS_OK;//at first assume every neucleotide is ok
	
	for(unsigned int i=0;i < khmer.length() &&
						(neucleotide_status != N_FOUND_IN_NEUCLEOTIDE);i++){
		//iterate through each & every char
		switch(toupper(khmer[i])){
			case 'A': //A = whichHashFunc % 4
				hashed_val = (hashed_val << 2) | hashA;
				break;

			case 'T'://T = (whichHashFunc + 1) % 4
				hashed_val = (hashed_val << 2) | hashT;
				break;

			case 'G'://G = (whichHashFunc + 2) % 4
				hashed_val = (hashed_val << 2) | hashG;
				break;

			case 'C'://C = (whichHashFunc + 3) % 4
				hashed_val = (hashed_val << 2) | hashC;
				break;
			
			default://N found no need to compute further
				neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
				return 0; //now check at the call side of full hasher
				break;
				
		}
	}
	
	return hashed_val;
}


/*
 * if you give the hashed value of previous khmer & the next neucleotide
 * it will return the 64bit hash value of the current khmer.
 * runtime O(1)
 */
unsigned long long partialHasher(unsigned long long hashVal, char lastNeucleotide, unsigned short int kSize,unsigned int whichHashFunc){
	
	//security checking -- can be stopped when releasing
	if(whichHashFunc > LAST_HASH_FUNC){ //|| whichHashFunc < 0){ // un-necessay checking is removed because whichHashFunc is unsigned which value would never be negative, caught by osx g++.
		std::cout<<"invalid hash function requested :: "<<whichHashFunc<<std::endl;
		std::cout<<"Exiting..."<<std::endl;
		exit(0);
	}
	
	unsigned char map_byte = two_bit_repr[whichHashFunc]; // A T G C
	//decide the hash values
	unsigned char hashA = ((map_byte & 0XC0)>>6),
				  hashT = ((map_byte & 0X30)>>4),
				  hashG = ((map_byte & 0X0C)>>2),
				  hashC = ((map_byte & 0X03));
	//unsigned int hByte = whichHashFunc;
	unsigned int hByte;
	
	neucleotide_status = NEUCLEOTIDE_CHAIN_IS_OK;//at first assume every neucleotide is ok
	
	switch(toupper(lastNeucleotide)){
				case 'A': //A = whichHashFunc % 4
					hByte = hashA;
					break;

				case 'T'://T (whichHashFunc + 1)  % 4
					hByte = hashT;
					break;

				case 'G'://G (whichHashFunc + 2)  % 4
					hByte = hashG;
					break;

				case 'C'://C (whichHashFunc + 3)  % 4
					hByte = hashC;
					break;
				
				default://N found no need to compute further
					neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
					return 0;
					
					break;
					
	}

	hashVal = (hashVal << 2) | hByte; //actual hashing
	
	//if the khmer size is less than 32 bit -> then the hash bit at the index > 2k is invalid -- clean them man :P
	unsigned long long bitmusk = 0xFFFFFFFFFFFFFFFF; //here assumed that unsigned long long is 64 bit long
	bitmusk = bitmusk >> ((sizeof(unsigned long long) * 8 )- (2 * kSize));

	hashVal = hashVal & bitmusk;

	return hashVal ;
}




/*****************************************/
//check rev_hash for the correct working
/*****************************************/
string _revhash(unsigned long long hashed,unsigned short hashFunc, unsigned int kSize){
	string ans="";
	for(int i=0;i< kSize ;i++){
		ans = rev_two_bit_repr((hashed) & 3,(hashFunc)) + ans;
		hashed = hashed >> 2;
	}
	return ans;
}

/*
char* __revhash(unsigned long long hashed,unsigned short hashFunc, unsigned int kSize){
	string ans="";
	for(int i=0;i< kSize ;i++){
		ans += rev_two_bit_repr((hashed) & 3,(hashFunc));
		hashed = hashed >> 2;
	}
	return ans.c_str();
}
*/

////newly changed -- this following function is added newly
//void printWholeBlockArrayAndVectorStructure(void* blockArr, bool is5Byte,unsigned long long hashSize, unsigned int kSize,vector<struct vector_trial_5_byte> *mapPointer5Byte, vector<struct vector_trial_6_byte> *mapPointer6Byte)
//{	

	//char neighbour_musk = 0xF0;
	
	//FILE *fp, *fp2;
	//fp	=fopen("hashtable.txt", "w");
	//fp2 =fopen("vector_structure.txt", "w"); 
	
	//// if its 5 byte structure -- cast it to 5 byte
	//if(is5Byte)
	//{
		///**
		//* dumping the HashTable
		//**/
		////its a 5 byte structure
		//unsigned long long hashValue 	= 0;
		//unsigned long long qutioned 	= 0;
		//unsigned short hashFunc			= 0;
		//char 		   neighbour		= 0;
		
		//khmerInfo5Byte *blockArray5 = (khmerInfo5Byte*) blockArr;
		//for(unsigned long long index = 0; index < hashSize;index++)
		//{
			////now check specified index is occupied or not
			//if((blockArray5[index].data[4] & neighbour_musk) != 0){
				////occupuying here
				
				////cleaning the syringe to push injection ;) beware of aids :P
				//neighbour = hashFunc = qutioned = hashValue = 0;
	
				////forming the qutioned
				//qutioned = blockArray5[index].data[4] & 1; // only the last bit is the part of the qutioned
				//qutioned = (qutioned << 8) | blockArray5[index].data[3];
				//qutioned = (qutioned << 8) | blockArray5[index].data[2];
				//qutioned = (qutioned << 8) | blockArray5[index].data[1];

				////forming the hash value
				//hashValue = (hashSize * qutioned) + index;
				//hashFunc = (blockArray5[index].data[4] & 0x0E ) >> 1; //mask out the hash function
				
				////forming the neighbour
				//switch(blockArray5[index].data[4] & 0xF0)//making out the first 4 bit
				//{
					//case 0x80://found A
						//neighbour = 'A';
					//break;
					
					//case 0x40://found T
						//neighbour = 'T';
					//break;
					
					//case 0x20://found C
						//neighbour = 'C';
					//break;
					
					//case 0x10://found G
						//neighbour = 'G';
					//break;
				//}
				
				////prints the khmer
				//fprintf(fp, "%s-%c\n", _revhash(hashValue,hashFunc,kSize).c_str(), neighbour);
			//}
		//}
		
		
		///**
		//* dumping the VectorStructure -- using the same variable as dumping the hashtable :)
		//**/
		///*
		//for(unsigned long long i=0;i<num_of_vector;i++)//number of vector is declared in the common.cpp -- so no need to pass around
		//{
			//for(unsigned long long j=0;j<mapPointer5Byte[i].size();j++)
			//{
				
				
				//for(unsigned long long k=0;k < mapPointer5Byte[i][j].vect.size() ; k++)//newly changes in the loop
				//{
					////now take steps to print 
					////now check specified index is occupied or not
					//if((mapPointer5Byte[i][j].vect[k].data[4] & neighbour_musk) != 0){
						////occupuying here
						
						////cleaning the syringe to push injection ;) beware of aids :P
						//neighbour = hashFunc = qutioned = hashValue = 0;
			
						////forming the qutioned
						//qutioned = mapPointer5Byte[i][j].vect[k].data[4] & 1; // only the last bit is the part of the qutioned
						//qutioned = (qutioned << 8) | mapPointer5Byte[i][j].vect[k].data[3];
						//qutioned = (qutioned << 8) | mapPointer5Byte[i][j].vect[k].data[2];
						//qutioned = (qutioned << 8) | mapPointer5Byte[i][j].vect[k].data[1];

						////forming the hash value
						//hashValue = (hashSize * qutioned) + index;
						//hashFunc = (mapPointer5Byte[i][j].vect[k].data[4] & 0x0E ) >> 1; //mask out the hash function
						
						////forming the neighbour
						//switch(mapPointer5Byte[i][j].vect[k].data[4] & 0xF0)//making out the first 4 bit
						//{
							//case 0x80://found A
								//neighbour = 'A';
							//break;
							
							//case 0x40://found T
								//neighbour = 'T';
							//break;
							
							//case 0x20://found C
								//neighbour = 'C';
							//break;
							
							//case 0x10://found G
								//neighbour = 'G';
							//break;
						//}
						
						////prints the khmer
						//fprintf(fp, "%s-%c\n", _revhash(hashValue,hashFunc,kSize), neighbour);
					//}
				//}
			//}
		//}*/

	//}
	//else
	//{
		///**
		//* dumping the HashTable
		//**/
		////its a 6 byte structure
		//unsigned long long hashValue 	= 0;
		//unsigned long long qutioned 	= 0;
		//unsigned short hashFunc			= 0;
		//char 		   neighbour		= 0;
		
		//khmerInfo6Byte *blockArray6 = (khmerInfo6Byte*) blockArr;
		//for(unsigned long long index = 0; index < hashSize;index++)
		//{
			////now check specified index is occupied or not
			//if((blockArray6[index].data[4] & neighbour_musk) != 0){
				////occupuying here

				////forming the qutioned
				//qutioned = blockArray6[index].data[4] & 1; // only the last bit is the part of the qutioned
				//qutioned = (qutioned << 8) | blockArray6[index].data[3];
				//qutioned = (qutioned << 8) | blockArray6[index].data[2];
				//qutioned = (qutioned << 8) | blockArray6[index].data[1];

				////forming the hash value
				//hashValue = (hashSize * qutioned) + index;
				//hashFunc = (blockArray6[index].data[4] & 0x0E ) >> 1; //mask out the hash function

				////forming the neighbour
				//switch(blockArray6[index].data[4] & 0xF0)//making out the first 4 bit
				//{
					//case 0x80://found A
						//neighbour = 'A';
					//break;
					
					//case 0x40://found T
						//neighbour = 'T';
					//break;
					
					//case 0x20://found C
						//neighbour = 'C';
					//break;
					
					//case 0x10://found G
						//neighbour = 'G';
					//break;
				//}
				
				////prints the khmer with the count information
				//fprintf(fp, "%s-%c-%d\n", _revhash(hashValue,hashFunc,kSize).c_str(),neighbour, blockArray6[index].count);
			//}
		//}
		
		///**
		//* dumping the Vector Strcuture -- using the same variable as dumping the hashtable :)
		//**/
		
		///*
		//for(unsigned long long i=0;i<num_of_vector;i++)//number of vector is declared in the common.cpp -- so no need to pass around
		//{
			//for(unsigned long long j=0;j<mapPointer6Byte[i].size();j++)
			//{
				
				
				//for(unsigned long long k=0;k < mapPointer6Byte[i][j].vect.size() ; k++)//newly changes in the loop
				//{
					////now take steps to print 
					////now check specified index is occupied or not
					//if((mapPointer6Byte[i][j].vect[k].data[4] & neighbour_musk) != 0){
						////occupuying here
						
						////cleaning the syringe to push injection ;) beware of aids :P
						//neighbour = hashFunc = qutioned = hashValue = 0;
			
						////forming the qutioned
						//qutioned = mapPointer6Byte[i][j].vect[k].data[4] & 1; // only the last bit is the part of the qutioned
						//qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[3];
						//qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[2];
						//qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[1];

						////forming the hash value
						//hashValue = (hashSize * qutioned) + index;
						//hashFunc = (mapPointer6Byte[i][j].vect[k].data[4] & 0x0E ) >> 1; //mask out the hash function
						
						////forming the neighbour
						//switch(mapPointer6Byte[i][j].vect[k].data[4] & 0xF0)//making out the first 4 bit
						//{
							//case 0x80://found A
								//neighbour = 'A';
							//break;
							
							//case 0x40://found T
								//neighbour = 'T';
							//break;
							
							//case 0x20://found C
								//neighbour = 'C';
							//break;
							
							//case 0x10://found G
								//neighbour = 'G';
							//break;
						//}
						
						////prints the khmer with the count information
						//fprintf(fp, "%s-%c-%d\n", _revhash(hashValue,hashFunc,kSize),neighbour, mapPointer6Byte[i][j].vect[k].count);
					//}
				//}
			//}
		//}*/
	//}
	
	//fclose(fp);
	//fclose(fp2);
//}

