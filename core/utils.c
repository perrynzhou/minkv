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
#include <netinet/in.h>
int init_tcp_sock(int port, int backlog)
{
  int sfd;
  struct sockaddr_in addr;
  int addr_len = sizeof(addr);

  // Create server socket
  if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    return -1;
  }

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
  {
    perror("bind error");
  }

  // Start listing on the socket
  if (listen(sfd, backlog) < 0)
  {
    perror("listen error");
    return -1;
  }
  return sfd;
}
int set_tcp_so_linger(int fd, int onoff, int linger)
{
  struct linger so_linger;
  so_linger.l_onoff = onoff;
  so_linger.l_linger = linger;
  return setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof so_linger);
}
