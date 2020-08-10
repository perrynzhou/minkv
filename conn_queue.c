/*************************************************************************
    > File Name: conn_queue.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, August 10, 2020 PM03:55:46
 ************************************************************************/

#include "conn_queue.h"
#include "conn_item.h"
#include <stdio.h>
conn_queue *conn_queue_create();
void conn_queue_push(conn_queue *cq, void *item)
{

  if (NULL == cq->tail)
  {
    cq->head = (conn_queue_item *)item;
  }
  else
  {
    cq->tail->next = item;
  }
  cq->tail = item;
}
void *conn_queue_pop(conn_queue *cq)
{
    conn_queue_item *item = (conn_queue_item *)cq->head;
    if (NULL != item) {
        cq->head = item->next;
        if (NULL == cq->head)
            cq->tail = NULL;
    }
    return item;
}
void conn_queue_free(conn_queue *cq)
{
}
