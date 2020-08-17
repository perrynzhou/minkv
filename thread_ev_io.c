/*************************************************************************
  > File Name: thread_ev_io.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 23:22:01 2020
 ************************************************************************/

#include "thread_ev_io.h"
#include <stdlib.h>
thread_ev_io *thread_ev_io_create(void *ctx){
    thread_ev_io *tev =(thread_ev_io *)calloc(1,sizeof(thread_ev_io));
    if(tev!=NULL) {
          tev->ctx = ctx;
    }
    return tev;
}
void thread_ev_io_destroy(thread_ev_io *tev)
{
  if(tev!=NULL)
  {
    free(tev);
    tev=NULL;
  }
}

