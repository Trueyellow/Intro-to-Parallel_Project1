//Written by HKX to launch Sequential c++ code for md5 algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include "md5_sequential.h"
#include <chrono>

const int char_num = 3;
const int new_len = 56;
typedef std::chrono::high_resolution_clock Clock;

//print function
void output(uint8_t * output_data){
for(int i=0;i<16;i++)
printf("%x", output_data[i]);
}

// The md5 value that we should find out
const uint8_t we_want_find[16] =
{
  0x5c, 0xe8, 0xb4, 0xe3, 0x3c, 0x23, 0x62, 0x21,
  0x3d, 0xc2, 0x2f, 0x6c, 0x90, 0x8a, 0x44, 0xc6
};

int main() {
  // Call to md5_sequential.h
  md5_sequential md5_sequential_class;

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

  uint8_t store_sequential[16];
  uint8_t msg_matrix[5];
  uint8_t we_want_find[16];

  double tSerial = 0.0;
  bool flag = false;
  int total_num = 0;

  // seperate ascii code by chunk of max_num
  for(int a4 = 0;a4 < 63&&flag==false; a4++){
  for(int a3 = 0;a3 < 63&&flag==false; a3++){
  for(int a2 = 0;a2 < 63&&flag==false; a2++){
  for(int a1 = 0;a1 < 63&&flag==false; a1++){
  for(int a0 = 0;a0 < 63&&flag==false; a0++){

    auto begin_time = Clock::now();
    total_num += 1;
    // Assign ascii code to send into function
    msg_matrix[0] = ascii[a0];
    msg_matrix[1] = ascii[a1];
    msg_matrix[2] = ascii[a2];
    msg_matrix[3] = ascii[a3];
    msg_matrix[4] = ascii[a4];

    // calculate char number of ascii code
    int char_num =0;
    for(int q=0;q<5;q++)
      if(msg_matrix[q]!= 0)
        char_num += 1;

    // time counter

    md5_sequential_class.md5_sequential_calculate(msg_matrix, char_num, store_sequential);


    flag = true;
    for(int h=0;h<16;h++)
      if (store_sequential[h] != we_want_find[h])
        flag = false;
    if(total_num % 10000000 == 0)
        std::cout<< tSerial <<std::endl;

    auto end_time = Clock::now();
    tSerial += (double) std::chrono::duration_cast<std::chrono::nanoseconds> (end_time - begin_time).count();


     }


    }
   }
  }
 }

printf("The total time spend on running squential :\t%.3f seconds\n", double(tSerial / 1e9) );
std::cout<<"\nispc_Sequential output"<<std::endl;
output(store_sequential);
return 0;
}
