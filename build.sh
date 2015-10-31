# This is the script for building Havec.
# You need GCC 4.7 or later to support the -std=c++11
# For more details visit https://gcc.gnu.org/projects/cxx0x.html

echo 'building Havec...'
g++ -std=c++11 src/main.cpp -o Havec.out
echo 'Havec executable Havec.out created'
echo 'build complete'

