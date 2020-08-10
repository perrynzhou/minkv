/*************************************************************************
  > File Name: thread.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:34:35 PM CST
 ************************************************************************/

#include "thread.h"
#include <stdio.h>
static void thread_event_process(evutil_socket_t fd, short which, void *arg)
{
}

int thread_init(thread *thd, int notify_send_fd, int notify_receive_fd)
{
  thd->new_conn_queue = conn_queue_create();
  thd->notify_receive_fd = notify_receive_fd;
  thd->notify_send_fd = notify_send_fd;
  struct event_config *ev_config;
  ev_config = event_config_new();
  event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
  thd->base = event_base_new_with_config(ev_config);
  event_config_free(ev_config);
  //每个IO时间的回调函数设置为thread_libevent_process，当有IO event时候回调用thread_libevent_process函数处理
  event_set(&thd->notify_event, thd->notify_receive_fd,
            EV_READ | EV_PERSIST, thread_event_process, &thd);
  event_base_set(thd->base, &thd->notify_event);

  if (event_add(&thd->notify_event, 0) == -1)
  {
    fprintf(stderr, "Can't monitor libevent notify pipe\n");
    exit(1);
  }
}
void *thread_start(void *arg)
{
    thread *thd = (thread *)arg;
    event_base_loop(thd->base, 0);
    event_base_free(thd->base);
    return NULL;
}
