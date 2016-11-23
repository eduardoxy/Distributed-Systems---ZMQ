/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: eduardo
 *
 * Created on November 23, 2016, 3:11 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include "zmq.h"

int doRequest(const struct sockaddr_in *server, char *request, double *timeElapsed);
/*
 * 
 */
int main(int argc, char** argv)
{
    struct sockaddr_in      server;
    char                    *request;
    int                     success;
    double                  timeElapsed;
    int                     msgExp;
    char                    *host;
    
    success = 0;
    msgExp = 0;
    
    host = getIPAddressTCPServer(argc, argv);
    
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons( TCP_PORTDEFAULT );
    
    request = buildRequestExp(1);
    success = (!doRequest(&server, request, &timeElapsed) ? ++success : success);
    
    while (msgExp++ < 29)
    {
        request = buildRequestExp(msgExp);
        success = (!doRequest(&server, request, &timeElapsed) ? ++success : success);
    }
#ifdef DEBUG
    printf("%d successful messages!\n", success);
#endif

    return (EXIT_SUCCESS);
}

int doRequest(const struct sockaddr_in *server, char *request, double *timeElapsed)
{
    struct timeval          tStart;
    struct timeval          tEnd;
    unsigned long long int  size;
    unsigned long long int  sizeReq;
    int                     retCode;
    char                    *reply;
    int                     sock;

    //Create the socket and configuring the server
    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);
        
    // Connecting the socket to the server
    retCode = connect(sock, (struct sockaddr *) server, sizeof(*server));
    assert(retCode == 0);
    
    *(timeElapsed) = 0;
    sizeReq = strlen(request);

    // Sending the message
    gettimeofday(&tStart, NULL);
    retCode = tcp_send(sock, request, sizeReq);
    // Now the request is useless!
#ifndef WASTE_OF_MEMORY
    free(request);
#endif
    if (retCode != 0)
    {
#ifdef DEBUG
        printf("Error sending the message!\n");
#endif
        close(sock);
        return retCode;
    }
        
    reply = tcp_recv(sock, &size);
    gettimeofday(&tEnd, NULL);
    // Answer received!
    if (reply != NULL)
    {
        *(timeElapsed) = getDuration(&tStart, &tEnd);
#ifdef DEBUG
        printf("[%llu] Time Elapsed -> %.06f\n", sizeReq, *timeElapsed);
#else
        printf("%llu;%.06f\n", sizeReq, *timeElapsed);
#endif
        // Now the answer is useless!
#ifndef WASTE_OF_MEMORY
        free(reply);
#endif
    }
    close(sock);
    
    return 0;
}