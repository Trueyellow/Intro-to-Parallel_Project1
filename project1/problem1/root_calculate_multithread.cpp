//Kaixiang Huang demo program for calculating 20000000 random numbers' square root by newton method
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include "root_calculate.h"
#include "immintrin.h"

const int NUMBER_OF_THREADS = 8;
const int SIZE = 20000000 / NUMBER_OF_THREADS;
typedef std::chrono::high_resolution_clock Clock;

float * number_of_sqrt = new float[SIZE];
float * ground_truth = new float[SIZE];

void *root_calculate_sequential(void *i) {
	//Do square root calculation with newton method
	//The stop sign for each calculation is root - ground_truth >= 1e-4
	for (int i = 0; i < SIZE; i++) {
		float root = 2.82f;
		while (!((-0.0001 <= (root - ground_truth[i])) && ((root - ground_truth[i]) <= -0.0001))) {
			root = root - (root * root - number_of_sqrt[i]) / (2 * root);
		}
	}

  pthread_exit(NULL);
}

int main(){

  pthread_t threads[NUMBER_OF_THREADS];

  for (int j = 0; j < SIZE; j++) {
    //Generate random float number between 0 to 8
    float data_point = float(rand()) / float (RAND_MAX / 8);
    number_of_sqrt[j] = data_point;
    ground_truth[j] = sqrt(data_point);
  }
  int rc;
  int i;

  auto begin_time = Clock::now();
  auto end_time = Clock::now();
  double tThread;
  for( i = 0; i < NUMBER_OF_THREADS; i++ ) {
        std::cout << "main() : creating thread, " << i << std::endl;
        begin_time = Clock::now();
        rc = pthread_create(&threads[i], NULL, root_calculate_sequential, NULL);
        if (rc) {
           std::cout << "Error:unable to create thread," << rc << std::endl;
           exit(-1);
        }
        // pthread_join(threads[i], NULL);
     }

  end_time = Clock::now();;
  tThread += (double) std::chrono::duration_cast<std::chrono::nanoseconds> (end_time - begin_time).count();
  printf("The total time spend on running %d threads root calculation:\t%.3f seconds\n", NUMBER_OF_THREADS, tThread/1e6);
}
