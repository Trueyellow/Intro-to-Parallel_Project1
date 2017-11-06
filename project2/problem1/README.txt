Usage: ./program a b  
arg: a is the power of two which indicate the size of data. 
arg: b is the power of two which indicates number of threads

If you want to run sequential version, just set b to zero
like run $./sort 25 0



1. cd to the code file direction
2. open terminal, type: $g++ three_sort.cpp -o sort -std=c++11 -lpthread -lm
3. type: $./sort 25 4 (Here means run three sort in 2^25 random data and with 2^4 threads)





