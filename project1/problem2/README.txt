To run our code for project1-problem2:

1. cd to the folder, than open terminal and type "$g++ tasksys.cpp -o tasksys.o -c -lpthread"
2. input "$ispc --arch=x86-64 --target=sse2 md5.ispc -o md5.o -h md5.h"
3. input "$g++ -o md5_run_sequential md5_run_sequential.cpp md5_sequential.h -std=c++11"
4. input "$g++ -o md5_run_ispc md5_run_ispc.cpp md5.h md5.o tasksys.o -lpthread -std=c++11"
5. input "$g++ -o md5_run_ispc_task md5_run_ispc_task.cpp md5.h md5.o tasksys.o -lpthread -std=c++11"
6. run the file "$./md5_run_sequential" to get sequential result of RUNNING WHOLE CODE CRACKER ABOUT 63*63*63*63*63 times, and you can also change "we_want_find" and "true_result" to set the correct code you want to find 
6. run the file "$./md5_run_ispc" to get ispc result of RUNNING WHOLE CODE CRACKER ABOUT 63*63*63*63*63 times, and you can also change "we_want_find" and "true_result" to set the correct code you want to find 
6. run the file "$./md5_run_ispc_task" to get ispc multitask result of RUNNING WHOLE CODE CRACKER ABOUT 63*63*63*63*63 times, and you can also change "we_want_find" and "true_result" to set the correct code you want to find 
