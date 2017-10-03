//Kaixiang Huang demo program for calculating 20000000 random numbers' square root by newton method
#include <stdio.h>      
#include <math.h> 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>   

const int single_loop_num = 20000;
const int total_loop_num = 1000;

void root_calculate(float *data_points, float *ground_truth) {
	//Do square root calculation with newton method
	//The stop sign for each calculation is root - ground_truth >= 1e-4
	for (int i = 0; i < single_loop_num; i++) {
		float root = 2.82f;
		while (-(1e-4) <= (root - ground_truth[i]) <= 1e-4) {
			root = root - (root * root - data_points[i]) / (2 * root);
		}
	}
}

float calculation() {
	float number_of_sqrt[single_loop_num];
	float ground_truth[single_loop_num];
	for (int j = 0; j < single_loop_num; j++) {
		//Generate random float number between 0 to 8
		float data_point = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 8));
		number_of_sqrt[j] = data_point;
		ground_truth[j] = sqrt(data_point);
	}
	//return calculation time
	const clock_t begin_time = clock();
	root_calculate(number_of_sqrt, ground_truth);
	return float(clock() - begin_time);
}

void main() {
	srand(time(NULL));
	float time = 0;
	for (int i = 0; i < total_loop_num; i++) {
		//add up all those time
		time += calculation();
	}
	std::cout << "The total time spend to run "<< 20000000 <<" times of newton sqrt, is "<< time / CLOCKS_PER_SEC << " seconds";
}
