/* necessary includes */
#include <iostream>
#include <CUnit/Basic.h>
#include "../src/common.h"

using namespace std;

void test_two_bit_repr()
{
	CU_ASSERT( two_bit_repr[0] == 0X1B );
	CU_ASSERT( two_bit_repr[1] == 0XE4 );
	CU_ASSERT( two_bit_repr[2] == 0X4E );
	CU_ASSERT( two_bit_repr[3] == 0XB1 );
	CU_ASSERT( two_bit_repr[4] == 0X2D );
	CU_ASSERT( two_bit_repr[5] == 0XC9 );
	CU_ASSERT( two_bit_repr[6] == 0X9C );
	CU_ASSERT( two_bit_repr[7] == 0X78 );
}

void test_rev_two_bit_repr()
{
	// test the macro
	// rev_two_bit_repr

	CU_ASSERT( rev_two_bit_repr(0, 0) == 'A' );
	CU_ASSERT( rev_two_bit_repr(1, 0) == 'T' );
	CU_ASSERT( rev_two_bit_repr(2, 0) == 'G' );
	CU_ASSERT( rev_two_bit_repr(3, 0) == 'C' );

	CU_ASSERT( rev_two_bit_repr(0, 1) == 'C' );
	CU_ASSERT( rev_two_bit_repr(1, 1) == 'A' );
	CU_ASSERT( rev_two_bit_repr(2, 1) == 'T' );
	CU_ASSERT( rev_two_bit_repr(3, 1) == 'G' );

	CU_ASSERT( rev_two_bit_repr(0, 2) == 'G' );
	CU_ASSERT( rev_two_bit_repr(1, 2) == 'C' );
	CU_ASSERT( rev_two_bit_repr(2, 2) == 'A' );
	CU_ASSERT( rev_two_bit_repr(3, 2) == 'T' );

	CU_ASSERT( rev_two_bit_repr(0, 3) == 'T' );
	CU_ASSERT( rev_two_bit_repr(1, 3) == 'G' );
	CU_ASSERT( rev_two_bit_repr(2, 3) == 'C' );
	CU_ASSERT( rev_two_bit_repr(3, 3) == 'A' );

	CU_ASSERT( rev_two_bit_repr(3321, 3) == 'Z' );
}

void test_process_mem_usage()
{
	// test the process mem
	// memory usage
}

void test_is_prime()
{
	// test is_prime function
	CU_ASSERT( true == is_prime(7));
	CU_ASSERT( true == is_prime(4409));
	CU_ASSERT( false == is_prime(7827));
	CU_ASSERT( false == is_prime(7917))
}

void test_get_a_prime_above_x()
{
	// test get_a_prime_above_x
	CU_ASSERT( get_a_prime_above_x(5347) == 5347 );
	CU_ASSERT( get_a_prime_above_x(6360) < 6362 );
	CU_ASSERT( get_a_prime_above_x(7488) == 7489 );
	CU_ASSERT( get_a_prime_above_x(7907) != 7921 );
}

void test_currentDateTime()
{
	// test currentDateTime
}


void test_fullHasher()
{
	CU_ASSERT(fullHasher("ATGTC", 1) == 920);
	CU_ASSERT(fullHasher("AGTCCCCCGTAAACCTG", 3) == 9484692060);
	CU_ASSERT(fullHasher("TTAGCGGTCACACTTGCG", 7) == 66080281570);
	CU_ASSERT(fullHasher("AGTCCCCCACACTTGCG", 0) == 2684145006);
	CU_ASSERT(fullHasher("TTAGCGGTCTAAACCTG", 6) ==6122924551);
}

void test_partialHasher()
{
	CU_ASSERT(fullHasher("AGTCCCCCACACTTGCGC", 0) 	== partialHasher(fullHasher("AGTCCCCCACACTTGCG", 0), 'C', 32, 0));
	CU_ASSERT(fullHasher("AGTCCCTGCGT", 0) 			== partialHasher(fullHasher("AGTCCCTGCG", 0), 'T', 32, 0));
	CU_ASSERT(fullHasher("AGTCCCCCACACT", 0) 		== partialHasher(fullHasher("AGTCCCCCACAC", 0), 'T', 32, 0));
	CU_ASSERT(fullHasher("AGTCCCCCACACTGCA", 0) 	== partialHasher(fullHasher("AGTCCCCCACACTGC", 0), 'A', 32, 0));
	CU_ASSERT(fullHasher("AGTCCCCCACACTTG", 0) 		== partialHasher(fullHasher("AGTCCCCCACACTT", 0), 'G', 32, 0));
}

void test_string_revhash()
{
	
}


void createDirectory()
{
	
}

int init_suite(void)
{
	// populate the two bit representation
	// then test
	populateTwoBitReprHashMap();

	return 0;
}

int clean_suite(void)
{
	return 0;
}

/* The main function for setting up and running the tests.
   Returns a CUE_SUCCESS on successful running, another
   CUnit error code on failure.
*/
int main(int argc, char **argv)
{

	// running partial hasher on some test cases
	// partialHasher(unsigned long long hashVal,
	// 							 char lastNeucleotide,
	// 							 unsigned short int kSize,
	// 							 unsigned int whichHashFunc)

	// populateTwoBitReprHashMap();


	// cout<<"AGTCCCCCACACTTGCG : " <<partialHasher(2684145006, 'C', 32, 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGC : " << partialHasher(10736580027, 'G', 32, 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGCG : " << partialHasher(42946320110, 'T', 32, 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGCGT : " << partialHasher(171785280441, 'A', 32, 0)<<endl;

	// cout<<"AGTCCCCCACACTTGCGC : " <<	fullHasher("AGTCCCCCACACTTGCGC", 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGCG : " << 	fullHasher("AGTCCCCCACACTTGCGCG", 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGCGT : " << 	fullHasher("AGTCCCCCACACTTGCGCGT", 0)<<endl;
	// cout<<"AGTCCCCCACACTTGCGCGTA : " << fullHasher("AGTCCCCCACACTTGCGCGTA", 0)<<endl;

	// return 0;



	CU_pSuite pSuite = NULL;

	// initialize the CUnit test registry
	if(CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}


	// add a suite to the registry
	pSuite = CU_add_suite("common_h_test_suite", init_suite, clean_suite);
	if(NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	// add tests to the suite
	if (
		(NULL == CU_add_test(pSuite, "test of is_prime()", test_is_prime)) 
		|| (NULL == CU_add_test(pSuite, "test of get_a_prime_above_x()", test_get_a_prime_above_x))
	   	|| (NULL == CU_add_test(pSuite, "test of rev_two_bit_repr()", test_rev_two_bit_repr))
	   	|| (NULL == CU_add_test(pSuite, "test of two_bit_repr()", test_two_bit_repr))
	  	|| (NULL == CU_add_test(pSuite, "test of fullHasher()", test_fullHasher))
	  	|| (NULL == CU_add_test(pSuite, "test of partialHasher()", test_partialHasher))
	   	)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
