/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:25:31 2020
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include <pthread.h>
#include <stdint.h>
typedef struct queue_item_t
{
  struct queue_item_t *next;
  void *data;
} queue_item;
typedef struct queue_t
{
  queue_item *head;
  queue_item *tail;
  uint64_t size;
  pthread_mutex_t lock;
} queue;
queue *queue_create();
int queue_push(queue *q, void *data);
void *queue_pop(queue *q);
void queue_destroy(queue *q);
void queue_cleanall(queue *q);
#endif
