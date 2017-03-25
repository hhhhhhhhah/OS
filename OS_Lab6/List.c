#include <stdio.h>
#include <stdlib.h>
#include "List.h"

node *newNode(long id, int amount){
    node *newnode = (node*) malloc(sizeof(node));
    newnode->id = id;
    newnode->amount = amount;
    newnode->credit = 0;
    newnode->next = NULL;
    return newnode;
}

void initList(list *l){
    l->head = NULL;
    l->size = 0;
}

int emptyList(list *l){
    return l->size == 0;
}

void push(list *l, node *node1){
    if(l->head == NULL){
        l->head = (node*) malloc(sizeof(node));
        l->head = node1;
        l->size++;
    }
    else{
        node *temp = l->head;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = (node*) malloc(sizeof(node));
        temp->next = node1;
        l->size++;
    }
}

void pop(list *l){
    if(emptyList(l)){
        printf("List is empty.\n");
        return;
    }
    else{
        node *temp = l->head->next;
        free(l->head);
        l->head = temp;
        l->size--;
        printf("Node was popped.\n");
    }
}

void removeItem(list *l, long id){
    node* temp = (node*) malloc(sizeof(node));
    node* current = l->head;
    if(emptyList(l)){
        printf("List is empty.\n");
        return;
    }
    if(current->id == id){
        pop(l);
        l->size--;
        return;
    }
    while(current != NULL){
        if(current->next != NULL && current->next->id == id){
            temp = current->next;
            current->next = temp->next;
            //temp->next = current;
            l->size--;
            free(temp);
            return;
        }
        current = current->next;
    }
    printf("No such ID.\n");
    free(temp);
    return;
}

node* searchItem(list *l, long id){
    node *current = l->head;
    if(emptyList(l)){
        printf("List is empty.\n");
        return NULL;
    }
    while(current != NULL){
        if(current->id == id){
            return current;
        }
        current = current->next;
    }
    printf("No such ID in the list.\n");
    return NULL;
}

void printList(list *l){
    node *current = l->head;
    if(emptyList(l)){
        printf("List is empty.\n");
        return;
    }
    while(current != NULL){
        printf("ID - %d\nAmount - %d\nDebt - %d\n---------------------\n",current->id,
               current->amount,current->credit);
        current = current->next;
    }
    return;
}