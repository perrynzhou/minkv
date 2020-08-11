/*************************************************************************
    > File Name: sample_kv.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, August 11, 2020 PM03:20:29
 ************************************************************************/

#ifndef _SAMPLE_KV_H
#define _SAMPLE_KV_H
#include "connection.h"
#include "thread.h"
#include "utils.h"
typedef struct sample_kv_t
{
  struct event_base *main_base;
  size_t thread_size;
  thread *threads;
  connection *listen;
  connection **connections;
  size_t connection_max_size;
} sample_kv;
#endif
