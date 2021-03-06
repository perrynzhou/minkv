/*************************************************************************
    > File Name: channel.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, August 20, 2020 PM04:56:49
 ************************************************************************/

#include "channel.h"
#include "skiplist.h"
#include "log.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/select.h>
#define SHIFT 5
#define MASK 0x1F
#define BIT_MAX_RANGE  (16384*16384)
inline static void channel_setbit(channel *c, int n)
{
  c->fd_set[n >> SHIFT] |= (1 << (n & MASK));
}
inline static int channel_clsbit(channel *c, int n)
{
  return c->fd_set[n >> SHIFT] & (1 << (n & MASK));
}

channel *channel_create(const char *name,void *ctx)
{
  channel *c = (channel *)calloc(1, sizeof(channel));
  assert(c != NULL);
  c->ctx = ctx;
  c->obj.key = strdup(name);
  c->obj.type=OBJECT_SKIPLIST;
  skiplist_node_data_cb  cb = free;
  c->obj.value = skiplist_create(cb);
  size_t alloc_size = (BIT_MAX_RANGE/32)+1;
  c->fd_set = calloc(alloc_size / 32 + 1, sizeof(uint32_t));
  assert(c->fd_set != NULL);
  memset(c->fd_set,0,sizeof(uint32_t)*alloc_size);
  return c;
}
int channel_cancel_fd(channel *c, int fd)
{
  if(c!=NULL && fd!=-1) {
    channel_clsbit(c,fd);
    return 0;
  }
  return -1;
}
void channel_destroy(channel *c)
{
  if(c!=NULL) {
    free(c->obj.key);
    skiplist *slt = (skiplist *)c->obj.value;
    skiplist_destroy(slt);
    free(c->fd_set);
    free(c);
    c=NULL;
  }
}
