//
// Created by ayy on 3/24/17.
//

#ifndef OS_LAB6_LIST_H
#define OS_LAB6_LIST_H

typedef struct node {
    struct node *next;
    long id;
    int amount;
    int credit;
}node;

typedef struct list {
    node *head;
    int size;
}list;

node *newNode(long, int);
void initList(list *);
int emptyList(list *);
void push(list *, node *);
void pop(list *);
void removeItem(list *, long);
node *searchItem(list *, long);
void printList(list *);

#endif //OS_LAB6_LIST_H
