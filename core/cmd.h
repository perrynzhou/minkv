/*************************************************************************
  > File Name: cmd.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五  8/21 16:16:05 2020
 ************************************************************************/

#ifndef _CMD_H
#define _CMD_H
#include <stdint.h>
#define CMD_KEY_MAX_LEN  (4096)
typedef enum cmd_type_t {
  //lpush test_list value
  push_head_list_cmd,
  //rpush test_list  value
  push_tail_list_cmd,
  //lpop test_list
  pop_head_list_cmd,
  //rpop test_list
  pop_tail_list_cmd,
  //push test_hash_list value
  push_hash_list_cmd,
  //pop test_hash_list value
  pop_hash_list_cmd,
  //create channel test_channel
  create_channel_cmd,
  //subscribe  test_channel
  join_subscribe_cmd,
  //cancel test_channel
  cancel_subscribe_cmd,
  null_cmd,
}cmd_type;
typedef struct cmd_t {
  char key[CMD_KEY_MAX_LEN];
  union {
    char *str;
  }value;
  uint8_t type;
}cmd;
int cmd_parse(cmd *c,char *input,void *ctx);
#endif
