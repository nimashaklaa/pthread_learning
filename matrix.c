#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 8
#define N 8
#define THREAD_COUNT 4

pthread_t threads[THREAD_COUNT];
int thread_count = THREAD_COUNT;

int A[M][N]; //Matrix
int x[N]; //Vector
int y[M]; //result vector

void *pth_mat_vect(void *rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_attr_t attr;

    //Initialize the matrix and vector with some values
    for (int i=0; i<M;i++){
        for (int j=0;j<N;j++){
            A[i][j] = i+j;
        }
    }

    for (int j=0;j<N;j++){
        x[j]=j;
    }

    pthread_attr_init(&attr);  //tells your program to get attr ready with default settings.
    
    /*This line changes one of the settings in attr.
    it tells the program that the threads we create using attr should be "joinable".
    "Joinable" means that we can wait for these threads to finish their work before our main program continues. This is done using pthread_join
    */
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //create threads
    for(thread = 0; thread< thread_count; thread ++){
        pthread_create(&threads[thread],&attr,pth_mat_vect,(void *)thread);
    }
    pthread_attr_destroy(&attr);

    //Join threads
    for(thread =0; thread<thread_count;thread++){
        pthread_join(threads[thread],NULL);
    }

    printf("Result vector y:\n");
    for (int i=0; i<M; i++){
        printf("%d",y[i]);
        printf("\n");
    }
    printf("\n");
    pthread_exit(NULL);
    return 0;
}

void *pth_mat_vect(void *rank){
    long my_rank = (long)rank;
    int i,j;
    int local_m = M/thread_count;
    int my_first_row = my_rank * local_m;
    int my_last_row =(my_rank+1)* local_m-1;

    for(i= my_first_row; i<=my_last_row; i++){
        y[i]=0;
        for (j=0;j<N;j++){
            y[i]+=A[i][j] *x[j];
        }
    }

    return NULL;
}