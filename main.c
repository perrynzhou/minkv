/*************************************************************************
  > File Name: sample_kv.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/ 9 18:35:34 2020
 ************************************************************************/

#include "sample_kv.h"
#include "log.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <event2/event.h>
static int stop_main_loop = 0;
static void sig_handler(const int sig)
{
  stop_main_loop = 1;
}
static void start_worker_thread(sample_kv *sv)
{
  int ret = 0;
  //启动每个工作线程，在这里每个线程执行event_base_loop
  for (int i = 0; i < sv->thread_size; i++)
  {
    if ((ret=pthread_create(&sv->threads[i].thread_id, NULL, thread_start, &sv->threads[i])) != 0)
    {
      fprintf(stderr, "can't create thread: %s\n", strerror(ret));
      exit(1);
    }
  }
}
static void init_main_base(sample_kv *sv)
{
  struct event_config *ev_config;
  ev_config = event_config_new();
  event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
  sv->main_base = event_base_new_with_config(ev_config);
  event_config_free(ev_config);
  log_info("init main_base success");
}

inline static void init_worker_thread(sample_kv *sv, size_t thread_size)
{
  sv->threads = (thread *)calloc(thread_size, sizeof(thread));
  assert(sv->threads != NULL);
  sv->thread_size = thread_size;
  for (int i = 0; i < thread_size; i++)
  {
    int pipefd[2];
    if (pipe(pipefd) != 0)
    {
      perror("can't create pipe channel");
      exit(-1);
    }
    log_info("init thread %d",i);
    thread_init(&sv->threads[i], pipefd[1], pipefd[0],sv);
  }
}

inline static void init_connection(sample_kv *sv, int sfd, size_t connection_max_size)
{

  //sv->connections = (connection **)calloc(connection_max_size, sizeof(connection *));
  sv->connections = hash_list_alloc(16384);

  assert(sv->connections != NULL);
  sv->connection_max_size = connection_max_size;
  sv->listen = connection_new(sfd, listen_state,  EV_READ | EV_PERSIST, sv->main_base,sv);
  log_info("init global connections success");
}
int main(int argc, char *argv[])
{
  log_init(LOG_STDOUT_TYPE,NULL);
  sample_kv *sv = (sample_kv *)calloc(1, sizeof(sample_kv));
  int sock = init_tcp_sock(argv[1], atoi(argv[2]));
  if (sock != -1)
  {
    init_main_base(sv);
    init_connection(sv, sock, 8192);
    init_worker_thread(sv, 10);
  }
  signal(SIGINT, sig_handler);
  signal(SIGTERM, sig_handler);
  start_worker_thread(sv);
  /* enter the event loop */
  while (!stop_main_loop)
  {
    if (event_base_loop(sv->main_base, EVLOOP_ONCE) != 0)
    {
      break;
    }
  }
  event_base_free(sv->main_base);
  if (sv != NULL)
  {
    free(sv);
  }
  return 0;
}
