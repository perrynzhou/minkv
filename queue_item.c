/*************************************************************************
    > File Name: queue_item.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, August 11, 2020 PM03:45:40
 ************************************************************************/
#include "queue_item.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
queue_item *queue_item_create(int sfd, int state, int event_flags, void *ctx)
{
  queue_item *item = (queue_item *)calloc(1, sizeof(queue_item));
  assert(item != NULL);
  item->ctx = ctx;
  item->sfd = sfd;
  item->event_flags = event_flags;
  item->next = NULL;
  item->mode=queue_new_conn;
  item->init_state = state;
  return item;
}
void queue_item_free(queue_item *item)
{
  free(item);
  item = NULL;
}
