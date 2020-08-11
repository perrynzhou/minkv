/*************************************************************************
  > File Name: conn_queue_item.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:27:55 PM CST
 ************************************************************************/

#ifndef _CONN_QUEUE_ITEM_H
#define _CONN_QUEUE_ITEM_H
typedef enum queue_item_mode_t
{
  queue_new_conn = 0,
} queue_item_mode;
typedef struct queue_item_t
{
  int sfd;
  int init_state;
  struct queue_item_t *next;
  uint8_t   mode;
  int event_flags;
  void *ctx;
} queue_item;
queue_item *queue_item_create(int sfd, int state, int event_flags, void *ctx);
void queue_item_free(queue_item *item);
#endif
