run with flag -lpthread -lm -std=c++11

the code should be run like ./md5 + number of threads

1. cd to the code file direction
2.open terminal, type: $g++ md5_run_pthread.cpp md5_sequential.h -o md5 -std=c++11 -lpthread -lm
3. type: $ ./md5 8    (Here I assume using 8 threads)

