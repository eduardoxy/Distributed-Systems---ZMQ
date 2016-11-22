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

/*
 * 
 */
int main(int argc, char** argv) 
{
    struct timeval  tstart;
    struct timeval  tend;
    char           *answer;
    char           *request;
    int             size;
    int             expectedSize;
    void           *context;
    void           *socket;
    int             success;
    
    success = 0;
    memset(&tstart, 0, sizeof(tstart));
    memset(&tend, 0, sizeof(tend));
    
    // Get a context
    context = zmq_ctx_new();
    // Open a request socket
    socket = zmq_socket(context, ZMQ_REQ);
    
    
    printf("Trying to connect to ping server...\n");
    // Connect to the server
    zmq_connect(socket, "tcp://localhost:5555");
    printf("Connection established\n");
    
    while (success < 10000)
    {
        // Sending the message
        request = buildRequest(success);
        expectedSize = strlen(request);
        size = s_send(socket, request, expectedSize);
        if (size == expectedSize)
            success++;
        else
            printf("Error sending the message!\n");
        answer = s_recv(socket, &size);
        if (answer != NULL)
            printf("Answer: %s\n", answer);
    }
    printf("%d successful messages!\n", success);
    
    // Close the socket
    zmq_close(socket);
    // Destroy the context
    zmq_ctx_destroy(context);
    
}
