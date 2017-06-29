#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

/*
 * Trabalho de Redes: 01/2017
 *
 * Éden de Lucas Castelar Vale Medeiros - 12/0151898
 * Lucas Mafra Chagas - 12/0126443
 *
*/

int main(int argc, char **argv) {
    config *c = recuperar_parametros(argc, argv);

    run(c);

    close(c->socketFD);
    free(c);
}

void run(config *c) {
    if (c == NULL) {
        printf("ERRO: Não foi possível iniciar o programar!\n"
                       "Certifique-se que:\n"
                       "-> Para servidor use:\t -S [port][-t]\n"
                       "-> Para cliente use:\t host [port][-t]\n");

        return;
    }

    print_config(c);

    c->socketFD = socket(AF_INET, c->is_TCP ? SOCK_STREAM : SOCK_DGRAM, 0);

    if (c->socketFD == -1) {
        // workaround: Quando o servidor roda a partir da segunda vez, o socket pode não ter
        // sido liberado pelo OS ou se encontra em WAIT. Dessa maneira, tenta-se reutilizá-lo.
        if (c->is_Server) {
            setsockopt(c->socketFD, SOL_SOCKET, SO_REUSEADDR, (const void *) 1, sizeof(int));
        } else {
            printf("%s, código: %d", ERR_SOCKET, errno);
            return;
        }
    } else {
        printf(SCS_SOCKET);
    }

    if (c->is_Server) {
        runAsServer(c);
    } else {
        runAsClient(c);
    }
}

void runAsServer(config *c) {
    ssize_t bytesRead;
    int clientFD = 0;
    struct sockaddr_in client;
    socklen_t socketSize = sizeof(client);

    if (bind(c->socketFD, (struct sockaddr *) &c->socket, sizeof(c->socket)) == -1) {
        printf("%s código: %d\n", ERR_BIND, errno);
        return;
    } else {
        printf(SCS_BIND);
    }

    if (c->is_TCP) {

        if (listen(c->socketFD, QUEUE_SIZE) == -1) {
            printf(ERR_LISTEN);
            return;
        } else {
            printf(SCS_LISTEN);
        }

        clientFD = accept(c->socketFD, (struct sockaddr *) &client, &socketSize);

        printf("Servidor recebeu uma conexão de %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        write(clientFD, MSG_SERVER_DEFAULT, sizeof(MSG_SERVER_DEFAULT));
    }

    while (1) {

        if (c->is_TCP) {
            bytesRead = read(clientFD, c->mensagem, BUFFER_SIZE);
        } else {
            bytesRead = recvfrom(c->socketFD, c->mensagem, BUFFER_SIZE, 0, (struct sockaddr *) &client, &socketSize);
        }


        if (bytesRead == 0) {
            printf(WARM_CLIENT_LEFT);
            return;
        }

        printf("[%s:%d] : %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), c->mensagem);

        if (strcmp(KEYWORD_STOP, c->mensagem) == 0) {
            printf("Conexão encerrada por %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            break;
        }
    }

    if (c->is_TCP) {
        write(clientFD, MSG_CLOSE_CONNECT, sizeof(MSG_CLOSE_CONNECT));
        close(clientFD);
    } else {
        write(c->socketFD, MSG_CLOSE_CONNECT, sizeof(MSG_CLOSE_CONNECT));
    }

}

void runAsClient(config *c) {
    if (connect(c->socketFD, (struct sockaddr *) &c->socket, sizeof(c->socket)) == -1) {
        printf("%s codigo: %dz\n", ERR_CONNECT, errno);
        return;
    } else {
        printf(SCS_CONNECT);
    }

    // Mensagem inicial do servidor.
    if (c->is_TCP) {
        read(c->socketFD, c->mensagem, BUFFER_SIZE);
        printf("%s", c->mensagem);
        write(c->socketFD, MSG_CLIENT_DEFAULT, sizeof(MSG_CLIENT_DEFAULT));
    }

    while (1) {
        printf("Digite uma mensagem: ");
        fgets(c->mensagem, BUFFER_SIZE, stdin);

        // removendo \r da string.
        c->mensagem[strlen(c->mensagem) - 1] = '\0';

        if (c->is_TCP) {
            // enviando mensagem para o servidor
            write(c->socketFD, c->mensagem, BUFFER_SIZE);
        } else {
            sendto(c->socketFD, c->mensagem, BUFFER_SIZE, 0, (struct sockaddr *) &c->socket, sizeof(c->socket));
        }

        if (strcmp(KEYWORD_STOP, c->mensagem) == 0) {
            if (c->is_TCP) {
                read(c->socketFD, c->mensagem, BUFFER_SIZE);
                printf("%s\n", c->mensagem);
            }

            printf("Encerrando cliente...\n");
            break;
        }

    }
}


config *recuperar_parametros(int counter, char **params) {

    int portNumber;

    config *ptr = (config *) malloc(sizeof(config));

    if (ptr == NULL) {
        return ptr;
    }

    bzero(ptr->mensagem, BUFFER_SIZE);

    ptr->is_Server = strcmp("-S", params[1]) == 0 ? true : false;
    ptr->socket.sin_family = AF_INET;
    ptr->socket.sin_port = htons(33333);

    // Recuperar parâmetros opcionais -t e/ou [porta].
    if (counter > 2) {
        for (int i = 2; i < counter; i++) {

            if ((portNumber = atoi(params[i])) > 0) {

                if (portNumber < 1024 || portNumber > 65535) {
                    printf(ERR_PORT_RANGE);
                    free(ptr);
                    return NULL;
                }

                ptr->socket.sin_port = htons(atoi(params[i]));
            } else {
                ptr->is_TCP = strcmp("-t", params[i]) == 0 ? true : false;
            }

        }
    }

    ptr->socket.sin_addr.s_addr = ptr->is_Server ? INADDR_ANY
                                                 : inet_addr(params[1]);

    return ptr;
}

void print_config(config *c) {
    printf("Inicializando programa como: %s\n"
                   "IP: %s, Porta: %d\n"
                   "Protocolo: %s\n\n",
           c->is_Server ? "Servidor" : "Cliente",
           inet_ntoa(c->socket.sin_addr),
           ntohs(c->socket.sin_port),
           c->is_TCP ? "TCP" : "UDP");
}
