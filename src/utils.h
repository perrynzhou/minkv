/*************************************************************************
  > File Name: utils.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:29:05 PM CST
 ************************************************************************/

#ifndef _UTILS_H
#define _UTILS_H
int init_tcp_sock(int port, int backlog);
int set_tcp_so_linger(int fd, int onoff, int linger);
#endif
