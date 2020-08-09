/*************************************************************************
  > File Name: sample_kv.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/ 9 18:35:34 2020
 ************************************************************************/

#include "conn.h"
#include "thread.h"
#include <stdio.h>

typedef struct sample_kv_t
{
  struct event_base *main;
  size_t thread_size;
  thread *threads;
  conn *listen;
  conn **conns;
  size_t conn_max_size;
} sample_kv;
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
}
inline static void sample_kv_init_conn(sample_kv *sv, size_t conn_max_size)
{
  sv->gconns = (conn **)calloc(conn_max_size, sizeof(conn *));
  assert(sv->gconns != NULL);
  sv->conn_max_size = conn_max_size;
}
inline static int sample_kv_init_socket(const char *addr, int port)
{
  return -1;
}
int main(void)
{
  return 0;
}
