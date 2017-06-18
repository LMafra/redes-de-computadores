#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    config *c = recuperar_parametros(argc, argv);
    run(c);
    free(c);
}

void run(config *c){
    if(c == NULL){
        printf("ERRO: Não foi possível iniciar o programar!\n"
               "Certifique-se que:\n"
               "-> Para servidor use:\t -S [port][-t]\n"
               "-> Para cliente use:\t host [port][-t]\n");

        return;
    }

    // TODO: Avaliar se a criação do socket e o bind()
    // pode ser feito aqui e depois passado para as rotinas.
    // Pois aparenta ser código parecido entre os dois.

    print_config(c);
    if(c->is_Server){
        runAsServer(c);
    } else {
        runAsClient(c);
    }
}

void runAsServer(config* c){
    // a ser implementado.
}

void runAsClient(config* c){
    // a ser implementado.
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
        for( int i = 2; i < counter - 1; i++ ){

            // Foi passado um argumento númerico.
            // Possivelmente é o número da porta.
            if(isdigit(params[i]) == 0){
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
