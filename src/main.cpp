#include "common.cpp"
#include "vector_with_rehash_8_hash_with_N_detection_5_bytes.cpp"
#include "vector_with_rehash_8_hash_with_N_detection_6_bytes.cpp"











/*
* this is the real main function that is going to be called 
* ultimately :)
*/
int main(int argc,char **argv){
	
	bool cutOff = false;
	
	/*
	* check for the availability of the cut-off value
	*/
	if(strcmp("C",argv[4]) == 0 ||  strcmp("c",argv[4]) == 0 ){
		cutOff = true;
	}
	else{
		cutOff = false;
	}

	/*
	 * create the output directory
	 */
	 createDirectory(OUTPUT_DIRECTORY_NAME);

	//now call the 5 byte or the 6 byte functionalities :)
	if(cutOff){
		//call 6 byte code -- if valid values are given in the cutOffSection
		main6Byte(argc,argv);
	}else{
		//cutOffValue 1 assumed -- so call the 5 byte functionality
		main5Byte(argc,argv);
	}
	
	return 0;
}
