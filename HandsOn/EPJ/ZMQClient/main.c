/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: eduardo
 *
 * Created on November 22, 2016, 10:29 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "zmq.h"


int doRequest(void *socket, char *request, double *timeElapsed);
double getDuration(const struct timeval *tStart, const struct timeval *tEnd);
/*
 * 
 */
int main(int argc, char** argv) 
{
    char           *request;
    char           *url;
    void           *context;
    void           *socket;
    int             success;
    int             msgExp;
    double          timeElapsed;
    
    success = 0;
    msgExp = 0;
    
    url = getIPAddress(argc, argv);
    
    // Get a context
    context = zmq_ctx_new();
    // Open a request socket
    socket = zmq_socket(context, ZMQ_REQ);
    
#ifdef DEBUG
    printf("Connecting to ping server on [%s]\n", url);
#endif
    // Connect to the server
    zmq_connect(socket, url);
#ifdef DEBUG
    printf("Connection established\n");
#else
    printf("DataSize(Bytes);Time Elapsed\n");
#endif

    request = buildRequestExp(1);
    success = (!doRequest(socket, request, &timeElapsed) ? ++success : success);
    
    while (msgExp++ < 29)
    {
        // Prepare the message
        request = buildRequestExp(msgExp);
        success = (!doRequest(socket, request, &timeElapsed) ? ++success : success);
    }
#ifdef DEBUG
    printf("%d successful messages!\n", success);
#endif
    
    // Close the socket
    zmq_close(socket);
    // Destroy the context
    zmq_ctx_destroy(context);
    
}

int doRequest(void *socket, char *request, double *timeElapsed)
{
    struct timeval  tStart;
    struct timeval  tEnd;
    long long int   size;
    long long int   expectedSize;
    int             retCode;
    char            *answer;
    
    *(timeElapsed) = 0;
    expectedSize = strlen(request);
    // Sending the message
    gettimeofday(&tStart, NULL);
    size = s_send(socket, request, expectedSize);
#ifndef WASTE_OF_MEMORY
    // Now the request is useless!
    free(request);
#endif
    retCode = !(size == expectedSize);
    if (retCode != 0)
    {
#ifdef DEBUG
        printf("Error sending the message!\n");
#endif
        return retCode;
    }
        
    answer = s_recv(socket, &size);
    gettimeofday(&tEnd, NULL);
    // Answer received!
    if (answer != NULL)
    {
        *(timeElapsed) = getDuration(&tStart, &tEnd);
#ifdef DEBUG
        printf("[%llu] Time Elapsed -> %.06f\n", expectedSize, *timeElapsed);
#else
        printf("%llu;%.06f\n", expectedSize, getDuration(&tStart, &tEnd));
#endif
    }
#ifndef WASTE_OF_MEMORY
    // Now the request is useless!
    free(answer);
#endif
    
    return 0;
}

