//Written by HKX to launch SIMD ispc for md5 algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include "md5.h"
#include "md5_sequential.h"

using namespace ispc;
const int number_of_data = 100000000;

//PRINT FUNCTION
void output(uint8_t * output_data){
for(int i=0;i<5;i++)
printf("%d", output_data[i]);
}

// The md5 value that we should find out
const uint8_t we_want_find[16] =
{
  0x5c, 0xe8, 0xb4, 0xe3, 0x3c, 0x23, 0x62, 0x21,
  0x3d, 0xc2, 0x2f, 0x6c, 0x90, 0x8a, 0x46, 0xc6
};

// results ascii code
const uint8_t true_result[5] =
{
75, 75, 75, 0, 0
};


int main() {
  //ascii code
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

    // message matrix  *******using  static to allocate
    // more memory that without segment fault
    static uint8_t msg_matrix[number_of_data][5];
    int new_len = 56;
    //result matrix
    uint8_t result[5];

    int total_num = 0;
    // time counter
    float tISPC1 = 0.0;
    //early stop flag
    bool flag = false;
    // seperate ascii code by chunk of max_num
    for(int a4 = 0;a4 < 63&&flag==false; a4++,total_num++){
    for(int a3 = 0;a3 < 63&&flag==false; a3++,total_num++){
    for(int a2 = 0;a2 < 63&&flag==false; a2++,total_num++){
    for(int a1 = 0;a1 < 63&&flag==false; a1++,total_num++){
    for(int a0 = 0;a0 < 63&&flag==false; a0++,total_num++){

      // generate a chunk of data
      msg_matrix[(total_num % number_of_data)][0] = ascii[a0];
      msg_matrix[(total_num % number_of_data)][1] = ascii[a1];
      msg_matrix[(total_num % number_of_data)][2] = ascii[a2];
      msg_matrix[(total_num % number_of_data)][3] = ascii[a3];
      msg_matrix[(total_num % number_of_data)][4] = ascii[a4];

    // Load a chunk of data to ispc function
    if(total_num % number_of_data == 0 && total_num != 0){
        clock_t begin_time = clock();
        md5_cal(msg_matrix, new_len, number_of_data, we_want_find, result);
        tISPC1 += float(clock() - begin_time);

        flag = true;
        for(int h=0;h<5;h++)
          if (result[h] != true_result[h])
            flag = false;
      }
  }}}}}

printf("The total time spend on running ISPC :\t%.3f seconds\n", tISPC1/CLOCKS_PER_SEC);
std::cout<<"\nispc output"<<std::endl;
output(result);

return 0;
}
