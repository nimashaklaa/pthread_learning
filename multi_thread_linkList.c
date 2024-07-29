#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREADS 2

// Define the structure for a node in the list
struct list_node_s {
    int data;
    struct list_node_s* next; // pointer to the next node in the linked list
};

// Global head of the list and mutex lock
struct list_node_s* head = NULL;
pthread_mutex_t list_mutex;

// Function to check if a value is a member of the list
int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}
// Function to insert a new value into the list in sorted order
int Insert(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    
    if (curr_p == NULL || curr_p->data > value) {
        temp_p = (struct list_node_s*)malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p; // Initialize next pointer of the new node

        if (pred_p == NULL) {
            *head_pp = temp_p;
        } else {
            pred_p->next = temp_p;
        }
        return 1;
    } else {
        return 0;
    }
}

// Function to delete a value from the list
int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
        } else {
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    } else {
        return 0;
    }
}


// Print the list
void PrintList(struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;
    while (curr_p != NULL) {
        printf("%d ", curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}
// Thread function to perform insertions
void* InsertThread(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&list_mutex);
        Insert(i * 10, &head);
        printf("Inserted: %d\n", i * 10);
        pthread_mutex_unlock(&list_mutex);
    }
    return NULL;
}

// Thread function to perform deletions
void* DeleteThread(void* arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&list_mutex);
        Delete(i * 10, &head);
        printf("Deleted: %d\n", i * 10);
        pthread_mutex_unlock(&list_mutex);
    }
    return NULL;
}

// Main function to test the linked list operations
int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&list_mutex, NULL);

    // Create threads
    pthread_create(&threads[0], NULL, InsertThread, NULL);
    pthread_create(&threads[1], NULL, DeleteThread, NULL);

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print final list
    pthread_mutex_lock(&list_mutex);
    printf("Final List: ");
    PrintList(head);
    pthread_mutex_unlock(&list_mutex);

    pthread_mutex_destroy(&list_mutex);
    return 0;
}