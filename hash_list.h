/*************************************************************************
  > File Name: hash_list.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Friday, July 03, 2020 PM02:24:02 HKT
 ************************************************************************/

#ifndef _HASH_LIST_H
#define _HASH_LIST_H
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
typedef int (*hash_list_traverse_cb)(void *, void *);
typedef int (*hash_list_data_free_cb)(void *);
typedef struct hash_list_t
{
  size_t max_size;
  size_t cur_size;
  void **arrays;
} hash_list;
hash_list *hash_list_create(size_t max_size);
int hash_list_insert(hash_list *list, uint32_t key, void *item);
void *hash_list_remove(hash_list *list, uint32_t key);
void *hash_list_search(hash_list *list, uint32_t key);
void hash_list_traverse(hash_list *list, hash_list_traverse_cb cb, void *ctx);
void hash_list_destroy(hash_list *list, hash_list_data_free_cb free_cb);
#endif
