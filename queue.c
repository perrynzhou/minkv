/*************************************************************************
    > File Name: queue.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, August 10, 2020 PM03:55:46
 ************************************************************************/

#include "queue.h"
#include "queue_item.h"
#include <stdio.h>
queue *queue_create(){

}
void queue_push(queue *cq, void *item)
{

  if (NULL == cq->tail)
  {
    cq->head = (queue_item *)item;
  }
  else
  {
    cq->tail->next = item;
  }
  cq->tail = item;
}
void *queue_pop(queue *cq)
{
    queue_item *item = (queue_item *)cq->head;
    if (NULL != item) {
        cq->head = item->next;
        if (NULL == cq->head)
            cq->tail = NULL;
    }
    return item;
}
void connection_queue_free(queue *cq)
{
}
