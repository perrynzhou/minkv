/*************************************************************************
  > File Name: list.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:38:55 HKT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include "object.h"
#include <pthread.h>
typedef void (*list_node_data_handle)(void *ctx, void *arg1);
typedef int (*list_node_data_cmp)(void *data1, void *data2);
typedef struct list_node_t
{
  void *data;
  struct list_node_t *next;
  struct list_node_t *prev;
}list_node;
typedef struct list_t
{
  list_node dummy;
  size_t cur_size;
  size_t max_size;
} list;

list *list_create(size_t max_size);
int list_init(list *lt, size_t max_size);
int list_push_back(list *lt, void *obj);
int list_push_front(list *lt, void *obj);
void *list_pop_back(list *lt);
void *list_pop_front(list *lt);
void *list_search(list *lt, list_node_data_cmp cmp, void *data2);
int list_traverse(list *lt, list_node_data_handle cb, void *ctx);
void list_free(list *lt);
void list_deinit(list *lt);

#endif
