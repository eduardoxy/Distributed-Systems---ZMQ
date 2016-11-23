/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "zmq.h"

unsigned long long int ipow(int base, int exp)
{
    unsigned long long int result = 1ULL;
    
    while (exp-- >= 1)
        result *= base;
    
    return result;
}

char *s_recv(void *socket, long long int *msg_size)
{
    zmq_msg_t       msg;
    long long int   size;
    
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

long long int s_send(void *socket, char *string, long long int size)
{
    zmq_msg_t       msg;
    long long int   size_sent;
    
    zmq_msg_init_size(&msg, size);
    memcpy(zmq_msg_data(&msg), string, size);
    size_sent = zmq_msg_send(&msg, socket, 0);
    
    zmq_msg_close(&msg);
    
    return size_sent;
}

char *buildRequest(int mult)
{
    //const char REQ_MSG[] = "Hi! I'm the client!\0";
    const char REQ_MSG[] = "a\0";
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

char *buildRequestExp(int exp)
{
    //const char REQ_MSG[] = "Hi! I'm the client!\0";
    const char              REQ_MSG[] = "AA\0";
    char                    *req;
    unsigned long int       size;
    unsigned long int       length;
    int                     i;
    
    length = strlen(REQ_MSG);
    size = ((sizeof(char)) * (ipow(length, exp))) + 1;
    req = malloc(size);
    memset(req, 0, size);
    
    memcpy(req, REQ_MSG, length);
    for (i = 1; i < exp; i++)
    {
        length = strlen(req);
        memcpy(&req[length], req, length);
    }
    req[size - 1] = 0;

    return req;
}

char *buildReply(void)
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

char *getIPAddress(int argc, char **argv)
{
    const char URL_PREFIX[] = "tcp://";
    char *url;
    char *host;
    char *port;
    int   length;
    int   size;
    
    if (argc == 2)
        host = argv[1];
    else
        host = IP_DEFAULT;
    port = PORT_DEFAULT;
    
    length = strlen(URL_PREFIX) + strlen(host) + 1 + strlen(port) + 1;
    size = (sizeof(char) * length);
    
    url = malloc(size);
    sprintf(url, "%s%s:%s", URL_PREFIX, host, port);
    
    return url;
}
