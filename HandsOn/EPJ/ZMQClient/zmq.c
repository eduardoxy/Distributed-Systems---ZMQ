/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "zmq.h"

char *s_recv(void *socket, int *msg_size)
{
    zmq_msg_t   msg;
    int         size;
    
    zmq_msg_init(&msg);
    size = zmq_msg_recv(&msg, socket, 0);
    if (size == -1)
        return NULL;
    char *string = malloc((size + 1) * sizeof(char));
    memcpy(string, zmq_msg_data(&msg), size);
    zmq_msg_close(&msg);
    string[size] = 0;
    *(msg_size) = size;
    
    return (string);
}

int s_send(void *socket, char *string, int size)
{
    zmq_msg_t   msg;
    int         size_sent;
    
    zmq_msg_init_size(&msg, size);
    memcpy(zmq_msg_data(&msg), string, size);
    size_sent = zmq_msg_send(&msg, socket, 0);
    
    zmq_msg_close(&msg);
    
    return size_sent;
}

char *buildRequest(int mult)
{
    const char REQ_MSG[] = "Hi! I'm the client!\0";
    char *req;
    int  size;
    int  length;
    int  i;
    
    length = strlen(REQ_MSG);
    
    size = (sizeof (char)) * ((length * ++mult) + 1);
    req = malloc(size);
    for (i = 0; i < mult; i++)
        memcpy(&req[length * i], REQ_MSG, length);
    req[size - 1] = 0;

    return req;
}

char *buildReply(int repNbr)
{
    const char REP_MSG[] = "Hi! I'm the server and I have received your request successfully!\0";
    char *rep;
    int  size;
    int  length;
    
    length = strlen(REP_MSG);
    size = (sizeof (char)) * (length + 1);
    rep = malloc(size);
    memcpy(rep, REP_MSG, length);
    rep[size - 1] = 0;

    return rep;
}