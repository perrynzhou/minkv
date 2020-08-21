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
typedef struct channel_t {
  object  obj;
  size_t  active_fd;
  uint32_t  *fd_set;//save client fd,impl by bitmap
  //save cuurrent value of this channel
}channel;

channel *channel_create(const char *name,size_t size);
int channel_cancel(channel *c,int fd);
void channel_broadcast(channel *c);
void channel_destroy(channel *c);
#endif
