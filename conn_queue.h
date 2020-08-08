/*************************************************************************
  > File Name: conn_queue.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:37:41 PM CST
 ************************************************************************/

#ifndef _CONN_QUEUE_H
#define _CONN_QUEUE_H
#include "conn_queue.h"
typedef struct conn_queue_t {
    conn_queue_item *head;
    conn_queue_item *tail;
    pthread_mutex_t lock;
};
#endif
