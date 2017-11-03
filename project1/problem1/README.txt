To run our code for project1-problem1:

1. cd to the folder, than open terminal and type "$g++ tasksys.cpp -o tasksys.o -c -lpthread"
2. input "$ispc --arch=x86-64 --target=sse2 root_calculate.ispc -o root_calculate.o -h root_calculate.h"
3. input "$g++ -o root_ispc root_ispc.cpp root_calculate.o root_calculate.h tasksys.o -lpthread -std=c++11"
4. run the file "$./root_ispc"




To run Multi thread:
1. cd to the folder, than open terminal
2. input "$g++ -o root_multi root_calculate_multithread.cpp -lpthread -std=c++11"
3. run the file "$./root_multi"

