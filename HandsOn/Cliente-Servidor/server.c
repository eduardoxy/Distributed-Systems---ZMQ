#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    /* Todas aplicações 0mq iniciam pela criação do contexto, que em seguida será utilizado para criar o socket.
    / O contexto é um container para todos os sockets em um processo simples. */
    void *context   = zmq_ctx_new ();

    /* void *zmq_socket (void *context, int type);
    Cria efetivamente um 0mq socket dentro do contexto (passado no primeiro parâmetro), retornando um "opaque handle" para o socket recém criado.
    O parâmetro type define o tipo de conexão que se deseja realizar, podendo esta ser do tipo:
        ** descrever cada um individualmente **
        PUB and SUB 
        REQ and REP
        REQ and ROUTER (take care, REQ inserts an extra null frame)
        DEALER and REP (take care, REP assumes a null frame)
        DEALER and ROUTER
        DEALER and DEALER
        ROUTER and ROUTER
        PUSH and PULL
        PAIR and PAIR 
    O socket é efetivamente criado, mas não desconectado até que sejam definidos todos endpoints.
    ** Retornos **
    */
    void *responder = zmq_socket (context, ZMQ_REP);

    /*
    int zmq_bind (void *socket, const char *endpoint);
    Cria um endpoint que aceita conexões no socket referenciado no primeiro parâmetro. 
    O endpoint é definido pelo segundo parâmetro e consiste em protocolo de transporte://endereço
    ** Retornos **
    */
    int rc = zmq_bind (responder, "tcp://*:5555");

    // Caso tenha havido algum erro (já tem alguém nesta porta) retorna erro na saída padrão e aborta.
    assert (rc == 0);

    /* Faz um loop infinito (fritando a CPU) aguardando recebimento de mensagens */
    while (1) {
        char buffer [16];

        /* int zmq_recv (void *socket, zmq_msg_t *msg, int flags); 
        Recebe a mensagem do socket.
        */

        zmq_recv (responder, buffer, 16, 0);

        printf("Received %s\n", buffer);

        //sleep (1);          //  Do some 'work'
        zmq_send (responder, "World", 5, 0);
    }
    return 0;
}
