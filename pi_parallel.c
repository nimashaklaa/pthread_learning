#include <stdio.h>
#include <pthread.h>

// Define the number of terms and the number of threads
#define NUM_TERMS 1000000
#define NUM_THREADS 4

double sum = 0.0;  // Shared sum variable

/* It makes sure that when one thread is adding up its part of the calculation to the total, no other thread messes it up by trying to add their part at the same time.*/
pthread_mutex_t sum_mutex;  // Mutex for updating sum

typedef struct {
    int start;
    int end;
} ThreadData;

/*he program uses something called ThreadData to tell each thread which part of the Pi calculation to do.*/
void* thread_sum(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    double local_sum = 0.0;
    double factor = (data->start % 2 == 0) ? 1.0 : -1.0;

    for (int i = data->start; i < data->end; i++, factor = -factor) {
        local_sum += factor / (2 * i + 1);
    }

    pthread_mutex_lock(&sum_mutex);
    sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    /*ThreadData structure is used to pass necessary information to each thread. */
    ThreadData thread_data[NUM_THREADS];
    int terms_per_thread = NUM_TERMS / NUM_THREADS;

    // Initialize mutex
    pthread_mutex_init(&sum_mutex, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * terms_per_thread;
        thread_data[i].end = (i + 1) * terms_per_thread;
        pthread_create(&threads[i], NULL, thread_sum, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate final value of pi
    double pi = 4.0 * sum;
    printf("Approximated Pi (Parallel): %.12f\n", pi);

    // Destroy mutex
    /*program getting rid of the mutex after the calculation is done*/
    pthread_mutex_destroy(&sum_mutex);

    return 0;
}