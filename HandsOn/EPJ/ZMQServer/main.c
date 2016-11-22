/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: eduardo
 *
 * Created on November 22, 2016, 2:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zmq.h"

/*
 * 
 */
int main(int argc, char** argv) 
{
    void *context;
    void    *socket;
    int     retCode;
    char    *request;
    char    *reply;
    int     sizeRequest;
    int     successful;
    
    successful = 0;
    // Get a context
    context = zmq_ctx_new();
    // Open a reply socket
    socket = zmq_socket(context, ZMQ_REP);
    
    printf("Trying to start the ping server...\n");
    retCode = zmq_bind(socket, "tcp://*:5555");
    // Check if the bind was successful
    assert(retCode == 0);
    printf("Server is running\n");
    
    while (1)
    {
        request = s_recv(socket, &sizeRequest);
        printf("Request received!\n");
        
        reply = buildReply();
        retCode = s_send(socket, reply, strlen(reply));
        if (retCode == strlen(reply))
            successful++;
        else
            printf("Error sending the reply!");
    }
    
    // destroy the socket
    zmq_close(socket);
    // destroy the context.
    zmq_ctx_destroy(context);
    
    return EXIT_SUCCESS;
}

