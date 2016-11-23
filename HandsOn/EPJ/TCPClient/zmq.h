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
#include <math.h>

//#define DEBUG
#ifndef DEBUG
#define PRODUCTION
#endif
//#define WASTE_OF_MEMORY

#define IP_DEFAULT      "localhost\0"
#define PORT_DEFAULT    "5555\0"
#define TCP_IPDEFAULT   "127.0.0.1\0"
#define TCP_PORTDEFAULT 5555
#define ACK_CHARACTER   '\t'

double                  getDuration(const struct timeval *tStart, const struct timeval *tEnd);
char                    *s_recv(void *socket, long long int *msg_size);
long long int           s_send(void *socket, char *string, long long int size);
char                    *buildRequest(int mult);
char                    *buildReply(void);
char                    *buildRequestExp(int exp);
char                    *getIPAddressTCPServer(int argc, char **argv);
char                    *getIPAddress(int argc, char **argv);
char                    *tcp_recv(int socket, unsigned long long int *sizeRead);
int                     tcp_send(int socket, char *buf, unsigned long long int length);


#ifdef __cplusplus
}
#endif

#endif /* ZMQ_H */

