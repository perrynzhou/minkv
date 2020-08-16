/*************************************************************************
  > File Name: queue_item.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:26:46 2020
 ************************************************************************/

#ifndef _QUEUE_ITEM_H
#define _QUEUE_ITEM_H
typedef struct queue_item_t
{
  void *data;
  struct queue_item_t *next;
} queue_item;
queue_item *queue_item_create(int sfd, int state, int event_flags, void *ctx);
void queue_item_free(queue_item *item);
#endif
