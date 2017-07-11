
CXX=g++
CXXFLAGS="-std=c++11"


havecmake: src/main.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o src/common.o
	$(CXX) src/main.o src/vector_with_rehash_8_hash_with_N_detection_5_bytes.o src/vector_with_rehash_8_hash_with_N_detection_6_bytes.o src/common.o -o Havec.out


.PHONY: clean
clean:
	rm -f src/*.o *.out 
