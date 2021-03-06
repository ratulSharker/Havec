CXX=g++

CXXFLAGS+="-std=c++11"
#CXXFLAGS+="-lcunit"

havec:  src/main.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o src/common.o
	$(CXX) src/main.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o src/common.o -o Havec.out

ci:	
	g++-4.9 $(CXXFLAGS)   -c -o src/main.o src/main.cpp
	g++-4.9 $(CXXFLAGS)   -c -o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.cpp
	g++-4.9 $(CXXFLAGS)   -c -o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.cpp
	g++-4.9 $(CXXFLAGS)   -c -o src/common.o src/common.cpp
	g++-4.9 src/common.o src/main.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o -o Havec.out

test:	src/common.o tests/test_common.o
	$(CXX) src/common.o tests/test_common.o -lcunit -o test_common.out

install_deps:
	sudo apt-get install libcunit1 libcunit1-dev libcunit1-doc

remove_deps:
	sudo apt-get remove libcunit1 libcunit1-dev libcunit1-doc

.PHONY: clean
clean:
	rm -f src/*.o tests/*.o *.out
	rm -rf output/
