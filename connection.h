/*************************************************************************
  > File Name: connection.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:30:44 PM CST
 ************************************************************************/

#ifndef _CONNECTION_H
#define _CONNECTION_H
#include "utils.h"
#include "thread.h"
#include <event2/event.h>
#include <event2/event_struct.h>
typedef struct connection_t
{
  int sfd;
  state state;
  struct event event;
  short ev_flags;
  void *item;
  thread *thd;
  void *ctx;
} connection;
connection *connection_new(int sfd, state state, const int event_flags,struct event_base *base, void *ctx);
void connection_free(connection *con);
void connection_execute_cmd(connection *c);
#endif
