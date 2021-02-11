// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"args.h"

#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

#define HELP_ARG_SHORT            "-h"
#define HELP_ARG_LONG             "--help"
#define UPDATE_INTERVAL_ARG_SHORT "-u"
#define UPDATE_INTERVAL_ARG_LONG  "--update-interval"
#define INIT_TIMEOUT_ARG_SHORT    "-i"
#define INIT_TIMEOUT_ARG_LONG     "--init-timeout"
#define INIT_TRY_SLEEP_ARG_SHORT  "-I"
#define INIT_TRY_SLEEP_ARG_LONG   "--init-try-sleep"
#define SERIAL_PORT_ARG_SHORT     "-s"
#define SERIAL_PORT_ARG_LONG      "--serial-port"

static void print_help_message()
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

static int get_arg_index(
    int argc,
    char **argv,
    char *arg_short,
    char *arg_long
)
{
    UI i = 0;
    for(; i < (UI)argc; ++i)
    {
        if(
            strcmp(argv[i], arg_short) == 0 || 
            strcmp(argv[i], arg_long) == 0
        )
        {
            return i;
        }
    }

    return -1;
}

static int str_is_unsigned_number(const char *str)
{
    UI i = 0;
    for(; i < strlen(str); ++i)
    {
        if(!isdigit(str[i])) return 0;
    }

    return 1;
}

static void print_invalid_arg_error(char *arg)
{
    printf("Fatal error: Can't parse '%s' arg because it's value is invalid", arg);
}

UI check_for_help_arg(int argc, char **argv)
{
    int arg_index = get_arg_index(
        argc,
        argv,
        HELP_ARG_SHORT,
        HELP_ARG_LONG
    );

    if(arg_index > -1)
    {
        print_help_message();
        return ARG_OK;
    }

    return ARG_NOT_FOUND;
}

UI get_update_interval_arg(int argc, char **argv, UI *out)
{
    int arg_index = get_arg_index(
        argc,
        argv,
        UPDATE_INTERVAL_ARG_SHORT,
        UPDATE_INTERVAL_ARG_LONG
    );

    if(arg_index > -1)
    {
        /* if next arg can't be accessed return bad */
        if(arg_index + 1 > argc - 1)
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        const char *tmp = argv[arg_index + 1];
        
        /* if next arg isn't a number return bad */
        if(!str_is_unsigned_number(tmp))
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        *out = atoi(tmp);
        return ARG_OK;
    }

    return ARG_NOT_FOUND;
}

UI get_init_timeout_arg(int argc, char **argv, UI *out)
{
    int arg_index = get_arg_index(
        argc,
        argv,
        INIT_TIMEOUT_ARG_SHORT,
        INIT_TIMEOUT_ARG_LONG
    );

    if(arg_index > -1)
    {
        /* if next arg can't be accessed return bad */
        if(arg_index + 1 > argc - 1)
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        const char *tmp = argv[arg_index + 1];
        
        /* if next arg isn't a number return bad */
        if(!str_is_unsigned_number(tmp))
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        *out = atoi(tmp);        
        return ARG_OK;
    }

    return ARG_NOT_FOUND;
}

UI get_init_try_sleep_arg(int argc, char **argv, UI *out)
{
    int arg_index = get_arg_index(
        argc,
        argv,
        INIT_TRY_SLEEP_ARG_SHORT,
        INIT_TRY_SLEEP_ARG_LONG
    );

    if(arg_index > -1)
    {
        /* if next arg can't be accessed return bad */
        if(arg_index + 1 > argc - 1)
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        const char *tmp = argv[arg_index + 1];
        
        /* if next arg isn't a number return bad */
        if(!str_is_unsigned_number(tmp))
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }

        *out = atoi(tmp);        
        return ARG_OK;
    }

    return ARG_NOT_FOUND;
}

UI get_serial_port_arg(int argc, char **argv, char **out)
{
    int arg_index = get_arg_index(
        argc,
        argv,
        SERIAL_PORT_ARG_SHORT,
        SERIAL_PORT_ARG_LONG
    );

    if(arg_index > -1)
    {
        /* if next arg can't be accessed return bad */
        if(arg_index + 1 > argc - 1)
        {
            print_invalid_arg_error(argv[arg_index]);
            return ARG_BAD;
        }
        
        UI len = strlen(argv[arg_index + 1]);
        free(*out);
        *out = malloc(sizeof(char) * (len + 1));
        strcpy(*out, argv[arg_index + 1]);

        return ARG_OK;
    }

    return ARG_NOT_FOUND;
}
