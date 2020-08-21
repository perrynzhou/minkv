/*************************************************************************
  > File Name: skiplist.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Friday, July 17, 2020 AM11:43:40 HKT
 ************************************************************************/

#include "skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */
struct skiplist_node_t
{
  int64_t key;
  void *data;
  size_t level;
  struct skiplist_node_t *next_nodes[0];
};
static int skiplist_random_level(void)
{
  int level = 1;
  while ((random() & 0xFFFF) < (SKIPLIST_P * 0xFFFF))
    level += 1;
  return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}
static skiplist_node *
skiplist_node_alloc(int64_t key, void *obj)
{
  skiplist_node *node = NULL;
  if (obj != NULL)
  {
    int level = skiplist_random_level();
    node = calloc(1, sizeof(skiplist_node) + sizeof(skiplist_node *) * level);
    node->level = level;
    node->data = obj;
    node->key = key;
    memset(node->next_nodes, 0, sizeof(skiplist_node *) * level);
  }
  return node;
}
static void skiplist_node_free(skiplist *sl,
                                    skiplist_node *node)
{
  if (sl != NULL && node != NULL)
  {
    if (sl->cb != NULL)
    {
      sl->cb(node->data);
    }
    if (node->next_nodes != NULL)
    {
      free(node->next_nodes);
    }
    free(node);
    node = NULL;
  }
}
int skiplist_init(skiplist *sl,skiplist_node_data_cb *cb)

{
  sl->head = calloc(1, sizeof(skiplist_node) +sizeof(skiplist_node *)*SKIPLIST_MAXLEVEL);
  if (sl->head == NULL)
  {
    return -1;
  }
  sl->head->data = NULL;
  sl->max_level = SKIPLIST_MAXLEVEL;
  sl->cb = cb;
  return 0;
}
skiplist *skiplist_create(skiplist_node_data_cb *cb)
{
  skiplist *sl = calloc(1, sizeof(skiplist));
  if (sl != NULL)
  {
    if (skiplist_init(sl, cb) != 0)
    {
      free(sl);
      sl = NULL;
    }
  }
  return sl;
}
int skiplist_push(skiplist *sl, int64_t key, void *data)
{
  skiplist_node *updates[sl->max_level];
  if (sl != NULL && data != NULL)
  {
    memset(&updates, 0, sizeof(skiplist_node *) * sl->max_level);
    skiplist_node *cur = sl->head;
    for (size_t i = sl->max_level - 1; i >= 0; i--)
    {
      if (cur->next_nodes[i] != NULL)
      {
        if (cur->next_nodes[i]->key > key)
        {
          updates[i] = cur;
        }
        else
        {
          while (cur->next_nodes[i]->key < key)
          {
            cur = cur->next_nodes[i];
          }
          updates[i] = cur;
        }
      }
    }
    skiplist_node *node = skiplist_node_alloc(key, data);
    for (size_t i = 0; i < node->level - 1; i++)
    {
      node->next_nodes[i] = updates[i]->next_nodes[i];
      updates[i]->next_nodes[i] = node->next_nodes[i];
    }
    return 0;
  }
  return -1;
}
int skiplist_pop(skiplist *sl, int64_t key)
{
  if (sl != NULL)
  {
    skiplist_node *cur = sl->head;
    skiplist_node *updates[sl->max_level];
    skiplist_node *res = NULL;
    for (size_t i = sl->max_level - 1; i >= 0; i--)
    {
      if (cur->next_nodes[i] != NULL)
      {
        if (cur->next_nodes[i]->key > key)
        {
          updates[i] = NULL;
        }
        else if (cur->next_nodes[i]->key < key)
        {
          cur = cur->next_nodes[i];
        }
        else
        {
          updates[i] = cur;
          res = cur->next_nodes[i];
        }
      }
    }
    if (res == NULL)
    {
      return -1;
    }
    skiplist_node *tmp = NULL;
    for (size_t i = 0; i < res->level; i++)
    {
      if (updates[i] != NULL)
      {
        tmp = updates[i]->next_nodes[i];
        updates[i]->next_nodes[i] = tmp->next_nodes[i];
      }
    }
    skiplist_node_free(sl, res);
  }
  return -1;
}
void *skiplist_find(skiplist *sl, int64_t key)
{
  void *obj = NULL;

  if (sl != NULL)
  {
    skiplist_node *node = sl->head;

    for (size_t i = sl->max_level - 1; i >= 0; i--)
    {
      if (node->next_nodes[i] != NULL && node->next_nodes[i]->key > key)
      {
        continue;
      }
      while (node->next_nodes[i] != NULL && node->next_nodes[i]->key < key)
      {
        node = node->next_nodes[i];
      }
      if (node->next_nodes[i]->key == key)
      {
        obj = node->next_nodes[i]->data;
        break;
      }
    }
  }
  return obj;
}
void cstl_skip_list_deinit(skiplist *sl)
{
  if (sl != NULL)
  {
    skiplist_node *temp = sl->head;
    for (size_t i = sl->max_level - 1; i >= 0; i--)
    {
      skiplist_node *cur = temp->next_nodes[i];
      while (cur != NULL)
      {
        skiplist_node *v = cur->next_nodes[i];
        skiplist_node_free(sl, cur);
        cur = v;
      }
    }
    skiplist_node_free(sl, temp);
    temp = NULL;
    sl->head = NULL;
  }
}
void cstl_skip_list_destroy(skiplist *sl)
{
  cstl_skip_list_deinit(sl);
  if (sl != NULL)
  {
    free(sl);
    sl = NULL;
  }
}
