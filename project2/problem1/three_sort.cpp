//Kaixiang Huang project 2 problem 1
//because pthread is not friendly for class usage, so I am so sorry to write all these code in one program

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <pthread.h>
#include<bits/stdc++.h>
using namespace std;
typedef std::chrono::high_resolution_clock Clock;



// struct data to thread
typedef struct data_to_thread_bit{
    int low;
    int cnt;
    int flag;
} data_to_thread_bit;


typedef struct data_to_thread_quick{
    int l;
    int r;
} data_to_thread_quick;


typedef struct data_to_thread_radix {
    int *zero_bits;
    int *one_bits;
    unsigned *temp_pointer;
} data_to_thread_radix;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

data_to_thread_radix *radix_thread_arg;

// data to be sorted
int * data, *quick_data;
unsigned * unsigned_data;
int data_num, pthread_num;

//Global Active thread number
int now_thread_num = 0;


// definition of all functions
int asc(const void*,const void*);
int desc(const void*,const void*);
void compAndSwap(int, int, int);
void squential_merge(int, int, int);
void* MergeThreadFunction (void*);
void* compAndSwap_wrapper(void *);
void bitonicMerge(int, int, int);
void* thread_sort_wrapper(void *);
void sort_wrapper(int, int);
void bitonic_sequential(int, int, int);
void bitonic_sequential_merge(int, int, int);
void bitonicSort(int , int , int );
void parallel_quicksort(int, int);
void* quick_Sort_wrapper(void *);
int partition(int , int);
void QuickSort(int, int);
void *radix_sort(void *);
void radix_sort_wrapper();


// main
int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Usage: %s a b \n arg: a is the power of two which indicate the size of data.\n "
                       "arg: b is the power of two which indicates number of threads\n", argv[0]);
        exit(1);
    }

    data_num = pow(2, atoi(argv[1]));
    pthread_num = pow(2, atoi(argv[2]));

    int * random_data = new int[data_num];
    data = new int[data_num];

    for (int j = 0; j < data_num; j++) {
        //Generate random number
        random_data[j] = (int) float(rand()) / float (RAND_MAX / 1000);
        data[j] = random_data[j];
    }

    int sort_up = 1;   // means sort in ascending order
    auto begin_time = Clock::now();

    // if thread num == 1, implement the sequential version
    if(pthread_num == 1)
        bitonic_sequential(0, data_num, sort_up);
    else
        sort_wrapper(data_num, sort_up);

    auto end_time = Clock::now();
    double sort_time = (double) std::chrono::duration_cast<std::chrono::nanoseconds> (end_time - begin_time).count();
    printf("The total time spend on bitonicSort with data number: %d and thread number: %d is %.3f seconds\n",
           data_num, pthread_num, sort_time/1e9);

    quick_data = new int[data_num];
    for (int j = 0; j < data_num; j++) {
        //Reset data
        quick_data[j] = random_data[j];
    }

    // reset the number of activate thread
    now_thread_num = 0;

    begin_time = Clock::now();

    // if thread num == 1, implement the sequential version
    if(pthread_num == 1)
        QuickSort(0, data_num-1);
    else
        parallel_quicksort(0, data_num - 1);

    end_time = Clock::now();
    sort_time = (double) std::chrono::duration_cast<std::chrono::nanoseconds> (end_time - begin_time).count();
    printf("The total time spend on quick sort with data number: %d and thread number: %d is %.3f seconds\n",
           data_num, pthread_num, sort_time/1e9);

    unsigned_data = new unsigned[data_num];

    for (int j = 0; j < data_num; j++) {
        //Reset data
        unsigned_data[j] = (unsigned) random_data[j];
    }

    // reset the number of activate thread
    now_thread_num = 0;

    radix_thread_arg = new data_to_thread_radix[pthread_num];
    begin_time = Clock::now();

    // if thread num == 1, the pthread function automatically turn to a sequential version
    radix_sort_wrapper();
    end_time = Clock::now();
    sort_time = (double) std::chrono::duration_cast<std::chrono::nanoseconds> (end_time - begin_time).count();
    printf("The total time spend on radix sort with data number: %d and thread number: %d is %.3f seconds\n",
           data_num, pthread_num, sort_time/1e9);


//    printf("Sorted array: \n");
//    for (int i=0; i<data_num; i++)
//        printf("%d ", unsigned_data[i]);

    // garbage collection
    pthread_mutex_destroy(&m);
    delete[] data;
    delete[] random_data;
    delete[] unsigned_data;
    delete[] quick_data;

    return 0;
}

// Two functions for qsort function
int asc(const void* a, const void *b) {
    int* a1 = (int *)a;
    int* a2 = (int *)b;
    if ( *a1 < *a2 ) return -1;
    return *a1 != *a2;
}
int desc(const void* a,const void *b) {
    int* a1 = (int *)a;
    int* a2 = (int *)b;
    if ( *a1 > *a2 ) return -1;
    return *a1 != *a2;
}

// small function for compare and swap
void compAndSwap(int i, int j, int flag){
    if (flag==(data[i]>data[j]))
        swap(data[i],data[j]);
}

// Squential merge in case there would be thread traffic or the data is too small to call a thread
void squential_merge(int low, int cnt, int flag){

    int maxim = 0, left, right;

    int *temp_data = new int [cnt];

    for (int i=0; i<cnt; i++){
        temp_data[i] = data[low + i];
        if (data[low + i] > data[low + maxim] ) {
            maxim = i;
        }
    }


    left = maxim;
    right = maxim + 1;
    if (right == cnt ){
        right = 0;
    }
    if (flag == 0){
        for (int i=0; i<cnt; ++i) {
            if (temp_data[left] > temp_data[right]) {
                data[low + i] = temp_data[left--];
                if (left < 0)
                    left = cnt - 1;
            }
            else {
                data[low + i] = temp_data[right++];
                if (right == cnt )
                    right = 0;
            }
        }
    }
    else{
        for (int i=cnt; 0 < i; i--) {
            if (temp_data[left] > temp_data[right]) {
                data[low + i - 1] = temp_data[left--];
                if (left < 0) {
                    left = cnt - 1;
                }
            }
            else {
                data[low + i - 1] = temp_data[right++];
                if (right == cnt ) {
                    right = 0;
                }
            }
        }
    }
}


// Thread merge function wrapper
void* MergeThreadFunction ( void* arg ) {
    data_to_thread_bit * temp = (data_to_thread_bit *) arg;
    if (temp->cnt > 1) {
        bitonicMerge(temp->low, temp->cnt, temp->flag );
    }
}

// thread compare and swap function wrapper
void *compAndSwap_wrapper(void *arg){
    data_to_thread_bit * temp = (data_to_thread_bit *) arg;
    for(int i=temp->low; i<(temp->low + temp->cnt); i++ ) {
        compAndSwap(i,  i + 2*temp->cnt, temp->flag);
    }
}

// bitonic merge function
void bitonicMerge(int low, int cnt, int flag)
{
    // mutex lock to access to thread number counter
    pthread_mutex_lock (&m);
    if (cnt > 32768 && now_thread_num < pthread_num ) {
        now_thread_num++;
        pthread_mutex_unlock (&m);

        // unlock
        int k = cnt/2;

        // set thread data
        data_to_thread_bit data_compAndSwap;
        pthread_t compAndSwap_thread;
        data_compAndSwap.low = low;
        data_compAndSwap.cnt = k/2;
        data_compAndSwap.flag = flag;

        // create a thread and a sequential one to speed up
        pthread_create(&compAndSwap_thread, NULL, compAndSwap_wrapper, &data_compAndSwap);
        for (int i= (low + k/2); i<low+k; i++)
            compAndSwap(i, i+k, flag);
        pthread_join(compAndSwap_thread, NULL );

        pthread_mutex_lock (&m);
        --now_thread_num;
        pthread_mutex_unlock (&m);

        pthread_t merge_thread;

        // merge thread data
        data_to_thread_bit merge_data;
        merge_data.low = low;
        merge_data.cnt = k;
        merge_data.flag = flag;

        // create a thread and a sequential one to speed up
        pthread_create(&merge_thread, NULL, MergeThreadFunction, &merge_data );
        bitonicMerge(low+k, k, flag);
        pthread_join(merge_thread, NULL );

        pthread_mutex_lock(&m);
        --now_thread_num;
        pthread_mutex_unlock(&m);

    }
    else{
        // If there is no more threads or there is no need to call a thread to do merge operation
        pthread_mutex_unlock(&m);
        squential_merge(low, cnt, flag);

    }
}

// sort thread wrapper
void * thread_sort_wrapper(void *arg){
    data_to_thread_bit * tem = (data_to_thread_bit *) arg;
    if(tem->cnt > 1){
        bitonicSort(tem->low, tem->cnt, tem->flag);
    }
}

// bitonic sort function
void bitonicSort(int low, int cnt, int flag)
{
    if (cnt>1)
    {
        int k = cnt/2;

        //lock to check for thread number
        pthread_mutex_lock (&m);
        if (now_thread_num < pthread_num ) {
            now_thread_num++;
            pthread_mutex_unlock(&m);

            // prepare thread data
            data_to_thread_bit thread_data;
            pthread_t sort_thread;
            thread_data.flag = 1;
            thread_data.low = low;
            thread_data.cnt = k;

            pthread_create (&sort_thread, NULL, thread_sort_wrapper, &thread_data);
            bitonicSort(low + k, k, 0);
            pthread_join(sort_thread, NULL);

            pthread_mutex_lock(&m);
            --now_thread_num;
            pthread_mutex_unlock(&m);
        }
        else{
            // if there is no more thread, we will use a sort
            pthread_mutex_unlock(&m);
            qsort(data + low, cnt, sizeof(int), flag==1 ? asc:desc);
            return;
        }

        bitonicMerge(low, cnt, flag);
    }
}

// wrapper for the whole sort
void sort_wrapper(int N, int up)
{
    now_thread_num = 1;
    bitonicSort(0, N, up);
}

void bitonic_sequential(int low, int cnt, int flag){
    if (cnt>1)
    {
        int k = cnt/2;
        bitonic_sequential(low, k, 1);
        bitonic_sequential(low + k, k, 0);
        bitonic_sequential_merge(low, cnt, flag);
    }
}

void bitonic_sequential_merge(int low, int cnt, int flag){

    int k = cnt/2;
    for (int i= low; i<low+k; i++)
        compAndSwap(i, i+k, flag);
    bitonicMerge(low, k, flag);
    bitonicMerge(low+k, k, flag);
}


// function for sequential quick sort
void QuickSort(int top, int bottom)
{
    // top = subscript of beginning of array
    // bottom = subscript of end of array
    if (top < bottom)
    {
        int middle = partition(top, bottom);
        QuickSort(top, middle-1);   // sort first section
        QuickSort(middle+1, bottom);    // sort second section
    }
}


// Function to determine the partitions
// partitions the array and returns the middle subscript
int partition(int top, int bottom)
{
    int pivot = top + (bottom - top) / 2;
    int pivotV = quick_data[pivot];
    swap(quick_data[pivot], quick_data[bottom]);
    int sl = top;
    for (int i=top ; i<bottom ; i++)
    {
        if (quick_data[i] <= pivotV)
        {
            swap(quick_data[i], quick_data[sl]);
            sl++;
        }
    }
    swap(quick_data[sl], quick_data[bottom]);
    return sl;
}


// wrapper of quick sort
void* quick_Sort_wrapper(void *arg)
{
    data_to_thread_quick *start = (data_to_thread_quick*) arg;
    parallel_quicksort(start->l, start->r);
}

/*
IN FACT, quick sort is the slowest one consider the limit number of threads and
the recursive calling of new sort function, here I use a small qsort function to keep the efficiency of our program
*/

// true quick sort
void parallel_quicksort(int l, int r)
{
    if(r > l)
    {
        int middle = partition(l, r);
        pthread_mutex_lock (&m);

        // Consider the efficency of quick sort, I here set a threshold that if the size of data to sort is too small
        // to create a new thread
        if (now_thread_num < pthread_num && ((middle - 1 - l) > 512 || (r - middle - 1) > 512))
        {
            now_thread_num++;
            pthread_mutex_unlock (&m);

            data_to_thread_quick arg = {l, middle-1};
            pthread_t thread;
            pthread_create(&thread, NULL, quick_Sort_wrapper, &arg);
            parallel_quicksort(middle+1, r);
            pthread_join(thread, NULL);

            pthread_mutex_lock (&m);
            now_thread_num --;
            pthread_mutex_unlock (&m);
        }
        else{
            pthread_mutex_unlock (&m);

            // may be some bound problem(qsort function for efficiency)
            if(middle - 1 - l > 0)
                qsort(quick_data + l, middle - 1 - l, sizeof(int), asc);

            if(r - middle > 0)
                qsort(quick_data + middle, r - middle, sizeof(int), asc);

            return;
        }
    }
}


// wrapper for radix sort
void radix_sort_wrapper(){

    unsigned *temp =  new unsigned[data_num];
    int * zero = new int[pthread_num];
    int * ones = new int[pthread_num];

    pthread_t radix_thread[pthread_num];
    // barrier initialization
    pthread_barrier_init(&barrier, NULL, (unsigned)pthread_num);

    // for each thread, we have a set of args(like a kind of global varibles)
    for(int i=0; i<pthread_num; i++)
    {
        radix_thread_arg[i].temp_pointer = temp;
        radix_thread_arg[i].one_bits = ones;
        radix_thread_arg[i].zero_bits = zero;
        int * point = new int;
        *point = i;
        pthread_create(&radix_thread[i], NULL, radix_sort, point);
    }

    for (int i = 0; i < pthread_num; i++ ) {
        pthread_join(radix_thread[i], NULL);
    }

    delete[] temp;
    pthread_barrier_destroy(&barrier);
}

void *radix_sort(void *arg){
    int thread_num = *((int *) arg);

    unsigned *t_pointer = radix_thread_arg[thread_num].temp_pointer;

    int * zero_num = radix_thread_arg[thread_num].zero_bits;
    int * one_num = radix_thread_arg[thread_num].one_bits;

    int work_distribute = ceil(data_num/pthread_num);
    int start = work_distribute * thread_num;

    // set end point, but consider the bound problem
    int end = (thread_num+1)!=pthread_num ? work_distribute*(thread_num + 1):data_num;


    for(int bits=0; bits<29; bits++){

        zero_num[thread_num] = 0;

        for(int k = start; k<end; k++)
            if(((unsigned_data[k]>>bits) & 1) == 0)
                zero_num[thread_num]++;
        one_num[thread_num] = work_distribute - zero_num[thread_num];

        pthread_barrier_wait(&barrier);

        // calculate the number of counts in each distributed data block, then add then up for different thread
        // to do the data arrangement in different thread concurrently
        int zero_count=0, index_count=0;
        for(int i = 0; i<thread_num;i++)
        {
            zero_count += zero_num[i];
            index_count += one_num[i];
        }

        index_count += zero_count;

        for (int i = thread_num ; i < pthread_num; i++ ) {
            index_count += zero_num[i];
        }
        // barrier to make sure all index_count and zero_count is set correctly
        pthread_barrier_wait(&barrier);

        // set up the new shape based on the radix sort's result
        for(int i =start; i<end;i++){
            if(((unsigned_data[i]>>bits) & 1) == 0)
                t_pointer[zero_count++] = unsigned_data[i];
            else
                t_pointer[index_count++] = unsigned_data[i];
        }

        // exchange data from temp to source data
        for(int i=start; i<end; i++)
            unsigned_data[i] = t_pointer[i];
    }
}
