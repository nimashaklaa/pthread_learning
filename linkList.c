#include<stdio.h>
#include<stdlib.h>

//Define the structure for a node in the list
struct list_node_s{
    int data;
    struct list_node_s* next; //pointer to the next node in the linked list
};

//Function to check if a value is a member of the list
int Member(int value, struct list_node_s* head_p){
    //Start with the first node.
    struct list_node_s* curr_p = head_p;

    while (curr_p!=NULL && curr_p->data <value)
        curr_p = curr_p->next;

    if(curr_p==NULL || curr_p->data >value){
        return 0;
    }else{
        return 1;
    }
}

//Function to insert a new value into the list in sorted order
int Insert(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if(curr_p ==NULL ||curr_p->data>value){
        temp_p =(struct list_node_s*)malloc(sizeof(struct list_node_s));
        temp_p->data =value;
        temp_p->next=curr_p;
        if(pred_p ==NULL){
            *head_pp = temp_p;
        }else{
            pred_p->next =temp_p;
        }
        return 1;
    }else{
        return 0;
    }
}

// Function to delete a value from the list
int Delete(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p =*head_pp;
    struct list_node_s* pred_p =NULL;

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
//Print the List
void PrintList(struct list_node_s* head_p){
    struct list_node_s* curr_p =head_p;
    while(curr_p != NULL){
        printf("%d ", curr_p->data);
        curr_p= curr_p->next;
    };
    printf("\n");
}
//main function to test the linked list operations
int main(){
    struct list_node_s* head = NULL;
    Insert(10, &head);
    Insert(20,&head);
    Insert(15,&head);

    PrintList(head);

    Delete(15,&head);

    PrintList(head);

    if(Member(20,head)){
        printf("20 is in the list\n");
    }else{
        printf("20 is not in the list\n");
    }

    return 0;
}