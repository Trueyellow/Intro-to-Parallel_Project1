//Kaixiang Huang AVX program for calculating 20000000 random numbers' square root by newton method
#include <stdio.h>      
#include <math.h> 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>   
#include "immintrin.h"


const int single_loop_num = 20000;
const int total_loop_num = 1000;

void root_calculate(float const *data_points,  float const *ground_truth) {
	//Do square root calculation with newton method
	//The stop sign for each calculation is root - ground_truth >= 1e-4
	__m256 zero_re = _mm256_set1_ps(0.0f);
	__m256 two_re = _mm256_set1_ps(2.0f);
	__m256 root_re = _mm256_set1_ps(2.82f);
	__m256 to_compare_1_re = _mm256_set1_ps(1e-4);
	__m256 to_compare_2_re = _mm256_set1_ps(-1e-4);
	//assign each 8 elements to AVX to speed up
	for (int i = 0; i < single_loop_num; i+=8) {
		int while_flag = 1;
		__m256 true_result = _mm256_loadu_ps(&ground_truth[i]);
		__m256 data = _mm256_loadu_ps(&data_points[i]);
		__m256 new_root_re = root_re;
		__m256 calculation_mask;
		while(while_flag != 0) {
			//Use newton method to calculate result 
			new_root_re = _mm256_sub_ps(new_root_re, 
				_mm256_div_ps(_mm256_sub_ps(_mm256_mul_ps(new_root_re, new_root_re), data), _mm256_mul_ps(two_re, new_root_re)));
			
			//Generate compare mask
			__m256 compare = _mm256_sub_ps(new_root_re, true_result);
			__m256 calculate_mask_1 = _mm256_cmp_ps(compare, to_compare_1_re, 2);
			__m256 calculate_mask_2 = _mm256_cmp_ps(compare, to_compare_2_re, 13);
			calculation_mask = _mm256_cmp_ps(_mm256_and_ps(calculate_mask_1, calculate_mask_2), zero_re, _CMP_EQ_OQ);
			// A flag for compare difference between ground truth and our result calculated by newton method
			while_flag = _mm256_movemask_ps(calculation_mask);
		}
	}
}

//function to generate random number between 0 to 8
float calculation() {
	float number_of_sqrt[single_loop_num];
	float ground_truth[single_loop_num];
	for (int j = 0; j < single_loop_num; j++) {
		//Generate random float number between 0 to 8
		float data_point = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 8));
		number_of_sqrt[j] = data_point;
		ground_truth[j] = sqrt(data_point);
	}
	//time counter for calculation part
	const clock_t begin_time = clock();
	root_calculate(number_of_sqrt, ground_truth);
	return float(clock() - begin_time);
}

//main
void main() {
	
	srand(time(NULL));
	float time = 0;
	
	for (int i = 0; i < total_loop_num; i++) {
		//get calculation time returned by function 
		time += calculation();
	}
	std::cout << "The total time spend to run " << 20000000 << " times of newton sqrt, is " <<  time / CLOCKS_PER_SEC << " seconds";
	system("pause");
}