#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    config *c = recuperar_parametros(argc, argv);
    run(c);
    free(c);
}

void run(config *c){
    if(c == NULL){
        printf("ERRO: Os parâmetros estão incorretos!\n"
               "Para servidor use: -S [port][-t]\n"
               "Para cliente use: host [port][-t]\n");

        return;
    }

    // TODO: Avaliar se a criação do socket e o bind
    // pode ser feito aqui e depois passado para as rotinas.
    // Pois se trata de um código parecido entre os dois.

    if(c->is_Server){
        runServer(c);
    } else {
        runClient(c);
    }
}

void runServer(config* c){
    // a ser implementado.
}

void runClient(config* c){
    // a ser implementado.
}

// TODO: Nessa implementação tem que ser passado todos os parâmetros.
// ajustar para que a porta e o -t sejam opcionais.
config* recuperar_parametros(int counter, char** params){

    if(counter != 4){
        return NULL;
    }

    config *ptr = (config *) malloc(sizeof(config));

    if(ptr == NULL){
        return ptr;
    }

    // TODO: Pensar em uma maneira de recuperar os parâmetros
    // independente da ordem que eles aparecam.
    ptr->is_Server = strcmp("-S", params[1]) == 0 ? true : false;
    // ptr->is_TCP    = strcmp("-t", params[3]) == 0 ? true : false;

    // Apenas para testes. Deverá ser removido !!
    ptr->is_TCP = true;

    ptr->socket.sin_family      = AF_INET;
    ptr->socket.sin_port        = htons(33333);
    ptr->socket.sin_addr.s_addr = INADDR_ANY;
    // ==========================================

    return ptr;
}
