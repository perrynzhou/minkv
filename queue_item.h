/*************************************************************************
  > File Name: queue_item.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:26:46 2020
 ************************************************************************/

#ifndef _QUEUE_ITEM_H
#define _QUEUE_ITEM_H
#include <stdint.h>
typedef struct queue_item_t
{
  struct queue_item_t *next;
  uint8_t data[0];
} queue_item;
queue_item *queue_item_create(size_t data_size);
void queue_item_free(queue_item *item);
#endif
