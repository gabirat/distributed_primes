#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct node {
    uint64_t value;
    struct node* next;
} node;

typedef struct queue {
    node* head;
    node* tail;
} queue;

void init_queue(queue* q);
int is_empty(queue* q);
int enqueue(queue* q, uint64_t val);
uint64_t dequeue(queue* q);
size_t queue_length(queue* q);
void free_queue(queue* q);

#endif /* QUEUE_H */