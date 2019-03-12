/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _L_LIST_H_
#define _L_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

typedef struct node {
    void *data;
    struct node *next;
} node_t; 

typedef struct l_list {
    node_t *head; 
} l_list_t; 


void list_init(l_list_t *); 

int list_len(l_list_t *); 

void list_append(l_list_t *, void *); 

int list_pop(l_list_t *, int); 

int list_pop_tail(l_list_t *); 

int list_get(l_list_t *, int, void **); 

int list_set(l_list_t *, int, void *); 

int list_index(l_list_t *, void *); 

int list_iter(l_list_t *, void **, int *); 

void list_extend(l_list_t *, l_list_t *); 

int list_swap(l_list_t *, int, int); 

void list_reverse(l_list_t *); 

void list_cycle(l_list_t *, void **, int *); 

int list_from_array(l_list_t *, void *, int, int); 

#ifdef __cplusplus
}
#endif

#endif
