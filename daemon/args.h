// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_ARGS_H
#define MEMDUINO_ARGS_H

#include<stdbool.h>
#define ARG_OK 0
#define ARG_BAD 1

typedef struct
{
    /* the name of the arg */
	char *name;
    /* short cli arg */
	char *arg_short;
    /* long cli arg */
	char *arg_long;
    char *value;
    /* 
    true if the arg isn't succedeed by a value 
    ex: '-h' opposed to '-n 2'.
    Any args that have this set to true
    should only be interfaced with using the 
    args_get_bool function.
    */
	bool is_flag;
} Arg;

typedef struct 
{
    Arg *args_arr;
    unsigned int argsc;
} Args;

void print_help_message();

/* init the args struct */
void args_init(Args *args);
/* free the args struct and reinit */
void args_free(Args *args);
/* add a new arg to the args struct */
void args_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    bool is_flag,
    Args *args
);
/* try to parse all of the added args */
void args_parse(int argc, char **argv, Args *args);

bool args_get_bool(const char *name, Args *args);
int args_out_str(const char *name, char **out, Args *args);
int args_out_uint(const char *name, unsigned int *out, Args *args);

#endif // MEMDUINO_ARGS_H
