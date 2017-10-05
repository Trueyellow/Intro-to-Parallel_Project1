//Kaixiang Huang demo program for calculating 20000000 random numbers' square root by newton method
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include "root_calculate.h"
#include "immintrin.h"
#include <algorithm>

using namespace ispc;
const int single_loop_num = 2000000;
const int total_loop_num = 10;

void root_calculate_sequential(float *data_points, float *ground_truth) {
	//Do square root calculation with newton method
	//The stop sign for each calculation is root - ground_truth >= 1e-4
	for (int i = 0; i < single_loop_num; i++) {
		float root = 2.82f;
		while (-(1e-4) <= (root - ground_truth[i]) <= 1e-4) {
			root = root - (root * root - data_points[i]) / (2 * root);
		}
	}
}

void root_calculate_avx(float const *data_points,  float const *ground_truth) {
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

int main() {

	float tISPC1 = 0.0, tISPC2 = 0.0, tSerial = 0.0;
	float tTASK[9];
	float number_of_sqrt[single_loop_num];
	float ground_truth[single_loop_num];

	for (int j = 0; j < single_loop_num; j++) {
		//Generate random float number between 0 to 8
		float data_point = float(rand()) / float (RAND_MAX / 8);
		number_of_sqrt[j] = data_point;
		ground_truth[j] = sqrt(data_point);
	}

	//add up all those time for single task ISPC
	clock_t begin_time = clock();
	for (int i = 0; i < total_loop_num; i++) {
		root_calculate(number_of_sqrt, ground_truth, single_loop_num);
	}
	tISPC1 += float(clock() - begin_time);


	//add up all those time for sequential calculation
	begin_time = clock();
	for (int i = 0; i < total_loop_num; i++) {
		root_calculate_sequential(number_of_sqrt, ground_truth);
	}
	tSerial += float(clock() - begin_time);

	//add up all those time for avx calculation
	begin_time = clock();
	for (int i = 0; i < total_loop_num; i++) {
		root_calculate_avx(number_of_sqrt, ground_truth);
	}
	tISPC2 += float(clock() - begin_time);

	for (int i = 0; i < total_loop_num; i++) {
	//add up all those time for multi-task ispc
	for(int number_of_threads = 1; number_of_threads < 9; number_of_threads++){
		begin_time = clock();
		root_task_wrap(number_of_sqrt, ground_truth, single_loop_num, number_of_threads);
		tTASK[number_of_threads] += float(clock() - begin_time);
		}
	}
	printf("The total time spend on running ISPC root:\t%.3f seconds\n", tISPC1/CLOCKS_PER_SEC);
	printf("The total time spend on running AVX root:\t%.3f seconds\n", tISPC2/CLOCKS_PER_SEC);
	printf("The total time spend on running squential root:\t%.3f seconds\n", tSerial/CLOCKS_PER_SEC);

	for(int i = 1; i < 9; i++){
				printf("The total time spend on running ISPC with %d thread:\t %.3f seconds\n",
				i, tTASK[i]/CLOCKS_PER_SEC);
	}

	return 0;
}
