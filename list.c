/*************************************************************************
  > File Name: list.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:39:00 HKT
 ************************************************************************/

#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static list_node *list_node_create(void *data)
{
  list_node *node = NULL;
  if (data != NULL)
  {
    node = calloc(1, sizeof(list_node));
    assert(node != NULL);
    node->next = node->prev = NULL;
    node->data = data;
  }
  return node;
}
static int list_node_destroy(list *lt, list_node *node)
{
  if (node != NULL)
  {
    node->prev = node->next = NULL;
    free(node);
  }
}
list *list_create(size_t max_size)
{

  list *list = calloc(1, sizeof(list));
  if (list_init(list, max_size) != 0)
  {
    return NULL;
  }
  return list;
}
int list_init(list *lt, size_t max_size)
{
  if (lt != NULL)
  {

    // list->dummy.prev  pointer list head
    // list->dummy.next pointer list tail
    lt->dummy.next = lt->dummy.prev = NULL;
    lt->cur_size = 0;
    max_size = (max_size < 4) ? (4) : (max_size);
    lt->max_size = max_size;
    return 0;
  }
  return -1;
}
int list_insert(list *lt, int pos, void *data)
{
  if (lt != NULL && data != NULL)
  {
    return -1;
  }

  list_node *node = list_node_create(data);
  assert(node != NULL);

  if (lt->dummy.next == NULL)
  {
    lt->dummy.next = lt->dummy.prev = node;
  }
  else
  {
    if (pos >= 0)
    {
      list_node *head = lt->dummy.prev;
      node->next = head;
      head->prev = node;
    }
    else
    {
      list_node *tail = lt->dummy.next;
      tail->next = node;
      node->prev = tail;
      lt->dummy.next = node;
    }
  }
  lt->cur_size++;
  return 0;
}
int list_push_back(list *lt, void *data)
{
  return list_insert(lt, -1, data);
}
int list_push_front(list *lt, void *data)
{
  return list_insert(lt, 0, data);
}
list_node *list_remove(list *lt, int pos)
{
  list_node *node = NULL;

  if (lt != NULL && lt->cur_size > 0)
  {
    if (pos >= 0)
    {
      node = lt->dummy.prev;
      if (node == lt->dummy.next)
      {
        lt->dummy.prev = lt->dummy.next = NULL;
      }
      else
      {
        lt->dummy.prev = node->next;
      }
    }
    else
    {
      node = lt->dummy.next;
      if (node == lt->dummy.prev)
      {
        lt->dummy.prev = lt->dummy.next = NULL;
      }
      else
      {
        lt->dummy.next = node->prev;
      }
    }
    lt->cur_size--;
  }
  return node;
}
void *list_pop_back(list *lt)
{
  return list_remove(lt, -1);
}
void *list_pop_front(list *lt)
{
  return list_remove(lt, 0);
}

int list_traverse(list *lt, list_node_data_handle cb, void *ctx)
{
  list_node *node = lt->dummy.prev;
  for (; node != NULL; node = node->next)
  {
    cb(ctx, node->data);
  }
}
void list_deinit(list *lt)
{
  if (lt != NULL)
  {
    list_node *node = lt->dummy.prev;
    for (; node != NULL; node = node->next)
    {
      list_node_free(lt, node);
    }
  }
}
void list_destroy(list *lt)
{
  list_deinit(lt);
  if (lt != NULL)
  {
    free(lt);
    lt = NULL;
  }
}
void *list_search(list *lt, list_node_data_cmp cmp, void *data2)
{
  if (lt != NULL && lt->cur_size > 0)
  {
    list_node *node = NULL;
    for (node = lt->dummy.prev; node != NULL; node = node->next)
    {
      if (cmp(node->data, data2) == 0)
      {
        return node->data;
      }
    }
  }
  return NULL;
}