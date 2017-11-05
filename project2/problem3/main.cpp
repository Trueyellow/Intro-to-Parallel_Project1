//Written by HKX to launch Sequential c++ code for md5 algorithm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <iostream>
#include "md5_sequential.h"
#include <chrono>

using namespace std;
typedef std::chrono::high_resolution_clock Clock;
int MAX_THREAD;
const float TOTAL_NUM = 992436543;
int MAX_RUNNING_DATA = 125000000;
md5_sequential md5_sequential_class;

void * md5_wrapper(void *);

// The md5 value that we should find out
const uint8_t we_want_find[16] =
{
    0x6f, 0x50, 0xf6, 0xc3, 0xd9, 0x83, 0xd6, 0xa8,
    0x49, 0xa0, 0x2d, 0x36, 0xa2, 0xf6, 0x9b, 0x5c
};


// data pass to thread
typedef struct data_to_thread{
    uint8_t **msg_matrix = new uint8_t*[MAX_RUNNING_DATA];
    uint8_t store_sequential[16];
} data_to_thread;



int main(int argc, char *argv[]) {

    // Assign ascii code to a int list for better call
    // and here are 63 code with ascii[0] = 0, and number 0-9, character A-Z, a-z
    MAX_THREAD = atoi(argv[1]);
    float time_consume;

    // dynamic assignment of data to make full use of threads
    MAX_RUNNING_DATA = ceil((double)TOTAL_NUM / MAX_THREAD);
    while(MAX_RUNNING_DATA > 125000000)
        MAX_RUNNING_DATA = ceil((double)MAX_RUNNING_DATA/2);
    cout<< "The working data length distributed to each thread is " << MAX_RUNNING_DATA <<endl;

    pthread_t threads[100];
    uint8_t ascii[63];
    ascii[0] = 0;

    for(u_int8_t j=1; j<63;j++){
        if(j<11)
            ascii[j] = j + 47;
        else if((11 <= j) && (j < 37))
            ascii[j] = j + 54;
        else if((37 <= j) && (j< 63))
            ascii[j] = j + 60;
    }

    int thread_num = 0, total_num = 0;

    // Initialize data
    data_to_thread md5_data;
    for(int i=0; i<MAX_RUNNING_DATA;i++)
        md5_data.msg_matrix[i] = new uint8_t[5];

    auto begin_time = Clock::now();
    // Separate ascii code by chunk of max_num
    for(int a4 = 0;a4 < 63; a4++){
        for(int a3 = 0;a3 < 63; a3++){
            for(int a2 = 0;a2 < 63; a2++){
                for(int a1 = 0; a1 < 63; a1++){
                    for(int a0 = 0; a0 < 63; a0++){
                        total_num += 1;
                        // Assign a chunk of assii code
                        md5_data.msg_matrix[(total_num % MAX_RUNNING_DATA)][0] = ascii[a0];
                        md5_data.msg_matrix[(total_num % MAX_RUNNING_DATA)][1] = ascii[a1];
                        md5_data.msg_matrix[(total_num % MAX_RUNNING_DATA)][2] = ascii[a2];
                        md5_data.msg_matrix[(total_num % MAX_RUNNING_DATA)][3] = ascii[a3];
                        md5_data.msg_matrix[(total_num % MAX_RUNNING_DATA)][4] = ascii[a4];

                        // if data_num % MAX_RUNNING_DATA, pass  the value to thread to calculate

                        if((total_num % MAX_RUNNING_DATA == 0) && total_num != 0){
                            cout<<"Thread create"<<endl;
                            pthread_create(&threads[thread_num], NULL, md5_wrapper, &md5_data);
                            thread_num ++;
                        }

                        // Join threads
                        if(thread_num%MAX_THREAD == 0 && thread_num != 0){
                            for(int i = 0; i < MAX_THREAD; i++)
                            {
                                pthread_join(threads[i], NULL);
                                cout<<"Thread end!"<<endl;

                            }
                            thread_num = 0;
                        }
                    }
                }
            }
        }
    }

    cout<<"Thread create"<<endl;
    pthread_create(&threads[thread_num], NULL, md5_wrapper, &md5_data);
    thread_num ++;

    for(int i = 0; i < thread_num; i++)
    {
        pthread_join(threads[i], NULL);
        cout<<"Thread end!"<<endl;
    }
    auto end_time = Clock::now();
    time_consume = (float) std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time).count() ;

    cout<< total_num<<endl;
    printf("The total time spend on running with %d threads :\t%.3f seconds\n", MAX_THREAD, time_consume / 1e9);
    return 0;
}

void * md5_wrapper(void * arg){

    data_to_thread * temp = (data_to_thread *) arg;
    for(int i=0; i<MAX_RUNNING_DATA;i++){
        // calculate char number of ascii code
        int char_num = 0;
        for(int q=0;q<5;q++)
            if(temp->msg_matrix[i][q]!= 0)
                char_num += 1;

        // calculate time consumed calculation
_pointer
        md5_sequential_class.md5_sequential_calculate(temp->msg_matrix[i], char_num, temp->store_sequential);
    }

    return 0;
}
