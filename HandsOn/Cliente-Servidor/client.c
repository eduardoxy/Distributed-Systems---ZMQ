#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main (void)
{
	struct timeval tini, tfim;

    printf ("Connecting to ping server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://31.220.108.114:5555");
    //zmq_connect (requester, "tcp://localhost:5555");

    int request_nbr;
    char buffer [10];
    char send [20];
    strcpy(send, "sending msg: ");

    printf ("Iteration;Start;Finish;Time\n");
    for (request_nbr = 0; request_nbr != 100000; request_nbr++) {

        sprintf(send, "%d", request_nbr);

        //tstart = time(0);
        //start = clock();
        gettimeofday(&tini, NULL);
        zmq_send (requester, "A simple msg", 13, 0);
        zmq_recv (requester, buffer, 10, 0);
        printf("Msg: %s", buffer);
        //finish = clock();
        //tfinish = time(0);
        gettimeofday(&tfim, NULL);

        //printf ("%d;%f;%f;%f\n", request_nbr, ((double)(start))/CLOCKS_PER_SEC, ((double)(finish))/CLOCKS_PER_SEC, ((double)(finish - start))/CLOCKS_PER_SEC);
        //printf ("%d;%f;%f;%f\n", request_nbr, ((double)(tstart)), ((double)(tfinish)), ((double)(tfinish - tstart)));
        printf("time = %.6f\n", tfim.tv_sec - tini.tv_sec + (tfim.tv_usec - tini.tv_usec) * 1e-6);
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
