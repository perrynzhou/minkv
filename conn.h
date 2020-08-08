/*************************************************************************
  > File Name: conn.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:30:44 PM CST
 ************************************************************************/

#ifndef _CONN_H
#define _CONN_H
#include "conn_state.h"
#include "thread.h"
#include <event2/event.h>
typedef struct conn_t {
    int    sfd;
    conn_state  state;
    struct event event;
    void   *item;     /* for commands set/add/replace  */
    /* data for UDP clients */
    int    request_id; /* Incoming UDP request ID, if this is a UDP "connection" */
    struct sockaddr_in6 request_addr; /* udp: Who sent the most recent request */
    socklen_t request_addr_size;
    /* Used for generating a list of conn structures */
    thread *thread; /* Pointer to the thread object serving this connection */
    ssize_t (*read)(conn  *c, void *buf, size_t count);
    ssize_t (*write)(conn *c, void *buf, size_t count);
}conn;
#endif
