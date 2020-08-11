/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:37:41 PM CST
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include "queue_item.h"
#include <pthread.h>
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
