/*************************************************************************
  > File Name: connection.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sun 09 Aug 2020 05:43:03 PM CST
 ************************************************************************/

#include "connection.h"
#include "hashfn.h"
#include "utils.h"
#include "log.h"
#include "sample_kv.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <event2/event_compat.h>
static void connection_event_handler(const int fd, const short which, void *arg);
static bool connection_update_event(connection *c, const int new_flags);
static void handle_machine(connection *c);
//parse command
void connection_handle_parse_cmd(connection *c)
{
}
void connection_handle_put_cmd(connection *c)
{
}
void connection_handle_get_cmd(connection *c)
{
}
void connection_handle_del_cmd(connection *c)
{
}
void connection_handle_close_cmd(connection *c)
{
}
void connection_execute_cmd(connection *c)
{
  write(c->sfd, "success\n", 8);
  switch (c->state)
  {
  case put_cmd_state:
    connection_handle_put_cmd(c);
    break;
  case get_cmd_state:
    connection_handle_get_cmd(c);
    break;
  case del_cmd_state:
    connection_handle_del_cmd(c);
    break;
  case close_cmd_state:
    connection_handle_close_cmd(c);
    break;
  }
}

static bool connection_update_event(connection *c, const int new_flags)
{
  assert(c != NULL);

  struct event_base *base = c->event.ev_base;
  if (c->ev_flags == new_flags)
  {
    return true;
  }
  if (event_del(&c->event) == -1)
  {
    return false;
  }
  event_set(&c->event, c->sfd, new_flags, connection_event_handler, (void *)c);
  event_base_set(base, &c->event);
  c->ev_flags = new_flags;
  if (event_add(&c->event, 0) == -1)
  {
    return false;
  }
  return true;
}
//多线程来处理客户端请求，这里把fd封装到CQ_ITEM中，然后通知摸个线程去处理这个fd
static void dispatch_connection(int sfd, int init_state, int event_flags, void *ctx)
{
  sample_kv *sv = (sample_kv *)ctx;
  queue_item *item = queue_item_create(sfd, init_state, event_flags, ctx);
  char buf[1];
  if (item == NULL)
  {
    close(sfd);
    fprintf(stderr, "Failed to allocate memory for connectionection object\n");
    return;
  }
  int tid = hash_jump_consistent(sfd, sv->thread_size);
  log_info("choose thread %d", tid);
  thread *thd = &sv->threads[tid];
  //把item放到某个线程的连接队列中
  queue_push(thd->new_connection_queue, item);

  //这里通知写入一个字符到工作线程的notify_send_fd，工作线程的把notify_recv_fd注册到event中，有IO事件就通知
  buf[0] = 'c';
  if (write(thd->notify_send_fd, buf, 1) != 1)
  {
    perror("Writing to thread notify pipe");
  }
}
static void handle_machine(connection *c)
{
  bool stop = false;
  int sfd;
  socklen_t addrlen;
  struct sockaddr_storage addr;
  int res;
  const char *str;

  assert(c != NULL);
  log_info("trigge a event");
  while (!stop)
  {

    switch (c->state)
    {
    case listen_state:
      addrlen = sizeof(addr);
      sfd = accept(c->sfd, (struct sockaddr *)&addr, &addrlen);
      if (sfd == -1)
      {
        continue;
      }
      if (fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL) | O_NONBLOCK) < 0)
      {
        perror("fcntl");
        break;
      }
      dispatch_connection(sfd, parse_cmd_state, EV_READ | EV_PERSIST, c->ctx);
      stop = true;
      break;
    case parse_cmd_state:
      connection_execute_cmd(c);
      connection_update_event(c,0);
      break;
    }
  }
  return;
}

static void connection_event_handler(const int fd, const short which, void *arg)
{
  connection *c = (connection *)arg;
  assert(c != NULL);
  c->which =which;
  /* sanity */
  if (fd != c->sfd)
  {
    fprintf(stderr, "catastrophic: event fd doesn't match connection fd!\n");
    return;
  }
  //log_info("handle one event fd=%d", c->sfd);
  handle_machine(c);
}
connection *connection_new(int sfd, state state, const int event_flags, struct event_base *base, void *ctx)
{
  sample_kv *sv = (sample_kv *)ctx;
  connection *con = (connection *)hash_list_search(sv->connections, sfd);
  if (con == NULL)
  {
    con = (connection *)calloc(1, sizeof(connection));
    assert(con != NULL);
    con->sfd = sfd;
    con->state = state;
    con->ctx = ctx;
    hash_list_insert(sv->connections, sfd, con);
  }
  event_set(&con->event, sfd, event_flags, connection_event_handler, (void *)con);
  event_base_set(base, &con->event);
  con->ev_flags = event_flags;
  if (event_add(&con->event, 0) == -1)
  {
    perror("event_add");
    return NULL;
  }
  log_info("trigge a new conenction %d", sfd);
  return con;
}

void connection_free(connection *c)
{
  event_del(&c->event);
  close(c->sfd);
  free(c);
  c = NULL;
}
