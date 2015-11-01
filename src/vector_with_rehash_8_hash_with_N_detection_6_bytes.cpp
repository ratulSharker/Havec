
unsigned long long number_of_khmer_not_listed_in_memblock_count_variable = 0;
unsigned long long number_of_khmer_not_listed_in_vector_count_variable = 0;


////////////////////////////////////////////////
// NECESSARY STRUCTURES
///////////////////////////////////////////////

struct khmerInfo6Byte{
	//space for only qutioned + neighbour
	unsigned char data[5];//12 bit for qutioned + 4 bit for neighbour
	unsigned char count = 0;
	/*
	 * data[4] (NNNN WWWQ) N for neighbour & W for which hash & Q for qutioned
	 * data[3] (QQQQ QQQQ) Q for the qutioned
	 * data[2] (QQQQ QQQQ) Q for the qutioned
	 * data[1] (QQQQ QQQQ) Q for the qutioned
	 * data[0] (QQQQ QQQQ) Q for the qutioned
	 * count is for the number of times that khmer appeared
	 */
};


/*
 *	vector trial structure for six byte
 */
struct vector_trial_6_byte{
	unsigned long long index_val;
	vector<struct khmerInfo6Byte> vect;
};


/*
* number of vector required is passed to common.cpp
*/

vector<struct vector_trial_6_byte> *mapPointer6Byte;



/*
	for each type of vector 5/6 byte this function is'nt common -- thus why not moved
*/
void show_vect_status_6_byte()
{
	int total_vect=0,total_row=0,max_size=0;
	
	cout<<"calculating vector info..."<<endl;
	
	for(int i=0;i<num_of_vector;i++)
	{
		for(int j=0;j<mapPointer6Byte[i].size();j++)
		{
			//cout<<"got it "<<mapPointer6Byte[i][j]<<" i value "<<i<<" j value "<<j<<endl;
			total_vect+=mapPointer6Byte[i][j].vect.size();
			if(mapPointer6Byte[i][j].vect.size()>max_size)
			{
				max_size=mapPointer6Byte[i][j].vect.size();
			}
		}
		total_row+=mapPointer6Byte[i].size();
	}
	cout<<"vector info: :)"<<endl;
	cout<<" Maximum vector size for one hashtable index "<<max_size<<endl;
}

/*
* extra function that is not present in the 5 byte structure
*/
void show_count_status(struct khmerInfo6Byte * memBlock,long int size){
	
	long int TotalNumberOfKhmer_in_memblock = 0;
	long int TotalNumberofkhmer_in_vector = 0;
	unsigned long Maximum_count_in_memBlock = 0;
	unsigned long Maximum_count_in_vector = 0;	

	cout<<"calculating count info..."<<endl;
	
	//counting in the map table
	for(unsigned long long i=0;i<size;i++){
		//traversing all the element in the map
		TotalNumberOfKhmer_in_memblock += memBlock[i].count;
		
		if(memBlock[i].count > Maximum_count_in_memBlock){
			Maximum_count_in_memBlock = memBlock[i].count;
		}
	}
	
	//cout<<"counting memblock status is completed ....."<<endl<<"counting vector status ... "<<endl;
	

	//counting in the vector table
	for(unsigned long long i=0;i<num_of_vector;i++)
	{
		for(int j=0;j<mapPointer6Byte[i].size();j++)
		{

			for(int p=0;p<mapPointer6Byte[i][j].vect.size();p++)
			{
				TotalNumberofkhmer_in_vector+=mapPointer6Byte[i][j].vect[p].count;
				
				if(memBlock[i].count > Maximum_count_in_vector){
					Maximum_count_in_vector = mapPointer6Byte[i][j].vect[p].count;
				}

			}
		}
		
	}
	
	//cout<<"counting vcector status is complete ..."<<endl;
	
	cout<<"count status  :)"<<endl;
/*	cout<<"number of khmer listed in memblock count :: "<<TotalNumberOfKhmer_in_memblock<<endl;
	cout<<"number of khmer not listed in memblock count ::" <<number_of_khmer_not_listed_in_memblock_count_variable<<endl;
	cout<<"number of khmer listed in vector count :: "<<TotalNumberofkhmer_in_vector<<endl;
	cout<<"number of khmer not listed in vector count :: "<<number_of_khmer_not_listed_in_vector_count_variable<<endl;
*/
	cout<<"max count value in memblock "<<Maximum_count_in_memBlock<<endl;
	cout<<"max count value in vector "<<Maximum_count_in_vector<<endl;
}



/* for both 5byte & 6byte this function is not common -- thus why not moved to the common.cpp

 * put the given khmer into the memblock if the space is not occupied
 * then occupy & update its neighbour
 * if the given khmer is present then simply update its neighbour
 *
 * RETURNS LIST if the khmer is put in the list
 * 			MEM_BLOCK if the khmer is put in the mem_block
 * 			ERROR if any error occur in the function
 */
int storeKhmerInMemBlock6Byte(string sample,struct khmerInfo6Byte * memBlock,unsigned long long blockSize, unsigned long long khmer_h, char next_neucleotide, unsigned int whichHashFunc){

	unsigned long long index;
	unsigned long long qutioned; //this value must be under 12bit
	char neighbour_musk = 0xF0;
	char *p;

	//find the index value in the memBlock
	index = khmer_h % blockSize;
	
	qutioned = khmer_h / blockSize;//this value must be under 10bit

	//std::cout<<"data :: "<<(int)memBlock[index].data[1]<<std::endl;
	//cout<<"sample khmer "<<sample<<" kmer_h "<<khmer_h<<" hash func no "<<whichHashFunc<<endl;

	//now check the specified index is neighbourless or not (which is equivalent to -> is the space occupied)
	if((memBlock[index].data[4] & neighbour_musk) == 0){
		//no one is occupuying here put khmer_h here

		//putting the qutioned
		p = (char*) &qutioned;
		memBlock[index].data[0] = *p;
		p++;
		memBlock[index].data[1] = *p;
		p++;
		memBlock[index].data[2] = *p;
		p++;
		memBlock[index].data[3] = *p;
		p++;
		memBlock[index].data[4] = 0x01 & (*p);

		//putting "which hash value is stored"
		memBlock[index].data[4] = memBlock[index].data[4] | (whichHashFunc << 1);
		
		/*memBlock[index].data[0] = qutioned & 0xFF;
		memBlock[index].data[1] = (qutioned >> 8) & 0x0F;
		*/

		//std::cout<<"putting in the block array"<<std::endl;

		switch(toupper(next_neucleotide)){//putting the neighbour
		case 'A'://1000
			memBlock[index].data[4] = memBlock[index].data[4] | 0x80;
			break;
		case 'T'://0100
			memBlock[index].data[4] = memBlock[index].data[4] | 0x40;
			break;
		case 'C'://0010
			memBlock[index].data[4] = memBlock[index].data[4] | 0x20;
			break;
		case 'G'://0001
			memBlock[index].data[4] = memBlock[index].data[4] | 0x10;
			break;
		}
		//std::cout<<"index "<<index<<" quotioned :: "<< qutioned<< " data[0]=" <<(int)memBlock[index].data[0]<<" data[1]="<<(int)memBlock[index].data[1]<<std::endl;

		//increment the count value
		memBlock[index].count = 1; //--6 byte final update
		
		return MEM_BLOCK; //khmer putted in the mem_block so return mem_block
	}
	else if(((memBlock[index].data[4] >> 1) & 0x07) == whichHashFunc && //hash value matched
			(memBlock[index].data[0] == (qutioned & 0xFF)) &&
			(memBlock[index].data[1] == (qutioned & 0xFF00)>>8) &&
			(memBlock[index].data[2] == (qutioned & 0xFF0000)>>16) &&
			(memBlock[index].data[3] == (qutioned & 0xFF000000)>>24) &&
			((memBlock[index].data[4] & 0x01) == ((qutioned & 0x0100000000) >> 32))){
		//someone is sitting on the memBlock index & who is same as the khmer

		//now just update its neighbour
		//std::cout<<"now just update its neighbour"<<std::endl;
		switch(toupper(next_neucleotide)){//putting the neighbour
		case 'A'://1000
			memBlock[index].data[4] = memBlock[index].data[4] | 0x80;
			break;
		case 'T'://0100
			memBlock[index].data[4] = memBlock[index].data[4] | 0x40;
			break;
		case 'C'://0010
			memBlock[index].data[4] = memBlock[index].data[4] | 0x20;
			break;
		case 'G'://0001
			memBlock[index].data[4] = memBlock[index].data[4] | 0x10;
			break;
		}
		
		//counting upto 255 -- 6byte final
		if(memBlock[index].count<255){
			memBlock[index].count++;
		}
		/*//helps to compute how many khmer is not being listed
		else{
			number_of_khmer_not_listed_in_memblock_count_variable++;
		}
		*/
			
			
		return MEM_BLOCK_FOUND; //khmer putted in the mem_block so return mem_block -- simply update the neighbour table
	}
	else if(whichHashFunc == LAST_HASH_FUNC){
		//only for the last time put in the alternative mem
		//neighbour field is not blank -- someone already sitting there
		//and who sitting there is not same as the given khmer
		//and the set pointer is null so it must be initialized
		//std::cout<<"else reached index "<<index<<" quotioned::"<<qutioned<< " data[0]=" <<(int)memBlock[index].data[0]<<" data[1]="<<(int)memBlock[index].data[1]<<std::endl;
		//expand the set pointer & place the khmer in there
		vector<struct vector_trial_6_byte>::iterator it;
		int index_vect=0;
		int index_v=index%num_of_vector;

		for(it=mapPointer6Byte[index_v].begin();it!=mapPointer6Byte[index_v].end();it++)
		{
			if(it->index_val==index)
			{
				//cout<<"existing value found for index "<<index<<" for khmer "<<khmer_h<<endl;
				break;
			}
			index_vect++;
		}


		if(it == mapPointer6Byte[index_v].end()){
			struct vector_trial_6_byte temp_vect;
			temp_vect.index_val=index;
			//temp_vect.vect_pnt=pp;
			mapPointer6Byte[index_v].push_back(temp_vect);
			//cout<<"new value inserted for index "<<index<<" for khmer "<<khmer_h<<" stored in index "<<index_v<<endl;
		}

		struct khmerInfo6Byte kin;//making a demo
		kin.data[0] = kin.data[1] = kin.data[2] = kin.data[3] = kin.data[4] = 0; //cleaning the demo info
		
		//cout<<"New value inserted in list for memblock index "<<index<<" for khmer "<<khmer_h<<" stored in list in index "<<index_v<<" hash func no "<<whichHashFunc<<endl;
		
		switch(toupper(next_neucleotide)){//putting the neighbour
		case 'A'://1000
			kin.data[4] = kin.data[4] | 0x80;
			break;
		case 'T'://0100
			kin.data[4] = kin.data[4] | 0x40;
			break;
		case 'C'://0010
			kin.data[4] = kin.data[4] | 0x20;
			break;
		case 'G'://s
			kin.data[4] = kin.data[4] | 0x10;
			break;
		}

		//putting the hash function data in the newly made khmerInfo6Byte obj
		
		/*****************************************************/
		//problem occur -- limon
		/*****************************************************/
		bool isFound = false;
		for(std::vector<khmerInfo6Byte>::iterator pi = mapPointer6Byte[index_v][index_vect].vect.begin();
				pi != mapPointer6Byte[index_v][index_vect].vect.end() ; pi++)
		{
			if(
				((pi->data[4] >> 1) & 0x07) == whichHashFunc && //hash value matched
				(pi->data[0] == (qutioned & 0xFF)) &&
				(pi->data[1] == (qutioned & 0xFF00)>>8) &&
				(pi->data[2] == (qutioned & 0xFF0000)>>16) &&
				(pi->data[3] == (qutioned & 0xFF000000)>>24) &&
				((pi->data[4] & 0x01) == ((qutioned & 0x0100000000) >> 32))){
			//(pi->data[0] == (qutioned & 0xFF)) && ((pi->data[1] & 0x0F) == ((qutioned & 0x0F00) >> 8))){
				pi->data[4] = pi->data[4] | kin.data[4];
				
				/*
				 * searched khmer found in the list
				 * so simply increment the count variable
				*/
				if(pi->count < 255){
					pi->count++;
				}
				/*//helps to compute how many khmer is not being listed
				else{
					number_of_khmer_not_listed_in_vector_count_variable++;
				}
				*/
				
				
				isFound = true;
				//cout<<"isFound true"<<endl;
				break;
			}
		}
		if(isFound == false){
		
			//putting the qutioned
			p = (char*) &qutioned;
			kin.data[0] = *p;
			p++;
			kin.data[1] = *p;
			p++;
			kin.data[2] = *p;
			p++;
			kin.data[3] = *p;
			p++;
			kin.data[4] = 0x01 & (*p);
			
			//putting "which hash value is stored"
			kin.data[4] = kin.data[4] | (whichHashFunc << 1); //--optimization		

			/*
			 *  searched khmer not found in the vector
			 *  so set count to one
			 */
			kin.count = 1;
			
			
			(mapPointer6Byte[index_v])[index_vect].vect.push_back(kin);
			
			return LIST;//new entry in the list
		}

		return LIST_FOUND; //khmer found in the list so return LIST_FOUND
	}

	//none of the if-else block is reached so it caused error
	//this means that particular khmer couldn't be placed inside the memblock
	//it will basically return for hash func 0,1,2 -> for hash func 3 if it fails
	//it will be stored in the list/vector/sqlite
	return ERROR;
}

/* for both 5byte & 6byte this functionality isn't common -- thus why not moved to the common.cpp

 * populate the memBlock with the khmer inside the given fileName
 * {NB:: khmer size should be less than or equal 21}
 */
void populateBlock6Byte(struct khmerInfo6Byte * memBlock, unsigned long long blockSize, char *fileName,unsigned short int khmerSize){

	//opening the given file
	std::ifstream inputFile;
	inputFile.open(fileName,std::fstream::in);
	std::string buf,temp="";
	bool isFirstKhmer = true;
	unsigned long long * lastFoundKHashVal;
	lastFoundKHashVal=new unsigned long long[LAST_HASH_FUNC+1];
	
	bool n_found = false;//if n found in the read, then the read processing is left inactive 
						 // till the next new read sequence is found !!!
	
	unsigned long long consumedRead = 0;
	unsigned int lastHashFuncUsed = 0;//containe the value that is last used to store last khmer
	
	//for stat purpose use only:)
	unsigned long mem_block_occupied_stat = 0,mem_block_found_stat=0;
	unsigned long list_occupied_stat = 0,list_occupied_found=0;
	unsigned long mem_block_total_number_of_occupied = 0; //exact number of khmer stored in the memblock
	unsigned long vector_total_number_of_occupied = 0; //exact number of khmer stored in the vector 
	
	//ofstream myfile;
	//myfile.open("firstKmers.txt");
	//error log if can't be opened
	if(inputFile.fail() == true){
		/*
		 * if fail to load the file -- return
		 */
		std::cout<<"Fail to open the "<<fileName << std::endl;
		return;
	}


	//if successfull then log it !!!
	std::cout<<"Consuming... "<<fileName<<std::endl;

	//parsing the fasta file
	while(getline(inputFile,buf)){//reading a line

			if(buf[0] == '>'){
				//its a sequence name -- omit it !!! -- before omit clear the temp
				
				n_found = false; //new sequence found so i can't tell that exactly that, this new read sequence is bad or not -- time to consume
				
				temp = ""; //cleaning the old remaining khmer
				
				isFirstKhmer = true;
				consumedRead++;
				if(consumedRead%1000000==0){
					std::cout<<consumedRead/1000000<<" million read consumed..."<<std::endl;
					currentDateTime();
					/*show_vect_status_6_byte();
						std::cout<<"stat from consume function :: "<<std::endl
							<<"khmer in memBlock :: "<<mem_block_occupied_stat<<std::endl
							<<"khmer in list :: "<<list_occupied_stat<<std::endl<<std::endl;
							cout<<"*********************************"<<endl;
					*/	double vm, rss;
						process_mem_usage(vm, rss);
						cout << "Memory used (GB):  " << ((vm)/1024)/1024 <<endl;
				}
				//std::cout<<buf<<std::endl;
				continue;
			}
			else if(n_found == false){//if read sequence is not bad, consume it
									//if it is bad, then just omit consuming :) till a new read sequence found
									
				//here come the one sequnece of read
				buf = temp.append(buf);//no new read sequnence is found so the next line of read must be appended to the last
				unsigned int i;
				unsigned int bufSize = buf.size();//for optimization purpose only
				unsigned int loopTime = bufSize - khmerSize;//for optimization purpose only
				unsigned int addResult; //for optimization only

				if(isFirstKhmer == true){
					//its the first read line of the multiple read lines
					i = 1;
				}
				else{
					//its not the first line of the multiple read line
					i= 0;
				}

				//if it the first line then consume it explicitly by using fullHasher
				if(isFirstKhmer == true){
				
					if(bufSize < khmerSize){
						/*cout<<"**************************************************"<<endl;						
						cout<<"Problem detected !!!"<<endl;
						cout<<"read sequence size is less then the khmer size -- processing next read :("<<endl;
						cout<<"seq :: "<<buf<<endl;
						cout<<"**************************************************"<<endl;
						*/
						continue;						
					}
					//start of a new read detected
					temp = buf.substr(0,khmerSize); //here temp denote the string khmer
					
					//std::cout<<"khmer's in the read (first):: "<<temp<<std::endl;
					
					if(bufSize > khmerSize){
						//khmer has a neighbour :) -- so put the khmer in the blockArray
						
						//saving the first khmer
						//myfile<<temp<<endl;//_revhash(lastFoundKHashVal,khmerSize)<<endl; //saving the first khmer -- revhash isn't working right now
						
						// hash function trying formation 0th->1th->2th->3th
						bool stored = false;
						for(int ii=0;ii<=LAST_HASH_FUNC;ii++){
						
							//for the first khmer all the 
							//saving signature that which hash function is being used for the first khmer
							lastFoundKHashVal[ii] = fullHasher(temp,ii); //saving the hash
							
							if(buf.length() > khmerSize ){//checking that next khmer is not invalid
								switch(toupper(buf[khmerSize])){
									case 'A':
									case 'T':
									case 'G':
									case 'C':
									break;
									
									default:
									neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
									break;
								}
							}
							
							if(neucleotide_status == N_FOUND_IN_NEUCLEOTIDE){
								n_found = true;
								break;
							}
							
							//std::cout<<"store try ("<<ii<<")"<<endl;
							
							//cout<<"trying to store-"<<temp<<" hashfunc-"<<ii<<" hashed value-"<<lastFoundKHashVal[ii]<<" neighbour "<<buf[khmerSize]<<endl;
							switch(storeKhmerInMemBlock6Byte(temp,memBlock,blockSize,lastFoundKHashVal[ii],buf[khmerSize],ii)){
							
							case MEM_BLOCK:
								mem_block_occupied_stat++;
								mem_block_total_number_of_occupied++;
								lastHashFuncUsed = ii;
								stored = true;
								break;
							case MEM_BLOCK_FOUND:
								//cout<<"mem_block_found :: "<<temp<<endl;
								mem_block_found_stat++;
								mem_block_total_number_of_occupied++;
								lastHashFuncUsed = ii;
								stored=true;
								break;
							case LIST:
								list_occupied_stat++; //this will happen only for the las time (hash Func 3)
								lastHashFuncUsed = ii;
								vector_total_number_of_occupied++;
								stored = true;
								break;
							case LIST_FOUND:
								//cout<<"list_found :: "<<temp<<endl;
								list_occupied_found++;
								vector_total_number_of_occupied++;
								lastHashFuncUsed = ii;
								stored=true;
								break;
							case ERROR:
								//std::cout<<"ERROR OCCURED"<<std::endl;
								//return;
								break;
							}
							if(stored){
								break; //break out from the for loop
							}
						}
						//for debug purpose
						//std::cout<<"inside firstkhmer "<<std::endl;
						//std::cout<<temp<<std::endl;
					}

					isFirstKhmer = false;
				}
				
				if(n_found == true){ //in the first khmer N found so abort consuming using continue statement
					continue;
				}
				
				
				
				for( ; i <= loopTime && n_found == false; i++){
					temp = buf.substr(i,khmerSize);

					//std::cout<<"khmer's in the read (intermediate) :: "<<temp<<std::endl;

					addResult = i + khmerSize;

					if(bufSize > addResult){
						
						bool stored = false;
						for(int ii=0;ii<=LAST_HASH_FUNC;ii++){
							
							if(ii<=lastHashFuncUsed){
								//last used hash func & current hashfunc is same -- so partial hasher would do the job
								
								//neighbour left so put it into the memBlock
								//cout<<"using partial hash func with last hash func value "<<lastFoundKHashVal[ii]<<" and next neighbour "<<temp[khmerSize - 1]<<endl;
								lastFoundKHashVal[ii] = partialHasher(lastFoundKHashVal[ii],temp[khmerSize - 1],khmerSize,ii);
								//lastFoundKHashVal = fullHasher(temp,ii); //saving the hash
							}  
							else{
								lastFoundKHashVal[ii] = fullHasher(temp,ii); //saving the hash
							}
							
							if(buf.length() >= addResult + 1){//checking that next khmer is not invalid
								switch(toupper(buf[addResult])){
									case 'A':
									case 'T':
									case 'G':
									case 'C':
									break;
									
									default:
									neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
									break;
								}
							}
							
							if(neucleotide_status == N_FOUND_IN_NEUCLEOTIDE){//N found in the middle of consuming
								n_found = true;
								break;
							}
							
							//std::cout<<"store try ("<<ii<<")"<<endl;
							
							//cout<<"trying to store-"<<temp<<" neighbour "<<buf[addResult]<<" hashfunc-"<<ii<<" last found hashed value "<<lastFoundKHashVal[ii]<<endl;
							//cout<<"Hash "<<temp<<" "<<buf[addResult]<<endl;
							
							switch(storeKhmerInMemBlock6Byte(temp,memBlock, blockSize, lastFoundKHashVal[ii], buf[addResult],ii)){

								case MEM_BLOCK:
									mem_block_occupied_stat++;
									mem_block_total_number_of_occupied++;
									lastHashFuncUsed = ii;
									stored = true;
									break;
								case MEM_BLOCK_FOUND:
									//cout<<"mem_block_found:: "<<temp<<endl;
									mem_block_found_stat++;
									mem_block_total_number_of_occupied++;
									lastHashFuncUsed = ii;
									stored=true;
									break;
								case LIST:
									list_occupied_stat++;
									vector_total_number_of_occupied++;
									lastHashFuncUsed = ii;
									stored = true;
									break;
								case LIST_FOUND:
									//cout<<"list_found:: "<<temp<<endl;
									list_occupied_found++;
									vector_total_number_of_occupied++;
									lastHashFuncUsed = ii;
									stored=true;
									break;
								case ERROR:
									//std::cout<<"ERROR OCCURED"<<std::endl;
									break;
								
							}

							if(stored == true){
								//if stored then just end the loop
								break;
							}
							
							//for debug purpose
							//std::cout<<"\tinside loop "<<std::endl;
							//std::cout<<"\t\t"<<temp<<std::endl;
						}
					}

				}

				//temp = temp.substr(1,khmerSize-1);//advance to next khmer one time of ahead
			}
		}

	//myfile.close();

	//spit out the stats
	std::cout<<"stat from consume function :: "<<std::endl
			<<"khmer in memBlock (distinct):: "<<mem_block_occupied_stat<<std::endl
			<<"khmer in list (distinct):: "<<list_occupied_stat<<std::endl<<std::endl;
			/*<<"khmer in memBlock (found):: "<<mem_block_found_stat<<std::endl
			<<"khmer in list (found):: "<<list_occupied_found<<std::endl<<std::endl
			<<"Khmer in memBlock(total) "<<mem_block_total_number_of_occupied<<std::endl
			<<"khmer in vector(total) "<<vector_total_number_of_occupied<<endl;*/
}

/*  for both 5byte & 6byte this functionality isn't common -- thus why not moved to the common.cpp

 * this function returns FOUND if the given khmer is found
 * else return NOT_FOUND
 * NB:: for the last khmer NOT_FOUND is always retured
 */
 int findGivenKhmer6Byte(string sample,struct khmerInfo6Byte *memBlock,unsigned long long blockSize, unsigned long long khmer_h,unsigned int whichHashFunc){
		unsigned long long index;
		unsigned long long qutioned; //this value must be under 12bit
		char neighbour_musk = 0xF0;
		
		//cout<<sample << " whichhash::"<<whichHashFunc;
		
		//unsigned char upperQution,lowerQution;//this shit must be unsigned

		//find the index value in the memBlock
		index = khmer_h % blockSize;
		qutioned = khmer_h / blockSize;//this value must be under 12bit
		
		//cout<<"rechecking blcoksize "<<blockSize<<" index "<<index<<" khmer_h "<<khmer_h<<endl;

		/*lowerQution = (qutioned & 0xFF);
		upperQution = ((qutioned & 0x0F00) >> 8);
		*/

		//check that index in the memBlock
		//1.neighbour table
		//2.qutioned table
		if(((memBlock[index].data[4] >> 1) & 0x07) == whichHashFunc && //hash value matched
			(memBlock[index].data[0] == (qutioned & 0xFF)) &&
			(memBlock[index].data[1] == (qutioned & 0xFF00)>>8) &&
			(memBlock[index].data[2] == (qutioned & 0xFF0000)>>16) &&
			(memBlock[index].data[3] == (qutioned & 0xFF000000)>>24) &&
			((memBlock[index].data[4] & 0x01) == ((qutioned & 0x0100000000) >> 32))){

			return FOUND;
		}
		else if(whichHashFunc == LAST_HASH_FUNC){
			//cout<<"asche 01 for "<<sample<<endl;//delete this
			vector<struct vector_trial_6_byte>::iterator it;
			int index_v=index%num_of_vector;
			int index_vect=0;
			for(it = mapPointer6Byte[index_v].begin();it!=mapPointer6Byte[index_v].end();it++)
			{
				if(it->index_val == index)
				{
					break;
				}
				index_vect++;
			}
			if(it == mapPointer6Byte[index_v].end())
			{
				//cout<<"asche 02 for "<<sample<<" searching in "<<index_v<<" for memblock index "<<index<<" hash func no "<<whichHashFunc<<endl;//delete this
				return NOT_FOUND;
			}
			else
			{	//its actuallty found
				//cout<<"asche 03 for "<<sample<<endl;//delete this
				bool isFound = false;
				for(std::vector<khmerInfo6Byte>::iterator pi = mapPointer6Byte[index_v][index_vect].vect.begin();
						pi != mapPointer6Byte[index_v][index_vect].vect.end() ; pi++)
				{
					if(
						((pi->data[4] >> 1) & 0x07) == whichHashFunc && //hash value matched
						(pi->data[0] == (qutioned & 0xFF)) &&
						(pi->data[1] == (qutioned & 0xFF00)>>8) &&
						(pi->data[2] == (qutioned & 0xFF0000)>>16) &&
						(pi->data[3] == (qutioned & 0xFF000000)>>24) &&
						((pi->data[4] & 0x01) == ((qutioned & 0x0100000000) >> 32))){
					//(pi->data[0] == (qutioned & 0xFF)) && ((pi->data[1] & 0x0F) == ((qutioned & 0x0F00) >> 8))){
						//cout<<"asche 04 for "<<sample<<endl;//delete this
						isFound = true;
						break;
					}
				}
				if(isFound==true)
					return FOUND;
				else
					return NOT_FOUND;
			}
				
		}
	return NOT_FOUND;
}

/* for both 5byte & 6byte this functionality isn't common -- thus why not moved to the common.cpp

 * read the input file and check the data-structure for the khmer
 * we make from the input file, log the khmer that not found
 */
void reCheckTheKhmerExistence6Byte(struct khmerInfo6Byte * memBlock, unsigned long long blockSize, char *fileName,unsigned short int khmerSize){
	//opening the given file
		std::ifstream inputFile;
		inputFile.open(fileName,std::fstream::in);
		std::string buf,temp="";
		bool isFirstKhmer = true;
		unsigned long long *lastFoundKHashVal;
		lastFoundKHashVal=new unsigned long long[LAST_HASH_FUNC+1];		
		unsigned long long consumedRead = 0;
		unsigned int lastHashFuncUsed = 0;//containe the value that is last used to store last khmers
		unsigned long mem_block_occupied_stat = 0;
		unsigned long list_occupied_stat = 0;
		
		bool n_found = false;//added for the purpose of detecting N in the khmer
		
		//for stat purpose use only:)
		unsigned long found_khmer = 0,not_found=0;

		//error log if can't be opened
		if(inputFile.fail() == true){
			/*
			 * if fail to load the file -- return
			 */
			std::cout<<"Fail to open the "<<fileName << std::endl;
			return;
		}


		//if successfull then log it !!!
		std::cout<<"Rechecking... "<<fileName<<std::endl;

		//parsing the fasta file
		while(getline(inputFile,buf)){//reading a line

				if(buf[0] == '>'){
					//its a sequence name -- omit it !!! -- before omit clear the temp
					n_found = false;//new sequence so assign false to the N found tag
					temp = ""; //cleaning the old remaining khmer
					isFirstKhmer = true;
					continue;
				}
				else if (n_found == false){
					//here come the one sequnece of read
					buf = temp + buf;//no new read sequnence is found so the next line of read must be appended to the last
					unsigned int i;
					unsigned int bufSize = buf.size();//for optimization purpose only
					unsigned int loopTime = bufSize - khmerSize;//for optimization purpose only
					unsigned int addResult; //for optimization only

					if(isFirstKhmer == true){
						//its the first read line of the multiple read lines
						i = 1;
					}
					else{
						//its not the first line of the multiple read line
						i= 0;
					}

					//if it the first line then consume it explicitly by using fullHasher
					if(isFirstKhmer == true){
						if(bufSize < khmerSize){
						/*cout<<"**************************************************"<<endl;						
						cout<<"Problem detected !!!"<<endl;
						cout<<"read sequence size is less then the khmer size -- processing next read :("<<endl;
						cout<<"seq :: "<<buf<<endl;
						cout<<"**************************************************"<<endl;
						*/
						continue;						
					}
						//start of a new read detected
						temp = buf.substr(0,khmerSize); //here temp denote the string khmer
						
						//lastFoundKHashVal = fullHasher(temp); //saving the hash value
						//std::cout<<"recheck khmer's in the read (first) :: "<<temp<<std::endl;

						
						
						if(bufSize > khmerSize){
							
							bool found = false;
							
							for(int ii=0;ii<=LAST_HASH_FUNC;ii++){
							
							
								lastFoundKHashVal[ii] = fullHasher(temp,ii); 
									
								if(buf.length() > khmerSize ){//checking that next khmer is not invalid
									switch(toupper(buf[khmerSize])){
										case 'A':
										case 'T':
										case 'G':
										case 'C':
										break;
										
										default:
										neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
										break;
									}
								}
							
								if(neucleotide_status == N_FOUND_IN_NEUCLEOTIDE){
									n_found = true;
									break;//breaking the hash loop !!!!
								}
								
								//std::cout<<"try to find("<<ii<<")"<<endl;
								
								//khmer has a neighbour :) -- so put the khmer in the blockArray
								//cout<<"trying to find "<<termp<<endl;
								switch(findGivenKhmer6Byte(temp,memBlock,blockSize,lastFoundKHashVal[ii],ii)){

								case FOUND:
									found_khmer++;
									//std::cout<<temp<< " -- found"<<std::endl;
									found = true;
									lastHashFuncUsed=ii;
									break;
								case NOT_FOUND:
									//std::cout<<temp<< " -- not found"<<std::endl;
									found = false;
									break;
								}
								if(found){
								break; //break out from the for loop
							}
						}
						
						if(n_found == true){ //main while loop continues -- allowing to consume the line of read
							continue ;
						}
						else if(n_found  == false && found == false){//error reporting that -- the khmer is not found
							not_found++;
							std::cout<<temp<< "at first -- not found "<<std::endl;
							std::cout<<"buffer :: " <<buf<<std::endl;
						}
						
						//for debug purpose
						//std::cout<<"inside firstkhmer "<<std::endl;
						//std::cout<<temp<<std::endl;
					}
					isFirstKhmer = false;
				}

				if(n_found == true){ //main while loop continues -- allowing to consume the line of read
					continue ;
				}
				
				for( ; i <= loopTime && n_found == false; i++){
					temp = buf.substr(i,khmerSize);

					//std::cout<<"recheck khmer's in the read (intermediate) :: "<<temp<<std::endl;
					
					addResult = i + khmerSize;
					
					if(bufSize > addResult){
						
						bool found = false;
						for(int ii=0;ii<=LAST_HASH_FUNC;ii++){
						
							if(ii<=lastHashFuncUsed){
								//last used hash func & current hashfunc is same -- so partial hasher would do the job
								
								//neighbour left so put it into the memBlock
								lastFoundKHashVal[ii] = partialHasher(lastFoundKHashVal[ii],temp[khmerSize - 1],khmerSize,ii);
								//lastFoundKHashVal = fullHasher(temp,ii);
							}
							else{
								lastFoundKHashVal[ii] = fullHasher(temp,ii); //saving the hash
								
							}
							
							if(buf.length() >= addResult + 1){//checking that next khmer is not invalid
								switch(toupper(buf[addResult])){
									case 'A':
									case 'T':
									case 'G':
									case 'C':
									break;
									
									default:
									neucleotide_status = N_FOUND_IN_NEUCLEOTIDE;
									break;
								}
							}
							
							if(neucleotide_status == N_FOUND_IN_NEUCLEOTIDE){
								n_found = true;
								break;
							}
							
							//std::cout<<"try to find("<<ii<<")"<<endl;
							
							//neighbour left so put it into the memBlock
							//lastFoundKHashVal[ii] = partialHasher(lastFoundKHashVal,temp[khmerSize - 1],khmerSize,ii);
							switch(findGivenKhmer6Byte(temp,memBlock,blockSize,lastFoundKHashVal[ii],ii)){
								case FOUND:
									found_khmer++;
									//std::cout<<temp<< " -- found"<<std::endl;
									lastHashFuncUsed = ii;
									found = true;
									break;
								case NOT_FOUND:
									//cout<<temp<< " -- not found"<<std::endl;
									found = false;
									break;
							}

							//for debug purpose
							//std::cout<<"\tinside loop "<<std::endl;
							//std::cout<<"\t\t"<<temp<<std::endl;
							if(found){								
								break; //break out from the for loop
							}
						}
						
						if(n_found == false && found == false){//error reporting that -- the khmer is not found
							not_found++;
							std::cout<<temp<< " -- not found"<<std::endl;
						}

					}
					
					if(n_found == true){
						break;
					}
					
				}
				//temp = temp.substr(1,khmerSize-1);//advance to next khmer one time of ahead

				//std::cout<<"one line of read consumed" <<std::endl;
			}
		}

		//spit out the stats
		std::cout<<"stat from recheck function :: "<<endl
				<<"\tkhmer found :: "<<found_khmer<<endl<<"\tkhmer not found:: "<<not_found<<endl<<endl;
}


void print6ByteHashTableAndVectorStructure(void* blockArr,unsigned long long hashSize, unsigned int kSize,vector<struct vector_trial_6_byte> *mapPointer6Byte)
{
	printf("6 byte hashtable & vector info dumping began\n");
	
	char neighbour_musk = 0xF0;
	
	FILE *fp, *fp2;
	fp		=fopen("hashtable_6_Byte.txt", "w");
	fp2 	=fopen("vector_structure_6_byte.txt", "w"); 

	    /**
		* dumping the HashTable
		**/
		//its a 6 byte structure
		unsigned long long hashValue 	= 0;
		unsigned long long qutioned 	= 0;
		unsigned short hashFunc			= 0;
		char 		   neighbour		= 0;
		
		khmerInfo6Byte *blockArray6 = (khmerInfo6Byte*) blockArr;
		for(unsigned long long index = 0; index < hashSize;index++)
		{
			//now check specified index is occupied or not
			if((blockArray6[index].data[4] & neighbour_musk) != 0){
				//occupuying here

				//forming the qutioned
				qutioned = blockArray6[index].data[4] & 1; // only the last bit is the part of the qutioned
				qutioned = (qutioned << 8) | blockArray6[index].data[3];
				qutioned = (qutioned << 8) | blockArray6[index].data[2];
				qutioned = (qutioned << 8) | blockArray6[index].data[1];

				//forming the hash value
				hashValue = (hashSize * qutioned) + index;
				hashFunc = (blockArray6[index].data[4] & 0x0E ) >> 1; //mask out the hash function

				//prints the khmer with the count information
				fprintf(fp, "%s", _revhash(hashValue,hashFunc,kSize).c_str());
				
				char tempMask = 0x10;
				for(unsigned int ii = 0;ii < 4;ii++)
				{
					//looping through this values
					//0001 0000
					//0010 0000
					//0100 0000
					//1000 0000
					neighbour = ' ';
					//forming the neighbour
					switch(blockArray6[index].data[4] & tempMask)//making out the first 4 bit
					{
						case 0x80://found A
							neighbour = 'A';
						break;
						
						case 0x40://found T
							neighbour = 'T';
						break;
						
						case 0x20://found C
							neighbour = 'C';
						break;
						
						case 0x10://found G
							neighbour = 'G';
						break;
					}
					
					fprintf(fp, "-%c", neighbour);
					tempMask = tempMask << 1;
				}
				
				fprintf(fp, "-%d\n",blockArray6[index].count);
			}
		}
		
		/**
		* dumping the Vector Strcuture -- using the same variable as dumping the hashtable :)
		**/
		
		unsigned long long vect_index = 0;
		for(unsigned long long i=0;i<num_of_vector;i++)//number of vector is declared in the common.cpp -- so no need to pass around
		{
			for(unsigned long long j=0;j<mapPointer6Byte[i].size();j++)
			{	
				
				vect_index = mapPointer6Byte[i][j].index_val;
				for(unsigned long long k=0;k < mapPointer6Byte[i][j].vect.size() ; k++)//newly changes in the loop
				{
					//now take steps to print 
					//now check specified index is occupied or not
					if((mapPointer6Byte[i][j].vect[k].data[4] & neighbour_musk) != 0){
						//occupuying here
						
						//cleaning the syringe to push injection ;) beware of aids :P
						neighbour = hashFunc = qutioned = hashValue = 0;
			
						//forming the qutioned
						qutioned = mapPointer6Byte[i][j].vect[k].data[4] & 1; // only the last bit is the part of the qutioned
						qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[3];
						qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[2];
						qutioned = (qutioned << 8) | mapPointer6Byte[i][j].vect[k].data[1];

						//forming the hash value
						hashValue = (hashSize * qutioned) + vect_index;
						hashFunc = (mapPointer6Byte[i][j].vect[k].data[4] & 0x0E ) >> 1; //mask out the hash function
						
						//prints the khmer
						fprintf(fp2, "%s", _revhash(hashValue,hashFunc,kSize).c_str());
						
						char tempMask = 0x10;
						for(unsigned int ii = 0;ii < 4;ii++)
						{
							//looping through this values
							//0001 0000
							//0010 0000
							//0100 0000
							//1000 0000
							neighbour = ' ';
							//forming the neighbour
							switch(mapPointer6Byte[i][j].vect[k].data[4] & tempMask)//making out the first 4 bit
							{
								case 0x80://found A
									neighbour = 'A';
								break;
								
								case 0x40://found T
									neighbour = 'T';
								break;
								
								case 0x20://found C
									neighbour = 'C';
								break;
								
								case 0x10://found G
									neighbour = 'G';
								break;
							}
							
							fprintf(fp2, "-%c", neighbour);
							tempMask = tempMask << 1;
						 }
						
						//prints the khmer with the count information
						fprintf(fp2, "-%d\n", mapPointer6Byte[i][j].vect[k].count);
					}
				}
			}
		}
		
	fclose(fp);
	fclose(fp2);
	
	printf("6 byte hashtable & vector info dumping ended\n");
}



void main6Byte(int argc, char** argv) {

	//freopen("results.txt","w",stdout);
	unsigned long long size;
	int kSize;
	std::string::size_type sz = 0;
	char *filename=new char[200];
	num_of_vector = get_a_prime_above_x(500000);
	
	/*
	* new variable introduced in the 6 byte code section --- which is cutoff value
	*/
	
	if(argc < 7){//newly changed
		std::cout<<"sufficient number of argument is not passed"<<std::endl<<" 'fileName' 'khmersize' 'howManyHash' 'cut-off or no cut-off' 'print or not print' 'hashSize' 'multiplying factor'"<<std::endl;//newly changed
		return ;
	}


	/*std::string::size_type sz = 0;
	std::cout<<"Give input file name :: "<<std::endl;
	std::cin>>filename;*/

	//fetching the args
	strcpy(filename,argv[1]);
	kSize = atoi(argv[2]);
	LAST_HASH_FUNC = atoi(argv[3])-1;//0 indexing hash function used
	
	//size =  std::stoull(argv[3],&sz,10);
	size= 0;
	if(argc == 7){ //size provided as the last argument -- so process it //newly changed
		size =  std::stoull(argv[6],&sz,10);//newly changed
	}
	else if(argc == 8)//newly changed
	{
		/*
		* here we parse the multiplying by factor --- which is not optional anymore
 		*/
		size =  std::stoull(argv[6],&sz,10) * atof(argv[7]);//newly changed
	}

/* unblock this area -- you must should (blocked this code section due to low memory space)
 * warning 
 * warning
 * WARNING --- unblock this code of section before releasing
 */
 /*
	if(size < (pow(2,2 * kSize - 33)+ 536870912)){
		size = pow(2,2 * kSize - 33)+ 536870912;
	}
*/

// helps to keep output in a file

	string fileName = "5_";
	fileName.append(argv[1]+3);
	size_t pos=fileName.find_first_of('.');
	if(pos!=string::npos)
		fileName=fileName.substr(0,pos);
	fileName.append("_");
	fileName.append(argv[2]);
	fileName.append("_");
	fileName.append(argv[3]);
	if(argc>4)
	{
		fileName.append("_");
		fileName.append(argv[4]);
		if(argc>5)
		{
			fileName.append("_my_");
			fileName.append(argv[5]+2);
		}
	}
	fileName.append(".txt");
	
	//enable this line will give you access the whole output in the cmd
	freopen(fileName.c_str(),"w",stdout);

	
	currentDateTime();
	
	/*std::cout<<"Give Kmer Size :: "<<std::endl;
	std::cin>>kSize;
	std::cout<<"Minimum space you want to give to hash's :: "<<std::endl;
	std::cin>>size;*/
	//initial memory arrangement
	size=get_a_prime_above_x(size);
	cout<<"Hash size is "<<size<<endl;

	std::cout<<"filename::"<<filename<<" ksize::"<<kSize<<" Size::"<<size<<std::endl;

	populateTwoBitReprHashMap();//populating map
	struct khmerInfo6Byte *blockArray=new struct khmerInfo6Byte[size];
	mapPointer6Byte=new vector<struct vector_trial_6_byte>[num_of_vector];
	
	//cleaning the array
	memset( blockArray, 0, sizeof(struct khmerInfo6Byte) * size);

/*	
	bool zeroFound = true;
	for(int j=0;j<size;j++){
		if(blockArray[j].count != 0){
			zeroFound = false;
			break;
		}		
	}

	if(zeroFound == false){
		cout<<"All are not set to zero"<<endl;
	}
	else{
		cout<<"All are zero"<<endl;
	}


	cout<<"After memory allocation"<<endl;
	*/
	
	currentDateTime();

	//this function will populate the whole memblock & the vector structure
	populateBlock6Byte(blockArray, size , filename , kSize);

	show_vect_status_6_byte();
	
	currentDateTime();
	
	//rechecking ...
	//reCheckTheKhmerExistence6Byte(blockArray,size,argv[1],kSize);
	show_count_status(blockArray,size);
	currentDateTime();
	
	double vm, rss;
	process_mem_usage(vm, rss);
	
	cout<<"HashTable memory allocation (GB):: "<<(size * 6 )/(1024 * 1024 * 1024)<<endl;
	cout<<"Vector memory allocation    (GB):: "<<(((vm)/1024)/1024) - ((size * 6 )/(1024 * 1024 * 1024)) <<endl;

	cout << "Total memory used (GB):  " << ((vm)/1024)/1024 <<endl;
	
	/*//newly changed
	* before deleting the block array check for the print state
	*/
	if(strcmp("p",argv[5]) == 0 ||  strcmp("P",argv[5]) == 0 ) //newly changed
	{
		//time to print the whole block array
		print6ByteHashTableAndVectorStructure(blockArray , size, kSize,mapPointer6Byte);
	}
	
	delete blockArray;
	
//	fclose(stdout);
	return ;
}
