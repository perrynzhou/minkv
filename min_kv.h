/*************************************************************************
    > File Name: kv.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, August 11, 2020 PM03:20:29
 ************************************************************************/

#ifndef _MIN_KV_H
#define _MIN_KV_H
#include "hash_list.h"
#include "thread.h"
#include "utils.h"
#include "list.h"
typedef struct min_kv_t
{
  //server socket fd
  int sfd;
  //save  main_ev_io struct,that is  used for accept
  void *ctx;
  struct ev_loop *loop;
  size_t thread_size;
  thread *threads;
  //save list objects
  list *list_objects;
  //save channel objects
  hash_list *channel_objects;
  //save string objects
  hash_list *string_objects;
} min_kv;
#endif
