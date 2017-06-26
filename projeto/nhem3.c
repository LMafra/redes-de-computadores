#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

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
    int clientFD = 0;
    char mensagem[BUFFER_SIZE];
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
        bzero(mensagem, BUFFER_SIZE);

        if (c->is_TCP) {
            read(clientFD, mensagem, BUFFER_SIZE);
            mensagem[strlen(mensagem) - 2] = '\0';  // remover \r \n da string
        } else {
            recvfrom(c->socketFD, mensagem, BUFFER_SIZE, 0, (struct sockaddr *) &client, &socketSize);
            mensagem[strlen(mensagem) - 1] = '\0';  // remover \n da string
        }

        printf("[%s:%d] : %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), mensagem);

        if (strcmp(KEYWORD_STOP_SERVER, mensagem) == 0) {
            printf("Conexão encerrada por %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            break;
        }
    }

    if (c->is_TCP) {
        close(clientFD);
    } else {
        // avisar para o cliente que o servidor vai parar.
        write(c->socketFD, KEYWORD_STOP_SERVER, sizeof(KEYWORD_STOP_SERVER));
    }

}

void runAsClient(config *c) {
    int client = 0;
    char mensagem[BUFFER_SIZE];
    struct sockaddr_in server;
    socklen_t socketSize = sizeof(c->socket);

    if (c->is_TCP) {

        client = connect(c->socketFD, (struct sockaddr *) &c->socket, socketSize);

        printf("Olá, você está conectado \n");

        write(c->socketFD, MSG_CLIENT_DEFAULT, sizeof(MSG_CLIENT_DEFAULT));

        printf("Conexão recebida: %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

        while (1) {
            bzero(mensagem, BUFFER_SIZE);

            if (c->is_TCP) {
                write(client, mensagem, BUFFER_SIZE);
                mensagem[strlen(mensagem) - 2] = '\0';  // remover \r \n da string
            } else {
                sendto(c->socketFD, mensagem, BUFFER_SIZE, 0, (struct sockaddr *) &c->socket, &socketSize);
                mensagem[strlen(mensagem) - 1] = '\0';  // remover \n da string
            }

            printf("[%s:%d] : %s\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port), mensagem);

            if (strcmp(KEYWORD_STOP_SERVER, mensagem) == 0) {
                printf("Conexão encerrada por %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
                break;
            }
        }

        if (c->is_TCP) {
            close(client);
        } else {
            // avisar para o cliente que o servidor vai parar.
            read(c->socketFD, KEYWORD_STOP_SERVER, sizeof(KEYWORD_STOP_SERVER));
        }
    }
}

config *recuperar_parametros(int counter, char **params) {

    int portNumber;

    config *ptr = (config *) malloc(sizeof(config));

    if (ptr == NULL) {
        return ptr;
    }

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
                                                 : inet_addr(params[1]); // FIXME: e se não for
    // um ip válido ?!
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
