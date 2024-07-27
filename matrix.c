#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 8
#define N 8
#define THREAD_COUNT 4

pthread_t threads[THREAD_COUNT];
int thread_count = THREAD_COUNT;

int A[M][N];
int x[N];
int y[M]; //result vector