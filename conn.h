/*************************************************************************
  > File Name: conn.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:30:44 PM CST
 ************************************************************************/

#ifndef _CONN_H
#define _CONN_H
#include "conn_state.h"
#include <event2/event.h>
typedef struct conn_t
{
  int sfd;
  conn_state state;
  struct event event;
  short ev_flags;
  void *item;
  ssize_t (*read)(conn *c, void *buf, size_t count);
  ssize_t (*write)(conn *c, void *buf, size_t count);
} conn;
conn *conn_new(int sfd, conn_state state, void *item, const int event_flags);
int conn_init_eventbase(conn *c, struct event_base *base);
void conn_event_handler(const int fd, const short which, void *arg);
void conn_delete(conn *con);
#endif
