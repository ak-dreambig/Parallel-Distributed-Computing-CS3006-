#include <stdio.h>
#include <stdlib.h>

struct task {
    int id;
    int priority;
    struct task *next;
};

struct queue {
    struct task **head;
    struct task **tail;
};

void enqueue(struct queue *q, struct task *t) {
    if (*(q->head) == NULL) {
        *(q->head) = t;
        *(q->tail) = t;
    }
    else {
        (*(q->tail))->next = t;
        *(q->tail) = t;
    }
}

struct task *dequeue(struct queue *q) {
    if (*(q->head) == NULL) {
        return NULL;
    }
    else {
        struct task *t = *(q->head);
        (q->head) = ((q->head))->next;
        return t;
    }
}