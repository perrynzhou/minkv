/*************************************************************************
  > File Name: thread.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:34:35 PM CST
 ************************************************************************/
#include "connection.h"
#include "sample_kv.h"
#include "log.h"
#include "thread.h"
#include "queue.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <event2/event_compat.h>

static void thread_event_process(evutil_socket_t fd, short which, void *arg)
{
  thread *thd = arg;
  /*
  sample_kv *sv = (sample_kv *)thd->ctx;
  hash_list *connections = sv->connections;
  */
  queue_item *item;
  char buf[1];
  connection *c;

  if (read(fd, buf, 1) != 1)
  {
    fprintf(stderr, "can't read from libevent pipe\n");
    return;
  }
  switch (buf[0])
  {
  case 'c':
    item = queue_pop(thd->new_connection_queue);
    if (NULL == item)
    {
      break;
    }
    switch (item->mode)
    {
    case queue_new_conn:
      c = connection_new(item->sfd, item->init_state, item->event_flags, thd->base, item->ctx);
      c->thd = thd;
      c->state = parse_cmd_state;
      log_info("thread %ld init %d connection", pthread_self(), c->sfd);
      break;
    }
    queue_item_free(item);
    break;
  case 's':
    event_base_loopexit(thd->base, NULL);
    break;
  }
}

int thread_init(thread *thd, int notify_send_fd, int notify_receive_fd, void *ctx)
{
  thd->new_connection_queue = queue_create();
  thd->notify_receive_fd = notify_receive_fd;
  thd->notify_send_fd = notify_send_fd;
  thd->ctx = ctx;
  struct event_config *ev_config;
  ev_config = event_config_new();
  event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
  thd->base = event_base_new_with_config(ev_config);
  event_config_free(ev_config);
  //每个IO时间的回调函数设置为thread_event_process，当有IO event时候回调用thread_event_process函数处理

  event_set(&thd->notify_event, thd->notify_receive_fd, EV_READ | EV_PERSIST, thread_event_process, (void *)thd);
  event_base_set(thd->base, &thd->notify_event);

  if (event_add(&thd->notify_event, 0) == -1)
  {
    fprintf(stderr, "Can't monitor libevent notify pipe\n");
    exit(1);
  }
  log_info("init thread %ld success", thd->thread_id);
}
void *thread_start(void *arg)
{
  thread *thd = (thread *)arg;
  event_base_loop(thd->base, 0);
  event_base_free(thd->base);
  return NULL;
}
