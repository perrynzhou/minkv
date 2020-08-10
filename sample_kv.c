/*************************************************************************
  > File Name: sample_kv.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/ 9 18:35:34 2020
 ************************************************************************/

#include "conn.h"
#include "thread.h"
#include "conn_utils.h"
#include <stdio.h>
static int stop_main_loop = 0;
typedef struct sample_kv_t
{
  struct event_base *main;
  size_t thread_size;
  thread *threads;
  conn *listen;
  conn **conns;
  size_t conn_max_size;
} sample_kv;
static void sig_handler(const int sig)
{
  stop_main_loop = 1;
}
static void sample_kv_start_worker(sample_kv *sv)
{
  //启动每个工作线程，在这里每个线程执行event_base_loop
  for (i = 0; i < nthreads; i++)
  {
    if ((ret = pthread_create(&sv->threads[i].thread_id, NULL, thread_start, &sv->threads[i])) != 0)
    {
      fprintf(stderr, "Can't create thread: %s\n", strerror(ret));
      exit(1);
    }
  }
}
static void sample_kv_init_eventbase(sample_kv *sv)
{
  struct event_config *ev_config;
  ev_config = event_config_new();
  event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
  sv->main = event_base_new_with_config(ev_config);
  event_config_free(ev_config);
}

inline static void sample_kv_init_thread(sample_kv *sv, size_t thread_size)
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
    thread_init(&threads[i], pipefd[1], pipefd[0]);
  }
}

inline static void sample_kv_init_conn(sample_kv *sv, int sfd, size_t conn_max_size)
{
  sv->gconns = (conn **)calloc(conn_max_size, sizeof(conn *));
  assert(sv->gconns != NULL);
  sv->conn_max_size = conn_max_size;
  sv->listen = conn_new(sfd, conn_listening, NULL, EV_READ | EV_PERSIST, &sv->conns);
  conn_init_eventbase(sv->listen, sv->main);
}
int main(void)
{
  sample_kv *sv = (sample_kv *)calloc(1, sizeof(sample_kv));
  int sock = init_tcp_sock(argv[1], argv[2]);
  if (sock != -1)
  {
    sample_kv_init_eventbase(sv);
    sample_kv_init_conn(sv, sock, 8192);
    sample_kv_init_thread(sv, 10);
  }
  signal(SIGINT, sig_handler);
  signal(SIGTERM, sig_handler);
  sample_kv_start_worker(sv);
  /* enter the event loop */
  while (!stop_main_loop)
  {
    if (event_base_loop(main_base, EVLOOP_ONCE) != 0)
    {
      retval = EXIT_FAILURE;
      break;
    }
  }
  event_base_free(main_base);
  if (sv != NULL)
  {
    free(kv);
  }
  return 0;
}
