/*************************************************************************
  > File Name: queue_item.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:26:51 2020
 ************************************************************************/

#include<stdio.h>

#include "queue_item.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
queue_item *queue_item_create(size_t data_size)
{
  queue_item *item = (queue_item *)calloc(1, sizeof(queue_item)+sizeof(char)*data_size);
  assert(item != NULL);
  
  return item;
}
void queue_item_free(queue_item *item)
{
  free(item);
  item = NULL;
}
