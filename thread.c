/*************************************************************************
  > File Name: thread.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:34:35 PM CST
 ************************************************************************/
#include "kv.h"
#include "log.h"
#include "thread.h"
#include "thread_ev_io.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <ev.h>
void thread_read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  char buffer[4096] = {'\0'};
  thread_ev_io *teo = (thread_ev_io *)watcher;
  struct ev_io *watcher_ = &teo->watcher;
  thread *thd =(thread *)teo->ctx;
  size_t read = recv(watcher_->fd, buffer, 4096, 0);
  if (read < 0)
  {
    perror("read error");
    return;
  }

  if (read == 0)
  {
    if(watcher_->fd!=-1)
    {
      close(watcher_->fd);
    }
    // Stop and free watchet if client socket is closing
    ev_io_stop(loop, watcher_);
    free(teo);
    __sync_fetch_and_sub(&thd->connections,1);
    log_info("stop watcher,thread connections=%d",thd->connections);
    if(thd->connections==0) {
      ev_break(loop,EVBREAK_ALL);
      log_info("====notify thread %ld stop,ret=%d",thd->thread_id);
      thd->status=0;
    }
    return;
  }
  else
  {
    log_info("thread %ld  recv:%s", pthread_self(), (char *)&buffer);
  }
  bzero(&buffer, read);
}

int thread_init(thread *thd, int id, void *ctx)
{
  thd->loop = ev_loop_new(EVBACKEND_EPOLL);
  thd->ctx = ctx;
  thd->id = id;
  thd->status = 0;
  log_info("thread %d init ev_loop success", thd->id);
}
void *thread_func(void *arg)
{
  thread *thd = (thread *)arg;
  log_info("thread %ld running ev_loop", pthread_self());
  pthread_detach(thd->thread_id);
  ev_run(thd->loop, 0);
  log_info("thread %ld stopping ev_loop", pthread_self());
  return NULL;
}
