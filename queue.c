/*************************************************************************
  > File Name: queue.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:25:36 2020
 ************************************************************************/

#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <assert.h>
static queue_item *queue_item_create(void *data)
{
  queue_item *item = (queue_item *)calloc(1, sizeof(queue_item));
  assert(item != NULL);
  item->data = data;
  item->next = NULL;
  return item;
}
static void queue_item_free(queue_item *item)
{
  free(item);
  item = NULL;
}
queue *queue_create()
{
  queue *q = (queue *)calloc(1, sizeof(queue));
  assert(q != NULL);
  q->head = q->tail = NULL;
  pthread_mutex_init(&q->lock, NULL);
  return q;
}
int queue_push(queue *q, void *data)
{

  queue_item *item = NULL;
  if (q != NULL && data != NULL)
  {
    item = queue_item_create(data);
    if (item == NULL)
    {
      return -1;
    }
    if (NULL == q->tail)
    {
      q->head = item;
      return 0;
    }
    q->tail->next = item;
    q->tail = item;
    __sync_fetch_and_add(&q->size, 1);
    return 0;
  }
  return -1;
}
void *queue_pop(queue *q)
{
  void *data = NULL;
  if (q != NULL && q->size > 0)
  {
    queue_item *item = q->head;
    if (NULL != item)
    {
      q->head = item->next;
      if (NULL == q->head)
      {
        q->tail = NULL;
      }
      data = item->data;
      queue_item_free(item);
      __sync_fetch_and_sub(&q->size, 1);
    }
  }
  return data;
}
void queue_cleanall(queue *q)
{
  if(q !=NULL)
  {
     for(size_t i=0;i<q->size;i++)
    {
      queue_pop(q);
    }
    q->head = q->tail = NULL;
    q->size=0;
  }
}
void queue_destroy(queue *q)
{
  if (q != NULL)
  {
    pthread_mutex_destroy(&q->lock);
     queue_cleanall(q);
    free(q);
    q = NULL;
  }
}
