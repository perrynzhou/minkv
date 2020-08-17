/*************************************************************************
  > File Name: thread_ev_io.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:21:28 2020
 ************************************************************************/

#ifndef _THREAD_EV_IO_H
#define _THREAD_EV_IO_H
#include <ev.h>
typedef struct thread_ev_io_t {
  struct ev_io watcher;
  void *ctx;
}thread_ev_io;
#endif
