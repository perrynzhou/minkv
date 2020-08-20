/*************************************************************************
    > File Name: channel.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, August 20, 2020 PM04:56:45
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H
#include <stdio.h>
typedef struct channel_t {
  char *chan_name;
  int  *fd_set;//save client fd,impl by bitmap
}channel;

channel *channel_create(const char *name,size_t size);
int channel_cancel(channel *c,int fd);
void channel_destroy(channel *c);
#endif
