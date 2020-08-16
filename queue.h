/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:25:31 2020
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include <pthread.h>
#include "queue_item.h"
typedef struct queue_t {
    queue_item *head;
    queue_item *tail;
    pthread_mutex_t lock;
}queue;
queue *queue_create();
void queue_push(queue *q,void *item);
void *queue_pop(queue *q);
void queue_free(queue *q);
#endif
