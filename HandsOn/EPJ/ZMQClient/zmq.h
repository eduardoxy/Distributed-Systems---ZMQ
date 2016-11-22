/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq.h
 * Author: eduardo
 *
 * Created on November 22, 2016, 2:40 PM
 */

#ifndef ZMQ_H
#define ZMQ_H

#ifdef __cplusplus
extern "C" {
#endif

/*! Libraries .*/
#include <zmq.h>
#include <string.h>
#include <stdlib.h>

char    *s_recv(void *socket, int *msg_size);
int     s_send(void *socket, char *string, int size);
char    *buildRequest(int mult);

#ifdef __cplusplus
}
#endif

#endif /* ZMQ_H */

