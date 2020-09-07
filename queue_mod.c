#include "queue_mod.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * q queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

queue *queue_create(size_t max_size) {
    struct queue * ret = (struct queue *) malloc(sizeof(queue));
    ret->max_size = max_size;
    ret->size = 0;
    ret->tail = NULL;
    ret->head = NULL;
    pthread_cond_init(&ret->cv, NULL);
    pthread_mutex_init(&ret->m, NULL);
    return ret;
}

void queue_destroy(queue *q) {
    queue_node * destroy = q->tail;
    queue_node * temp = NULL;
    while (destroy) {
        temp = destroy;
        destroy = destroy->next;
        free(temp);
    }
    pthread_mutex_destroy(&(q->m));
    pthread_cond_destroy(&(q->cv));
    free(q);
}

void queue_push(queue *q, void *data) {

    pthread_mutex_lock(&q->m);

    while (q->max_size > 0 && q->size >= q->max_size) {
        pthread_cond_wait(&q->cv, &q->m);
    }
    queue_node * elem = (queue_node *) malloc(sizeof(queue_node));
    elem->data = data;
    elem->next = NULL;
    if (q->head != NULL) {
      q->head->next = elem;
    }
    q->head = elem;
    if (q->tail == NULL) {
      q->tail = elem;
    }
    q->size = q->size + 1;
    if (q->size > 0) {
        pthread_cond_broadcast(&q->cv);
    }
    pthread_mutex_unlock(&q->m);
}

void * queue_pull(queue *q) {
    pthread_mutex_lock(&q->m);
    while (q->size == 0) {
        pthread_cond_wait(&q->cv, &q->m);
    }


    void * d = q->tail->data;
    queue_node * temp = q->tail;
    q->tail = q->tail->next;
    free(temp);
    q->size--;
    if (q->tail == NULL) {
        q->head = NULL;
    }
    if ( q->size < q->max_size && q->max_size > 0) {
        pthread_cond_broadcast(&q->cv);
    }
    pthread_mutex_unlock(&q->m);
    return d;
}
