rm -rf build
mkdir build
cp calc_error.py ./build
cd build
cmake ..
make