/*************************************************************************
  > File Name: thread.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:34:29 PM CST
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H
#include "queue.h"
#include <event2/event.h>
#include <event2/event-config.h>
#include <event2/event_struct.h>
typedef struct thread_t
{
  pthread_t thread_id;        /* unique ID of this thread */
  struct event_base *base;    /* libevent handle this thread uses */
  struct event notify_event;  /* listen event for notify pipe */
  int notify_receive_fd;      /* receiving end of notify pipe */
  int notify_send_fd;         /* sending end of notify pipe */
  queue *new_connection_queue; /* queue of new connections to handle */
  void *ctx;
} thread;
int thread_init(thread *thd,int notify_send_fd,int notify_receive_fd);
void *thread_start(void *arg);
#endif
