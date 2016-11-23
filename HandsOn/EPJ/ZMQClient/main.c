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


double getDuration(const struct timeval *tStart, const struct timeval *tEnd);

/*
 * 
 */
int main(int argc, char** argv) 
{
    struct timeval  tStart;
    struct timeval  tEnd;
    char           *answer;
    char           *request;
    char           *url;
    long long int   size;
    long long int   expectedSize;
    void           *context;
    void           *socket;
    int             success;
    int             msgExp;
    
    success = 0;
    msgExp = 0;
    
    url = getIPAddress(argc, argv);
    
    // Get a context
    context = zmq_ctx_new();
    // Open a request socket
    socket = zmq_socket(context, ZMQ_REQ);
    
    
    //printf("Trying to connect to ping server on [%s]\n", url);
    // Connect to the server
    zmq_connect(socket, url);
    //printf("Connection established\n");
    printf("DataSize(2^x);Time Elapsed\n");
    while (msgExp++ < 30)
    {
        // Prepare the message
        request = buildRequestExp(msgExp);
        expectedSize = strlen(request);

        // Sending the message
        gettimeofday(&tStart, NULL);
        size = s_send(socket, request, expectedSize);
        if (size == expectedSize)
            success++;
        else
            printf("Error sending the message!\n");
        // Now the request is useless!
        free(request);
        
        answer = s_recv(socket, &size);
        gettimeofday(&tEnd, NULL);
        // Answer received!
        if (answer != NULL)
        {
            printf("%d;%.09f\n", success, getDuration(&tStart, &tEnd));
            //printf("[%09d] Time used -> %.06f\n", success, getDuration(&tStart, &tEnd));
        }
        // Now the answer is useless!
        free(answer);
    }
    //printf("%d successful messages!\n", success);
    
    // Close the socket
    zmq_close(socket);
    // Destroy the context
    zmq_ctx_destroy(context);
    
}

double getDuration(const struct timeval *tStart, const struct timeval *tEnd)
{
    if ((tStart == NULL) || (tEnd == NULL))
        return -1;
    else
        return (tEnd->tv_sec - tStart->tv_sec) + 
            (tEnd->tv_usec - tStart->tv_usec) * 1e-6;
}
