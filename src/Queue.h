// FCFS Queue header file
#include <iostream>
#include "Instruction.h"

#ifndef QUEUE_H_
#define QUEUE_H_

struct QueueNode {
    Instruction * item;             // customer information
    struct QueueNode  *next;  // next customer in line; NULL if this is the last customer
};

struct Queue {
    struct QueueNode * head;    // Pointer to queue head: next node to be served
    struct QueueNode * tail;    // Pointer to queue tail: last node to be inserted
};

/*
 * Allocate memory for a node of type struct QueueNode and
 * initialize it with the item value and its arrival_time;
 * Return a pointer to the new node.
 */
struct QueueNode* CreateNode(Instruction * item) {
    struct QueueNode *newNode = (struct QueueNode *)malloc(sizeof(struct QueueNode)); // allocates new node
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

/*
 * Insert node with specified item and arrival time at the tail of the list.
 * If the queue is empty, then both the head and the tail should point to the new node
 * Should increment the arrival_count of the queue
 */
void Insert(struct Queue *q, Instruction * item) {
    struct QueueNode *newNode = CreateNode(item);
    // if the inserting item is the first one to be inserted
    if (q->head==NULL){
        q->head = newNode;
        q->tail = newNode;
        return;
    }
    q->tail->next = newNode;  //Inset after the last element (tail)
    q->tail = newNode;
}

/*
 * Delete node from head of the list and free its allocated memory
 * head pointer should move to the next node in the list. 
 * If head == NULL, do nothing
 * If the queue has one node, then both the head and the tail should point to NULL after the node is deleted
 * Should decrement the arrival_count of the queue
 */
Instruction * Delete (struct Queue *q) {
    if(q->head == NULL)  //if no element inside queue exit 
        return NULL;
    else if (q->head->next == NULL){ // if only one element inside queue assign head and tail to NULL and delete
        struct QueueNode *temp = q->head;
        q->head = NULL;
        q->tail = NULL;
        Instruction * return_item = temp->item;
        free(temp);          //free the node
        return return_item;  // return the deleted item
    }
    struct QueueNode *temp = q->head;
    q->head = q->head->next;

    Instruction * return_item = temp->item;
    free(temp);           //free the node
    return return_item;   //return the deleted item
}

/*
 * Count the current number of nodes in the queue.
 * Return 0 if the queue is empty, i.e., head == tail == NULL
 */
double CountNodes(struct Queue *q) {
    if(q->head == NULL)  //No element return 0
        return 0;
    int count = 0;
    struct QueueNode *iter = q->head;  //starts from head and count till the end (tail)
    while(iter!=NULL){
        iter = iter->next;
        count++;
    }
    return count;
}

// Free all the Nodes in the queue
void FreeNodes(struct Queue *q) {
    if (q->head == NULL) return;  // No elements to free

    struct QueueNode *iter = q->head;
    while (iter != NULL) {    // iterate to free all elements
        struct QueueNode *temp = iter;
       iter = iter->next; 
       free(temp);   //deallocates the space (memory)
    }
}

#endif
