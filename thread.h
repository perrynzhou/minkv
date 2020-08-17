/*************************************************************************
  > File Name: thread.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:34:29 PM CST
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H
#include "queue.h"
#include "hash_list.h"
#include <ev.h>
#include <stdint.h>
typedef struct thread_t
{
  struct ev_loop *loop; 
  int id;
  pthread_t thread_id;        /* unique ID of this thread */
  int status;
  uint64_t connections;
  void *ctx;
  hash_list *used_l;
  queue   *free_q;
} thread;

int thread_init(thread *thd,int id,void *ctx);
void thread_read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void *thread_func(void *arg);
#endif
