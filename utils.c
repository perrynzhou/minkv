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
#include <strings.h>
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
      /*
      if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
      {
        perror("setting O_NONBLOCK");
        close(sock);
        sock=-1;
      }
      */
    }
  }
  return sock;
}
int init_tcp_sock(int port)
{
  int sd;
  struct sockaddr_in addr;
  int addr_len = sizeof(addr);

  // Create server socket
  if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    return -1;
  }

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
  {
    perror("bind error");
  }

  // Start listing on the socket
  if (listen(sd, 2) < 0)
  {
    perror("listen error");
    return -1;
  }
  return sd;
}
