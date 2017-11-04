#include<iostream>
#include <ctime>
#include<cmath>
#include <cstdlib>
#include <iomanip>
#include<pthread.h>

using namespace std;

double **newmatrix(int);
int gaussianelimination(double **, double *, double *, int);
void showmatrix(double **, int);

double **A,*b,*y;



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
	showmatrix(u, size);

	return x;
}

struct work
{
	int columnstart;
	int columnend;
	int start;
	int end;
};

void *elimination(void *w) {
	work *a = (work*)w;
	for (int m = a->start; m <= a->end; m++)
	{
		for (int n = a->columnstart+1; n <= a->columnend; n++)
		{
			A[m][n] = A[m][n] - A[m][a->columnstart] * A[a->columnstart][n];
		}
		b[m] = b[m] - A[m][a->columnstart] * y[a->columnstart];
		A[m][a->columnstart] = 0;
	}

	return(NULL);
}

int gaussianelimination(double **A, double *b, double *y, int n) {
	for (int k = 0; k < n; k++)
	{
		for (int j = k + 1; j < n; j++)
		{
			A[k][j] = A[k][j] / A[k][k];
		}
		y[k] = b[k] / A[k][k];
		A[k][k] = 1;

		int threshold = 10;
		int thread_num = 4;
		int real_thread_num = (n - 1 - k) / threshold + 1;
		if (real_thread_num > 4)
		{
			real_thread_num = 4;
		}
		work *w = new work[real_thread_num];
		int temp = k + 1;
		for (int j = 0; j < real_thread_num; j++)
		{
			w[j].columnstart = k;
			w[j].columnend = n - 1;
			w[j].start = temp;
			if (j == real_thread_num - 1)
			{
				w[j].end = n - 1;
			}
			else
			{
				w[j].end = w[j].start + (n - 1 - k) / real_thread_num - 1;
			}			
			temp = w[j].end + 1;
		}

		pthread_t *threads = new pthread_t[real_thread_num];
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		for (int i = 0; i < real_thread_num; i++)
		{
			pthread_create(&threads[i], &attr, elimination, (void*)&w[i]);
		}
		for (int l = 0; l < real_thread_num; l++)
		{
			pthread_join(threads[l], NULL);
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

int main()
{
	srand((unsigned)time(0));

	int n;
	cout << "Please input the size of the matrix: ";
	cin >> n;
	while (n <= 0) {
		cout << "The size must be larger than 0. Try again!" << endl;
		cin >> n;
	}
	A = newmatrix(n);
	b = new double[n];
	y = new double[n];
	/*A[0][0] = 1; A[0][1] = 1; A[0][2] = 0;
	A[1][0] = 0; A[1][1] = 1; A[1][2] = 2;
	A[2][0] = 1; A[2][1] = 0; A[2][2] = -1;

	b[0] = 1;
	b[1] = 2;
	b[2] = 1;*/
	for (int i = 0; i < n; i++)
		b[i] = sqrt((double)rand());

	gaussianelimination(A, b, y, n);

	cout << y[0] << ", " << y[1] << ", " << y[2] << endl;
	showmatrix(A, n);

	return 0;
}