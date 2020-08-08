/*************************************************************************
  > File Name: conn_item.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:27:55 PM CST
 ************************************************************************/

#ifndef _CONN_ITEM_H
#define _CONN_ITEM_H
#include "conn_state.h"
#include "conn.h"
typedef struct conn_queue_item_t {
    int               sfd;
    conn_state  init_state;
    conn *c;
    struct conn_queue_item_t          *next;
}conn_queue_item;
#endif
