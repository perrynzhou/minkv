/*************************************************************************
    > File Name: connection_utils.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, August 10, 2020 PM04:05:29
 ************************************************************************/

#include "utils.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
static int init_socket(int domain, int type, int protocol, int backlog,
                       const char *addr, int port)
{
  int sock = socket(domain, type, protocol);
  if (sock != -1)
  {
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = domain;
    serveraddr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &serveraddr.sin_addr) <= 0)
    {
      close(sock);
      printf("inet_pton error for %s\n", addr);
      return -1;
    }
    bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (type == SOCK_STREAM)
    {
      int opt = 1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
      // set_tcp_nonblock(sock);
      listen(sock, backlog);
      if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
      {
        perror("setting O_NONBLOCK");
        close(sock);
        sock=-1;
      }
    }
  }
  return sock;
}
int init_tcp_sock(const char *addr, int port)
{
  return init_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 4096, addr, port);
}
