/*************************************************************************
  > File Name: main.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/ 9 18:35:34 2020
 ************************************************************************/

#include "min_kv.h"
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
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
static void start_thread(thread *thd);
static min_kv *g_sv = NULL;
typedef struct main_ev_io_t
{
  struct ev_io watcher;
  void *ctx;
} main_ev_io;
void *minkv_channel_expire_ticker(void *arg)
{
  min_kv *mv= (min_kv *)arg;
  struct timeval tv;
 tv.tv_sec = 0;
 tv.tv_usec = 200000; /* 0.2 秒*/
  for(;;)
  {
    select(0, NULL, NULL, NULL, &tv);
    //access skiplist from max_level,and expire key
    //todo
  }
  pthread_exit(NULL);
  return NULL;
}
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

  main_ev_io *mev = (main_ev_io *)watcher;
  struct ev_io *watcher_ = &mev->watcher;
  // Accept client request
  cfd = accept(watcher_->fd, (struct sockaddr *)&client_addr, &client_len);

  if (cfd < 0)
  {
    perror("accept error");
    return;
  }
  log_info("main thread %ld accept a new connection", pthread_self());

  set_tcp_so_linger(cfd, 1, 0);

  min_kv *sv = (min_kv *)mev->ctx;
  int tid = hash_jump_consistent(cfd, sv->thread_size);
  thread *thd = &sv->threads[tid];
  thread_ev_io *client = (thread_ev_io *)queue_pop(thd->free_q);
  if (client == NULL)
  {
    client = thread_ev_io_create(thd);
  }
  if (thd->status == 0)
  {
    log_info("recovery  thread-%d running", thd->id);
    start_thread(thd);
  }
  hash_list_insert(thd->used_l, cfd, client);
  log_info("dispatch request fd=%d to  thread-%d", cfd, thd->id);
  __sync_fetch_and_add(&thd->connections, 1);
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

inline static void setup_thread(min_kv *sv, size_t thread_size)
{
  sv->threads = (thread *)calloc(thread_size, sizeof(thread));
  assert(sv->threads != NULL);
  sv->thread_size = thread_size;
  for (int i = 0; i < thread_size; i++)
  {
    thread_init(&sv->threads[i], i, sv);
  }
}

static min_kv *min_kv_create(const char *addr, int port, size_t thread_size)
{

  min_kv *sv = (min_kv *)calloc(1, sizeof(min_kv));
  if (g_sv == NULL)
  {
    g_sv = sv;
  }
  assert(sv != NULL);
  sv->sfd = init_tcp_sock(port, 1024);
  sv->loop = ev_default_loop(EVBACKEND_EPOLL);
  sv->channels = list_create(4096);
  main_ev_io *mev = (main_ev_io *)calloc(1, sizeof(main_ev_io));
  assert(mev!=NULL);
  sv->ctx = mev;
  mev->ctx = sv;
  ev_io_init(&mev->watcher, accept_cb, sv->sfd, EV_READ);
  ev_io_start(sv->loop, &mev->watcher);
  log_info("init min_kv success");
  setup_thread(sv, thread_size);
  ev_run(sv->loop, 0);
}
static void min_kv_destroy(min_kv *sv)
{
  if (sv != NULL)
  {
    ev_break(sv->loop, EVBREAK_ALL);
    main_ev_io *mev = (main_ev_io *)sv->ctx;
    ev_io_stop(sv->loop, &mev->watcher);
    for (size_t i = 0; i < sv->thread_size; i++)
    {
      hash_list_data_free_cb free_cb = (hash_list_data_free_cb)&thread_ev_io_destroy;
      queue_destroy(sv->threads[i].free_q);
      hash_list_destroy(sv->threads[i].used_l, free_cb);
      log_info("deinit thread-%d", i);
    }
    free(sv->threads);
    log_info("min_kv exit success");
  }
}
inline static void sample_exit(int sig)
{
  min_kv_destroy(g_sv);
  _exit(0);
}
int main(int argc, char *argv[])
{
  log_init(LOG_STDOUT_TYPE, NULL);
  signal(SIGINT, sample_exit);
  min_kv *sv = min_kv_create(argv[1], atoi(argv[2]), 1);
  return 0;
}
