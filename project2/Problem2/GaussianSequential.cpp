#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <chrono>

using namespace std;
typedef std::chrono::high_resolution_clock Clock;

double **newmatrix(int);
int gaussianelimination(double **,double *, double *, int);
void showmatrix(double **, int);

int main(int argc, char* argv[])
{
	float exe_time;
	srand((unsigned)time(0));

	int n;
	n = atoi(argv[1]);
	double **A = newmatrix(n);
	double *b = new double[n];
	double *y = new double[n];
	/*cout << "Please input the size of the matrix: ";
	cin >> n;
	while (n <= 0) {
		cout << "The size must be larger than 0. Try again!" << endl;
		cin >> n;
	}
	double **A = newmatrix(n);
	double *b = new double[n];
	double *y = new double[n];
	A[0][0] = 1; A[0][1] = 1; A[0][2] = 0;
	A[1][0] = 0; A[1][1] = 1; A[1][2] = 2;
	A[2][0] = 1; A[2][1] = 0; A[2][2] = -1;
	
	b[0] = 1;
	b[1] = 2;
	b[2] = 1;*/
	for (int i = 0; i < n; i++)
		b[i] = sqrt((double)rand());
	auto time_start = Clock::now();
	gaussianelimination(A, b, y, n);
	auto time_end = Clock::now();
	exe_time = (float) std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time).count();
	cout << y[0] << ", " << y[1] << ", " << y[2] << endl;
	//showmatrix(A, n);
	cout << "Execution time is " << exe_time/1e9 <<" seconds" << endl;

	delete A;
	delete b;
	delete y;
	return 0;
}

double **newmatrix(int size) {	
	double **x, **l, **u;
	
	//initiate the matrix
	x = new double*[size];
	for (int i = 0; i < size; i++) {
		x[i] = new double[size];
		for (int j = 0; j < size; j++) {
			x[i][j] = 0;
		}
	}

	l = new double*[size];
	for (int i = 0; i < size; i++) {
		l[i] = new double[size];
		for (int j = 0; j < size; j++) {
			l[i][j] = 0;
		}
	}

	u = new double*[size];
	for (int i = 0; i < size; i++) {
		u[i] = new double[size];
		for (int j = 0; j < size; j++) {
			u[i][j] = 0;
		}
	}

	//set random lower and upper trangular matrix 

	for (int i = 0; i < size; i++)
		for (int j = 0; j <= i; j++) {
			l[i][j] = sqrt(rand() % 100);
		}

	for (int i = 0; i<size; i++)
		for (int j = i; j < size; j++) {
			if (i == j)
				u[i][j] = 1;
			else
				u[i][j] = sqrt(rand() % 100);
		}

	//multiply the two matrix to get the final random matrix
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double partial = 0;
			for (int k = 0; k < size; k++) {
				partial += l[i][k] * u[k][j];
			}
			x[i][j] = partial;
		}
	}

	//print the matrix
	showmatrix(x, size);

	delete l;
	delete u;
	return x;
}

int gaussianelimination(double **A, double *b, double *y, int n) {
	for (int k = 0; k < n; k++)
	{
		for (int j = k+1; j < n; j++)
		{
			A[k][j] = A[k][j] / A[k][k];
		}
		y[k] = b[k] / A[k][k];
		A[k][k] = 1;
		for (int i = k+1; i < n; i++)
		{
			for (int j = k+1; j < n; j++)
			{
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
			b[i] = b[i] - A[i][k] * y[k];
			A[i][k] = 0;
		}
	}
	return 0;
}

void showmatrix(double **matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << setprecision(2) << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}
