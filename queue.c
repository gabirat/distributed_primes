#include "queue.h"

void init_queue(queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

void free_queue(queue* q) {
    node* tmp = q->head;
    while (q->head != NULL) {
        free(q->head);
        q->head = tmp->next;
    }
}

int is_empty(queue* q) {
    return q->head == NULL;
}

int enqueue(queue* q, uint64_t val) {
    node* new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) return -1;
    new_node->value = val;
    new_node->next = NULL;
    if (q->tail != NULL) {
        q->tail->next = new_node;
    }
    q->tail = new_node;
    if (is_empty(q)) {
        q->head = new_node;
    }
    return 0;
}

size_t queue_length(queue* q) {
    size_t len = 0;
    node* tmp = q->head;
    while (tmp != NULL) {
        tmp = tmp->next;
        len++;
    }
    return len;
}

//Always check with is_empty() before calling dequeue()
uint64_t dequeue(queue* q) {
   node* tmp = q->head;
   q->head = q->head->next;
   uint64_t val = tmp->value;
   if (is_empty(q)) {
       q->tail = NULL;
   } 
   free(tmp);
   return val;
}