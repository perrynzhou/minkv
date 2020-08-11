/*************************************************************************
  > File Name: utils.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Sat 08 Aug 2020 06:29:05 PM CST
 ************************************************************************/

#ifndef _UTILS_H
#define _UTILS_H
typedef enum state_t {
    listen_state, 
    parse_cmd_state,  
    put_cmd_state,
    get_cmd_state,  
    del_cmd_state,
    close_cmd_state
}state;
int init_tcp_sock(const char *addr,int port);
uint32_t 
#endif
