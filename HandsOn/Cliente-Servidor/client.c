#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main (void)
{
    time_t tstart, tfinish;
    clock_t start, finish;
    printf ("Connecting to ping server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    int request_nbr;
    char buffer [10];
    char send [20];
    strcpy(send, "sending msg: ");

    printf ("Iteration;Start;Finish;Time\n");
    for (request_nbr = 0; request_nbr != 100000; request_nbr++) {

        sprintf(send, "%d", request_nbr);

        tstart = time(0);
        start = clock();
        zmq_send (requester, "A simple msg", 13, 0);
        zmq_recv (requester, buffer, 10, 0);
        finish = clock();
        tfinish = time(0);

        printf ("%d;%f;%f;%f\n", request_nbr, ((double)(start))/CLOCKS_PER_SEC, ((double)(finish))/CLOCKS_PER_SEC, ((double)(finish - start))/CLOCKS_PER_SEC);
        //printf ("%d;%f;%f;%f\n", request_nbr, ((double)(tstart)), ((double)(tfinish)), ((double)(tfinish - tstart)));
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
