/*************************************************************************
  > File Name: cmd.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六  8/22 14:35:50 2020
 ************************************************************************/

#include "cmd.h"
#include "min_kv.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
typedef struct cmd_value_t
{
  char *name;
  size_t len;
  size_t support_type;
} cmd_value;
static cmd_value cmd_vlaues[null_cmd - 1] = {
    {
        .name = "lpush",
        .len = strlen("lpush"),
    },
    {
        .name = "rpush",
        .len = strlen("rpush"),
    },
    {
        .name = "lpop",
        .len = strlen("lpop"),
    },
    {
        .name = "push",
        .len = strlen("push"),
    },
    {
        .name = "pop",
        .len = strlen("pop"),
    },
    {
        .name = "rpop",
        .len = strlen("rpop"),
    },
    {
        .name = "subscribe",
        .len = strlen("subscribe"),
    },
    {
        .name = "cancel",
        .len = strlen("subscribe"),
    }};
cmd_value *cmd_check_type(const char *str)
{
  if (str != NULL)
  {
    size_t len = strlen(str);
    for (int i = 0; i < null_cmd; i++)
    {
      if (strncmp(str, cmd_vlaues[i].name, cmd_vlaues[i].len) && cmd_vlaues[i].len == len)
      {
        return &cmd_values[i];
      }
    }
  }
  return NULL;
}
static int  cmd_cmp_key(void *src,void *dst) {
  char *str_src =(char *)src;
  char *str_dst =(char *)dst;
  size_t src_len = strlen(str_src);
  size_t dst_len = strlen(str_dst);
  if(src_len !=dst_len) {
    return 1;
  }
  return strncmp(str_src,str_dst,src_len);
}
int cmd_parse(cmd *c, char *input,void *ctx)
{
  char *cmds[3] = {NULL};
  char *str = NULL;
  int index = 0;
  min_kv *mv = (min_kv *)ctx;
  cmd_value *c_value = NULL;
  while ((str = strsep(&input, " ")) != NULL)
  {
    cmds[index] = str;
  }
  if((c_value=cmd_check_type(cmds[0]))==NULL)
  {
    return -1;
  }
  strncpy((char *)&c->key,cmds[1],strlen(cmds[1]));
  c->type=c->type;
  c->value = strdup(cmds[2]);
  return 0;
}
