/*************************************************************************
    > File Name: channel.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, August 20, 2020 PM04:56:45
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H
#include "object.h"
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
typedef struct channel_t {
    //save cuurrent value of this channel
  object  obj;
  size_t  active_fd;
  uint32_t  *fd_set;//save client fd,impl by bitmap
  uint8_t  done;
  void *ctx;
}channel;

channel *channel_create(const char *name,void *ctx);
int channel_cancel_fd(channel *c,int fd);
void channel_broadcast(channel *c);
void channel_destroy(channel *c);
#endif
