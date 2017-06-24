#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv){
    config *c = recuperar_parametros(argc, argv);
    run(c);
    free(c);
}

void run(config *c){
    bool noError   = true;

    if(c == NULL){
        printf("ERRO: Não foi possível iniciar o programar!\n"
               "Certifique-se que:\n"
               "-> Para servidor use:\t -S [port][-t]\n"
               "-> Para cliente use:\t host [port][-t]\n");

        return;
    }

    print_config(c);
    if(noError){
        if(c->is_Server){
            runAsServer(c);
        } else {
            runAsClient(c);
        }
    }
}

void runAsServer(config* c){
    // TODO: Mudar para aceitar tanto TCP quanto UDP.
    int clientFD;
    char mensagem[BUFFER_SIZE];
    struct sockaddr_in client;
    socklen_t socketSize = sizeof(client);

    c->socketFD = socket(AF_INET, SOCK_STREAM, 0);

    bind(c->socketFD, (struct sockaddr *) &c->socket, sizeof(c->socket));
    listen(c->socketFD, QUEUE_SIZE);

    // recvfrom(c->socketFD, (void *) &mensagem, 1024, 0,
    // (struct sockaddr *) &c->socket, sizeof(c->socket));

    clientFD = accept(c->socketFD, (struct sockaddr *) &client, &socketSize);

    printf("Conexão recebida: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    write(clientFD, MSG_SERVER_DEFAULT, sizeof(MSG_SERVER_DEFAULT));

    close(clientFD);
}

void runAsClient(config* c){
    // TODO: Mudar para aceitar tanto TCP quanto UDP.
    int clientFD, clientSock;
    char mensagem[BUFFER_SIZE];
    struct sockaddr_in server;
    socklen_t socketSize = sizeof(server);

    clientSock = c->socketFD;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);


    clientFD = connect(clientSock, (struct sockaddr *) &server, &socketSize);

    printf("Olá, você está conectado \n");
    printf("Conexão recebida: %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
    
    write(clientFD, MSG_CLIENT_DEFAULT, sizeof(MSG_CLIENT_DEFAULT));

    close(clientFD);
}

config* recuperar_parametros(int counter, char** params){

    // Não foi passado o host ou -S.
    if( counter < 2 ){
        return NULL;
    }

    config *ptr = (config *) malloc(sizeof(config));

    if(ptr == NULL){
        return ptr;
    }

    ptr->is_Server         = strcmp("-S", params[1]) == 0 ? true : false;
    ptr->socket.sin_family = AF_INET;
    ptr->socket.sin_port   = htons(33333);

    // Recuperar parâmetros opcionais -t e/ou [porta].
    if( counter > 2 ){
        for( int i = 2; i < counter; i++ ){

            // Foi passado um argumento númerico.
            // Possivelmente é o número da porta.
            if(isdigit(atoi(params[i])) > 0){
                ptr->socket.sin_port = htons(atoi(params[i]));
            } else {
                ptr->is_TCP = strcmp("-t", params[i]) == 0 ? true : false;
            }

        }
    }

    ptr->socket.sin_addr.s_addr = ptr->is_Server ? INADDR_ANY
                                : inet_addr(params[2]); // FIXME: e se não for
                                                        // um ip válido ?!
    return ptr;
}

void print_config(config *c){
    printf("Inicializando programa como: %s\n"
           "IP: %s, Porta: %d\n"
           "Protocolo: %s\n\n",
           c->is_Server ? "Servidor" : "Cliente",
           inet_ntoa(c->socket.sin_addr),
           ntohs(c->socket.sin_port),
           c->is_TCP ? "TCP" : "UDP");
}
