#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

void *computation(){
    printf("Thread sample.\n");
    sleep(1);
    printf("Thread Ended.\n");
};

int main(){
    pthread_t thread1,thread2;

    pthread_create(&thread1,NULL,&computation,NULL);
    pthread_create(&thread2,NULL,&computation,NULL);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    return 0;
}

// void *computation(){
//     printf("Computation\n");
//     return NULL;
// }