#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include "md5_sequential.h"

const int number_of_data = 80000;
const int char_num = 3;
const int new_len = 56;

void output(uint8_t * output_data){
for(int i=0;i<16;i++)
printf("%x", output_data[i]);
}

int main() {
  md5_sequential md5_sequential_class;

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

  uint8_t store_sequential[16];
  uint8_t msg_matrix[5];
  uint8_t we_want_find[16];

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
  we_want_find[14] = 0x43;
  we_want_find[15] = 0xc6;
  //
  float tSerial = 0.0;
  bool flag = false;
  int total_num = 0;
  // seperate ascii code by chunk of max_num
  for(int a4 = 0;a4 < 63&&flag==false; a4++){
  for(int a3 = 0;a3 < 63&&flag==false; a3++){
  for(int a2 = 0;a2 < 63&&flag==false; a2++){
  for(int a1 = 0;a1 < 63&&flag==false; a1++){
  for(int a0 = 0;a0 < 63&&flag==false; a0++){
    total_num += 1;
    msg_matrix[0] = ascii[a0];
    msg_matrix[1] = ascii[a1];
    msg_matrix[2] = ascii[a2];
    msg_matrix[3] = ascii[a3];
    msg_matrix[4] = ascii[a4];
    int char_num =0;
    for(int q=0;q<5;q++)
    {
      if(msg_matrix[q]!= 0)
        {
        char_num += 1;
      }
    }

    clock_t begin_time = clock();
    md5_sequential_class.md5_sequential_calculate(msg_matrix, char_num, store_sequential);
    tSerial += float(clock() - begin_time);
    flag = true;
    for(int h=0;h<16;h++){
      if (store_sequential[h] != we_want_find[h])
        flag = false;
    }
  }
  }}}}

printf("The total time spend on running squential :\t%.3f seconds\n", tSerial/CLOCKS_PER_SEC);
std::cout<<"\nispc_Sequential output"<<std::endl;
output(store_sequential);


return 0;
}
