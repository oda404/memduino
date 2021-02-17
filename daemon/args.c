// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"args.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define UI unsigned int

void print_help_message()
{
printf("\
Usage: \n\
  memduinod [options]\n\
\n\
Starts the memduino daemon\n\
\n\
Options:\n\
  -h, --help                      show this message and exit\n\
  -u, --update-interval <n> >= 0  set the interval at which the packets are sent to the arduino in ms\n\
  -i, --init-timeot     <n> >= 0  set the timeout for connecting to the serial port in ms\n\
  -I, --init-try-sleep  <n> >= 0  set the sleep time between connection attempts in ms (can't be bigger than --init-timeout)\n\
  -s, --serial-port     <port>    set the device to which to connect\n\
\
");
}

void args_init(Args *args)
{
    args->args_arr = NULL;
    args->argsc = 0;
}

void args_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    bool is_flag,
    Args *args
)
{
    /* make space for the new arg */
    args->args_arr = 
    realloc(
        args->args_arr, 
        sizeof(Arg) * (args->argsc + 1)
    );

    Arg *head_arg = &args->args_arr[args->argsc];

    head_arg->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(head_arg->name, name);
    head_arg->arg_short = malloc(sizeof(char) * (strlen(arg_short) + 1));
    strcpy(head_arg->arg_short, arg_short);
    head_arg->arg_long = malloc(sizeof(char) * (strlen(arg_long) + 1));
    strcpy(head_arg->arg_long, arg_long);
    head_arg->is_flag = is_flag;
    head_arg->value = NULL;

    ++args->argsc;
}

void args_free(Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        free(args->args_arr[i].value);
        free(args->args_arr[i].name);
        free(args->args_arr[i].arg_short);
        free(args->args_arr[i].arg_long);
    }
    free(args->args_arr);
    /* clean up */
    args_init(args);
}

void args_parse(int argc, char **argv, Args *args)
{
    UI argv_i = 0;
    UI args_i = 0;

    for(; args_i < args->argsc; ++args_i)
    {
        for(argv_i = 0; argv_i < (UI)argc; ++argv_i)
        {
            Arg *arg_tmp = &args->args_arr[args_i];
            if(
                strcmp(arg_tmp->arg_short, argv[argv_i]) == 0 ||
                strcmp(arg_tmp->arg_long, argv[argv_i]) == 0
            )
            {
                if(arg_tmp->is_flag)
                {
                    arg_tmp->value = malloc(sizeof(bool));
                    *(bool *)arg_tmp->value = true;
                }
                else if(argv_i + 1 < (UI)argc)
                {
                    arg_tmp->value =
                    malloc(sizeof(char) * (strlen(argv[argv_i + 1]) + 1));
                    strcpy(arg_tmp->value, argv[argv_i + 1]);
                }
                else
                {
                    printf("[argparser] Warning: Invalid value for arg '%s'\n", argv[argv_i]);
                }
            }
        }
    }
}

bool args_get_bool(const char *name, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; i++)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            return tmp_arg->value ? true : false;
        }
    }

    return false;
}

int args_out_str(const char *name, char **out, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value)
            {
                free(*out);
                *out = malloc(sizeof(char) * (strlen(tmp_arg->value) + 1));
                strcpy(*out, tmp_arg->value);
                return ARG_OK;
            }
            return ARG_BAD;
        }
    }

    return ARG_OK;
}

int args_out_uint(const char *name, UI *out, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value)
            {
                *out = (UI)atoi(tmp_arg->value);
                return ARG_OK;
            }
            return ARG_BAD;
        }
    }

    return ARG_BAD;
}
