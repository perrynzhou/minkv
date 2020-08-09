/*************************************************************************
  > File Name: conn.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sun 09 Aug 2020 05:43:03 PM CST
 ************************************************************************/

#include "conn.h"
#include "conn_state.h"
#include <stdio.h>
conn *conn_new(int sfd, conn_state state, void *item, const int event_flags, struct event_base *base)
{
  conn *con = (conn *)calloc(1, sizeof(conn));
  assert(conn != NULL);
  con->sfd = sfd;
  con->state = state;
  con->item = item;
  con->write = NULL;
  con->read = NULL;
  return con;
}
int conn_init_eventbase(conn *c, struct event_base *base)
{
  event_set(&c->event, sfd, event_flags, event_handler, (void *)c);
  event_base_set(base, &c->event);
  c->ev_flags = event_flags;

  if (event_add(&c->event, 0) == -1)
  {
    perror("event_add");
    return -1;
  }
  return 0;
}
void conn_event_handler(const int fd, const short which, void *arg)
{
  conn *c = (conn *)arg;
  assert(c != NULL);

  c->which = which;

  /* sanity */
  if (fd != c->sfd)
  {
    if (settings.verbose > 0)
      fprintf(stderr, "Catastrophic: event fd doesn't match conn fd!\n");
    conn_delete(c);
    return;
  }

  //  drive_machine(c);
  return;
}
void conn_delete(conn *c)
{
  event_del(&c->event);
  close(c->sfd);
  free(c);
  c = NULL;
}
