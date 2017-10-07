#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "md5.h"
#include <iostream>
#include "md5_sequential.h"

using namespace ispc;
const int number_of_data = 1600000;


void output(uint8_t * output_data){
for(int i=0;i<5;i++)
printf("%d", output_data[i]);
}


// void running_wrapper(uint8_t * ascii,){
//
// }
int main() {
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

    uint8_t msg_matrix[number_of_data][5];
    int new_len = 56;
    uint8_t we_want_find[16];
    uint8_t result[5];

    we_want_find[0] = 0x5c;
    we_want_find[1] = 0xe8;
    we_want_find[2] = 0xb4;
    we_want_find[3] = 0xe3;
    we_want_find[4] = 0x3c;
    we_want_find[5] = 0x23;
    we_want_find[6] = 0x62;
    we_want_find[7] = 0x21;
    we_want_find[8] = 0x3d;
    we_want_find[9] = 0xc2;
    we_want_find[10] = 0x2f;
    we_want_find[11] = 0x6c;
    we_want_find[12] = 0x90;
    we_want_find[13] = 0x8a;
    we_want_find[14] = 0x46;
    we_want_find[15] = 0xc6;
    //
    int total_num = 0;
    float tISPC1 = 0.0;
    bool flag = false;
    // seperate ascii code by chunk of max_num
    for(int a4 = 0;a4 < 63&&flag==false; a4++,total_num++){
    for(int a3 = 0;a3 < 63&&flag==false; a3++,total_num++){
    for(int a2 = 0;a2 < 63&&flag==false; a2++,total_num++){
    for(int a1 = 0;a1 < 63&&flag==false; a1++,total_num++){
    for(int a0 = 0;a0 < 63&&flag==false; a0++,total_num++){
      msg_matrix[(total_num % number_of_data)][0] = ascii[a0];
      msg_matrix[(total_num % number_of_data)][1] = ascii[a1];
      msg_matrix[(total_num % number_of_data)][2] = ascii[a2];
      msg_matrix[(total_num % number_of_data)][3] = ascii[a3];
      msg_matrix[(total_num % number_of_data)][4] = ascii[a4];
    if(total_num % number_of_data == 0 && total_num != 0){
        clock_t begin_time = clock();
        md5_cal(msg_matrix, new_len, number_of_data, we_want_find, result);
        tISPC1 += float(clock() - begin_time);
        output(result);
        if(
          result[0] == 75 &&
          result[1] == 75 &&
          result[2] == 75 &&
          result[3] == 0 &&
          result[4] == 0
        )
        flag = true;
      }
  }}}}}

printf("The total time spend on running ISPC :\t%.3f seconds\n", tISPC1/CLOCKS_PER_SEC);
std::cout<<"\nispc output"<<std::endl;
output(result);

return 0;
}
