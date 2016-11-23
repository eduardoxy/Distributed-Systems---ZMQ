/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: eduardo
 *
 * Created on November 23, 2016, 2:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include "zmq.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    int                     sockDesc;
    int                     clientSock;
    int                     retCode;
    int                     sStruct;
    struct sockaddr_in      server;
    struct sockaddr_in      client;
    char                    *request;
    char                    *reply;
    unsigned long long int  sizeReceived;
    
     
    //Create socket
    sockDesc = socket(AF_INET , SOCK_STREAM , 0);
    assert(sockDesc != -1);
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( TCP_PORTDEFAULT );
     
    //Bind
    retCode = bind(sockDesc, (struct sockaddr *) &server, sizeof(server));
    assert(retCode >= 0);
    printf("TCP server running, awaiting connections\n");
    
    // Mark as a passive socket with max 3 clients in the queue.
    listen(sockDesc, 3);
    
    
    while (1)
    {
        sStruct = sizeof(struct sockaddr_in);
        clientSock = accept(sockDesc, (struct sockaddr *) &client, 
            (socklen_t *) &sStruct);
        assert(clientSock >= 0);
        printf("Connection accepted!\n");
        
        request = tcp_recv(clientSock, &sizeReceived);
        if (request != NULL)
        {
            free(request);
            
            printf("Request received -> Size %llu -> Reply ", sizeReceived);
            reply = buildReply();
            retCode = tcp_send(clientSock, reply, strlen(reply));
            free(reply);
            printf("%s\n", (retCode == 0 ? "sent successfully!" : 
                "fail!!"));
        }
        close(clientSock);
    }
    
    fflush(stdout);
    close(sockDesc);
    
    return EXIT_SUCCESS;
}
