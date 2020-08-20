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
typedef struct min_kv_t
{
  struct ev_loop *loop;
  size_t thread_size;
  thread *threads;
  int   sfd;
  void *ctx;
} min_kv;
#endif