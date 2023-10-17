cd ../../unionfind/prefixLib
make
cd ../
make

cd ../paratreet/utility/structures
./configure
make
cd ../../src
make
make -j8
cd ../examples
make FoFApp
