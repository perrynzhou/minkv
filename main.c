/*************************************************************************
  > File Name: main.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/ 9 18:35:34 2020
 ************************************************************************/

#include "kv.h"
#include "log.h"
#include "thread.h"
#include "utils.h"
#include "hashfn.h"
#include "thread_ev_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <ev.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
static sample_kv *sv = NULL;
static void start_thread(thread *thd);
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int cfd;

  if (EV_ERROR & revents)
  {
    perror("got invalid event");
    return;
  }
  // Accept client request
  cfd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);
  thread_ev_io *client = (thread_ev_io *)calloc(1, sizeof(thread_ev_io));

  if (cfd < 0)
  {
    perror("accept error");
    return;
  }
  fprintf(stdout, "accept a new connection\n");
  struct linger so_linger;
  so_linger.l_onoff = 1;
  so_linger.l_linger = 0;
  setsockopt(cfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof so_linger);
  int tid = hash_jump_consistent(cfd, sv->thread_size);
  thread *thd = &sv->threads[tid];
  client->ctx = thd;

  if (thd->status == 0)
  {
    log_info("recovery a new thread running");
    start_thread(thd);
  }
  log_info("choose thread %d to do client=%d", thd->id, cfd);
  __sync_fetch_and_add(&thd->connections, 1);
  // Initialize and start watcher to read client requests
  ev_io_init(&client->watcher, thread_read_cb, cfd, EV_READ);
  ev_io_start(thd->loop, &client->watcher);
}
static void start_thread(thread *thd)
{
  int ret = 0;
  thd->status = 1;
  if ((ret = pthread_create(&thd->thread_id, NULL, thread_func, thd)) != 0)
  {
    fprintf(stderr, "can't create thread: %s\n", strerror(ret));
    exit(1);
  }
}

inline static void setup_thread(sample_kv *sv, size_t thread_size)
{
  sv->threads = (thread *)calloc(thread_size, sizeof(thread));
  assert(sv->threads != NULL);
  sv->thread_size = thread_size;
  for (int i = 0; i < thread_size; i++)
  {
    thread_init(&sv->threads[i], i, sv);
  }
}

static void sample_kv_init(const char *addr, int port, size_t thread_size)
{
  if (sv == NULL)
  {
    sv = (sample_kv *)calloc(1, sizeof(sample_kv));
  }
  assert(sv != NULL);
  sv->sfd = init_tcp_sock(port);
  sv->loop = ev_default_loop(EVBACKEND_EPOLL);
  struct ev_io w_accept;
  ev_io_init(&w_accept, accept_cb, sv->sfd, EV_READ);
  ev_io_start(sv->loop, &w_accept);
  log_info("init sample_kv success");
  setup_thread(sv, thread_size);
  ev_run(sv->loop, 0);
}
int main(int argc, char *argv[])
{
  log_init(LOG_STDOUT_TYPE, NULL);
  sample_kv_init(argv[1], atoi(argv[2]), 2);
  return 0;
}
