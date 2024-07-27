#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/*Global variable: accessible to all threads*/
int thread_count;

/*Thread Function*/
void *Hello(void* rank);

int main(int argc, char* argv[]){
    /*Use long in case of a 64-bit system*/
    long thread;
    /*here thread_handles: has [thread_t] object which store our threads*/
    pthread_t* thread_handles;

    /*Get number of threads form command line*/
    thread_count = strtol(argv[1], NULL,10);

    thread_handles = malloc(thread_count* sizeof(pthread_t));

    for(thread =0; thread<thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
    }

    printf("Hello from the main thread\n");

    for(thread =0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread],NULL);
    }

    free(thread_handles);
    return 0;
}

void *Hello(void* rank){
    long my_rank =(long)rank;

    printf("Hello from thread %ld of %d\n", my_rank,thread_count);

    return NULL;
}