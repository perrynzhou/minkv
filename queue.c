/*************************************************************************
    > File Name: queue.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, August 10, 2020 PM03:55:46
 ************************************************************************/

#include "queue.h"
#include <stdlib.h>
#include <assert.h>
queue *queue_create(){
    queue *q = (queue *)calloc(1,sizeof(queue));
    assert(q!=NULL);
    q->head = q->tail = NULL;
    pthread_mutex_init(&q->lock,NULL);
    return q;
}
void queue_push(queue *q, void *item)
{

  if (NULL == q->tail)
  {
    q->head = (queue_item *)item;
  }
  else
  {
    q->tail->next = item;
  }
  q->tail = item;
}
void *queue_pop(queue *q)
{
    queue_item *item = (queue_item *)q->head;
    if (NULL != item) {
        q->head = item->next;
        if (NULL == q->head)
            q->tail = NULL;
    }
    return item;
}
void queue_free(queue *q)
{
  if(q!=NULL)
  {
    pthread_mutex_destroy(&q->lock);
    q->head = q->tail = NULL;
    free(q);
    q=NULL;
  }
}
