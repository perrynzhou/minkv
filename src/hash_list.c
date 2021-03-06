/*************************************************************************
  > File Name: hash_list.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Friday, July 03, 2020 PM02:24:10 HKT
 ************************************************************************/

#include "hash_list.h"
#include "hashfn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct hash_list_node_t
{
  struct hash_list_node_t *next;
  uint32_t key;
  void *data;
} hash_list_node;
static hash_list_node *hash_list_node_create(uint32_t key, void *data)
{
  hash_list_node *n = (hash_list_node *)calloc(1, sizeof(*n));
  n->next = NULL;
  n->data = data;
  n->key = key;
  return n;
}
static void hash_list_node_destroy(hash_list_node *node, bool flag)
{
  if (flag)
  {
    free(node->data);
  }
  free(node);
  node = NULL;
}
int hash_list_insert(hash_list *list, uint32_t key, void *item)
{
  uint32_t index = hash_jump_consistent(key, list->max_size);
  hash_list_node *node = hash_list_node_create(key, item);
  if (list->arrays[index] == NULL)
  {
    list->cur_size++;
    list->arrays[index] = node;
    return 0;
  }
  hash_list_node *first = (hash_list_node *)list->arrays[index];
  hash_list_node *cur = first;

  hash_list_node *prev = NULL;
  int ret = 0;
  while (cur != NULL)
  {
    if (key == cur->key)
    {
      hash_list_node_destroy(node, false);
      ret = -1;
      break;
    }
    cur = cur->next;
  }
  __sync_fetch_and_add(&list->cur_size, 1);
  node->next = first;
  list->arrays[index] = node;
  return 0;
}
hash_list *hash_list_create(size_t max_size,hash_list_data_cmp_cb  cmp)
{
  hash_list *lt = (hash_list *)calloc(1, sizeof(*lt));
  lt->max_size = max_size;
  lt->cur_size = 0;
  lt->arrays = (void **)calloc(max_size, sizeof(void *));
  lt->cmp = cmp;
  return lt;
}
void *hash_list_search(hash_list *list, uint32_t key,void *data)
{
  void *data = NULL;
  uint32_t index = hash_jump_consistent(key, list->max_size);
  if (list->arrays[index] != NULL)
  {
    hash_list_node *cur = list->arrays[index];
    while (cur != NULL)
    {
      hash_list_node *next = cur->next;
      if (cur->key == key && list->cmp(cur->data,data)==0)
      {
        data = cur->data;
        break;
      }
      cur = next;
    }
  }
  return data;
}
void *hash_list_remove(hash_list *list, uint32_t key,void *data)
{
  void *data = NULL;

  if (list != NULL)
  {
    uint32_t index = hash_jump_consistent(key, list->max_size);
    if (list->arrays[index] != NULL)
    {

      hash_list_node *cur = list->arrays[index];
      hash_list_node *prev = NULL;
      while (cur != NULL)
      {
        hash_list_node *next = cur->next;
        if (cur->key == key && list->cmp(cur->data,data)==0)
        {
          data = cur->data;
          break;
        }
        else
        {
          prev = cur;
        }
        cur = next;
      }
      if (prev != NULL)
      {
        prev->next = cur->next;
      }
      else
      {
        list->arrays[index] = cur->next;
      }
      if (cur != NULL)
      {
        __sync_fetch_and_sub(&list->cur_size, 1);
        hash_list_node_destroy(cur, false);
      }
    }
  }
  return data;
}
void hash_list_traverse(hash_list *list, hash_list_traverse_cb cb, void *ctx)
{
  if (list != NULL && cb != NULL)
  {
    for (size_t i = 0; i < list->max_size; i++)
    {
      hash_list_node *cur = list->arrays[i];
      while (cur != NULL)
      {
        void *data = cur->data;
        cb(ctx, data);
        cur = cur->next;
      }
    }
  }
}
void hash_list_destroy(hash_list *list, hash_list_data_free_cb free_cb)
{
  if (list != NULL)
  {
    for (size_t i = 0; i < list->max_size; i++)
    {
      hash_list_node *cur = list->arrays[i];
      while (cur != NULL)
      {
        hash_list_node *next = cur->next;
        if (free_cb != NULL)
        {
          free_cb(cur->data);
        }
        hash_list_node_destroy(cur, false);
        cur = next;
        list->cur_size--;
      }
    }
    if (list->arrays != NULL)
    {
      free(list->arrays);
      list->arrays = NULL;
    }
    free(list);
  }
}