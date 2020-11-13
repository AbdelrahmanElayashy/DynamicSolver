rm -rf *.o
rm -rf auto a.auto
gcc-8 -c *.cpp -pthread
gcc-8 -pthread -o auto *.o
                                  
