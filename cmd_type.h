/*************************************************************************
  > File Name: cmd_type.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五  8/21 16:16:05 2020
 ************************************************************************/

#ifndef _CMD_H
#define _CMD_H
typedef enum cmd_type_t {
  //create list test_list
  create_list_cmd,
  //lpush test_list value
  push_head_list_cmd,
  //rpush test_list  value
  push_tail_list_cmd,
  //lpop test_list
  pop_head_list_cmd,
  //rpop test_list
  pop_tail_list_cmd,
  //create channel test_channel
  create_channel_cmd,
  //subscribe  test_channel
  join_subscribe_cmd,
  //cancel test_channel
  cancel_subscribe_cmd
}cmd_type;

#endif
