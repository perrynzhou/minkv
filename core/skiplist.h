/*************************************************************************
  > File Name: skiplist.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Friday, July 17, 2020 AM11:43:34 HKT
 ************************************************************************/

#ifndef _SKIPLIST_H
#define _SKIPLIST_H
typedef struct skiplist_node_t skiplist_node;
typedef void (*skiplist_node_data_cb)(void *);
typedef struct skiplist_t
{
  skiplist_node *head;
  size_t max_level;
  skiplist_node_data_cb *cb;
} skiplist;

skiplist *skiplist_create(skiplist_node_data_cb *cb);
int skiplist_init(skiplist *sl,skiplist_node_data_cb *cb);
int skiplist_push(skiplist *sl, int64_t key,void *data);
int skiplist_pop(skiplist *sl, int64_t key);
void *skiplist_find(skiplist *sl, int64_t key);
void cstl_skip_list_destroy(skiplist *sl);
void cstl_skip_list_deinit(skiplist *sl);
#endif
