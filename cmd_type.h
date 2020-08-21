/*************************************************************************
  > File Name: cmd_type.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五  8/21 16:16:05 2020
 ************************************************************************/

#ifndef _CMD_H
#define _CMD_H
typedef enum cmd_type_t {
  //create list "test_list"
  create_list_cmd,
  push_head_list_cmd,
  push_tail_list_cmd,
  pop_head_list_cmd,
  pop_tail_list_cmd,
  create_channel_cmd,
  join_subscribe_cmd,
  cancel_subscribe_cmd
}cmd_type;

#endif
