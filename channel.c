/*************************************************************************
    > File Name: channel.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, August 20, 2020 PM04:56:49
 ************************************************************************/

#include "channel.h"
#include <stdlib.h>
#include <assert.h>
channel *channel_create(const char *name,size_t size){
  channel *c = (channel *)calloc(1,sizeof(channel));
  assert(c!=NULL);
  c->chan_name = strdup(name);
  
  c->fd_set =calloc(size/32+1,sizeof(int));
  assert(c->fd_set!=NULL);
  return c;
}
int channel_cancel(channel *c,int fd)
{
  
}
void channel_destroy(channel *c)
{

}
