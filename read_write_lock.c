#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_VALUE 65536  // 2^16

// Structure for node in the list
typedef struct list_node_s {
    int data;
    struct list_node_s* next;
} list_node_s;

// Read-Write Lock
// Global variables
list_node_s* head = NULL;  // Global head of the list
pthread_rwlock_t rwlock;   // Global read-write lock

// Function Prototypes
int Member(int value, list_node_s* head_p);
int Insert(int value, list_node_s** head_pp);
int Delete(int value, list_node_s** head_pp);
void PrintList(list_node_s* head_p);

// Thread function for operations
void* Thread_Operation(void* args) {
    long my_rank = (long) args; // Thread rank
    int m = 1000; // Total operations per thread
    int mMember = m * 0.5;
    int mInsert = m * 0.3;
    int mDelete = m - mMember - mInsert;
    int operation, val;

    srand(time(NULL) ^ my_rank); // Unique seed per thread

    while (mMember > 0 || mInsert > 0 || mDelete > 0) {
        operation = rand() % 3;
        val = rand() % MAX_VALUE;

        if (operation == 0 && mMember > 0) { // Member operation
            pthread_rwlock_rdlock(&rwlock);
            Member(val, head);
            pthread_rwlock_unlock(&rwlock);
            mMember--;
        } else if (operation == 1 && mInsert > 0) { // Insert operation
            pthread_rwlock_wrlock(&rwlock);
            if (Insert(val, &head) == 1) mInsert--;
            pthread_rwlock_unlock(&rwlock);
        } else if (operation == 2 && mDelete > 0) { // Delete operation
            pthread_rwlock_wrlock(&rwlock);
            if (Delete(val, &head) == 1) mDelete--;
            pthread_rwlock_unlock(&rwlock);
        }
    }
    return NULL;
}
int Member(int value, list_node_s* head_p) {
    struct list_node_s* current = head_p;
    while (current != NULL && current->data < value) {
        current = current->next;
    }
    if (current == NULL || current->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int Insert(int value, list_node_s** head_pp) {
    list_node_s* current = *head_pp;
    list_node_s* previous = NULL;
    list_node_s* new_node;

    while (current != NULL && current->data < value) {
        previous = current;
        current = current->next;
    }
    if (current == NULL || current->data > value) {
        new_node = malloc(sizeof(list_node_s));
        new_node->data = value;
        new_node->next = current;
        if (previous == NULL) {
            *head_pp = new_node;
        } else {
            previous->next = new_node;
        }
        return 1;
    }
    return 0;
}

int Delete(int value, list_node_s** head_pp) {
    list_node_s* current = *head_pp;
    list_node_s* previous = NULL;

    while (current != NULL && current->data < value) {
        previous = current;
        current = current->next;
    }
    if (current != NULL && current->data == value) {
        if (previous == NULL) {
            *head_pp = current->next;
        } else {
            previous->next = current->next;
        }
        free(current);
        return 1;
    }
    return 0;
}

void PrintList(list_node_s* head_p) {
    list_node_s* current = head_p;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}
int main() {
    int n = 100; // Initial number of unique elements
    long thread_count = 4; // Number of threads
    pthread_t* thread_handles;
    
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);

    // Initial list population
    for (int i = 0; i < n; i++) {
        Insert(rand() % MAX_VALUE, &head);
    }

    // Thread creation
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Thread_Operation, (void*) thread);
    }

    // Join threads
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    PrintList(head);
    pthread_rwlock_destroy(&rwlock);
    free(thread_handles);
    return 0;
}
