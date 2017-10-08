//Written by HKX to launch ispc task for md5 algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include "md5.h"


using namespace ispc;
const int MAX_RUNNING_DATA = 100000000;

// The md5 value that we should find out
const uint8_t we_want_find[16] =
{
  0x5c, 0xe8, 0xb4, 0xe3, 0x3c, 0x23, 0x62, 0x21,
  0x3d, 0xc2, 0x2f, 0x6c, 0x90, 0x8a, 0x44, 0xc6
};

const uint8_t true_result[5] =
{
75, 75, 75, 0, 0
};

//print function
void output(uint8_t * output_data){
  for(int i=0;i<5;i++)
  printf("%d", output_data[i]);
}


int main() {
  // Assign ascii code to a int list for better call
  // and here are 63 code with ascii[0] = 0, and number 0-9, character A-Z, a-z
  uint8_t ascii[63];
  ascii[0] = 0;
  for(int j=1;j<63;j++){
    if(j<11)
    ascii[j] = j + 47;
    else if((11 <= j) && (j < 37))
    ascii[j] = j + 54;
    else if((37 <= j) && (j< 63))
    ascii[j] = j + 60;
  }

  // matrix to store character to sent
  static uint8_t msg_matrix[MAX_RUNNING_DATA][5];


  // new_len is a len parameter, since we only got no more than 5 characters or
  // numbers, we can just assign this number
  int new_len = 56;

  //store result
  uint8_t result[5];

  //number counter
  int total_num = 0;

  // store time
  float tTASK[9];

  // flag for early stop
  bool flag = false;
  // seperate ascii code by chunk of max_num
  for(int a4 = 0;a4 < 63&&flag==false; a4++,total_num++){
  for(int a3 = 0;a3 < 63&&flag==false; a3++,total_num++){
  for(int a2 = 0;a2 < 63&&flag==false; a2++,total_num++){
  for(int a1 = 0;a1 < 63&&flag==false; a1++,total_num++){
  for(int a0 = 0;a0 < 63&&flag==false; a0++,total_num++){
    // assign a chunk of assii code
    msg_matrix[(total_num % MAX_RUNNING_DATA)][0] = ascii[a0];
    msg_matrix[(total_num % MAX_RUNNING_DATA)][1] = ascii[a1];
    msg_matrix[(total_num % MAX_RUNNING_DATA)][2] = ascii[a2];
    msg_matrix[(total_num % MAX_RUNNING_DATA)][3] = ascii[a3];
    msg_matrix[(total_num % MAX_RUNNING_DATA)][4] = ascii[a4];

    if((total_num % MAX_RUNNING_DATA == 0) && total_num != 0){
      //add up all those time for multi-task ispc

      for(int number_of_threads = 1; number_of_threads < 9; number_of_threads++){
        clock_t begin_time = clock();
        md5_task_wrap(msg_matrix,
                      new_len,
                      MAX_RUNNING_DATA,
                      we_want_find,
                      number_of_threads,
                      result);
        tTASK[number_of_threads] += float(clock() - begin_time);

        // running indicator
        std::cout<<number_of_threads<<" threads is running on "
                << total_num << std::endl;
        // test for result

        flag = true;
        for(int h=0;h<5;h++)
          if (result[h] != true_result[h])
            flag = false;
        }
       }
      }
     }
    }
   }
  }

        for(int i = 1; i < 9; i++){
          printf("The total time spend on running ISPC with %d thread:\t %.3f seconds\n",
          i, tTASK[i]/CLOCKS_PER_SEC);
        }
        std::cout<<"\nispc_task output"<<std::endl;
        output(result);

return 0;
}
