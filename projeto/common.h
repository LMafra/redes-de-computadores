#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*
 * Header que define os protótipo de funções e estruturas que serão utilizadas
 * por todo o projeto.
 */

 // forward declaration
 struct sockaddr_in;

/*
 * Trata-se de uma estrutura que engloba a estrutura do socket e também
 * algumas flags que serão utilizadas pelas rotinas runServer() e runClient().
 *
*/
 typedef struct {

     bool is_Server;             /* Flag que define se é servidor */
     bool is_TCP;                /* Flag que define se o protócolo será TCP */
     struct sockaddr_in socket;  /* Socket da conexão */

 } config;

/*
 * Recupera os parâmetros passados por linha de comando e retorna em uma
 * estrutura que os engloba. Caso, ocorra algum erro na recuperação dos
 * parâmetros ou esteja faltando algum parâmetro será retornado NULL.
 */
 config* recuperar_parametros(int, char**);

/*
 * Função que abstrai a inicialização da aplicação. Nela que é decidido se o
 * programa rodará como servidor ou cliente.
 */
 void run(config*);

/*
 * Rotina que faz o programa rodar como servidor.
 */
 void runServer(config*);

/*
 * Rotina que faz o programa rodar como cliente.
 */
 void runClient(config*);

#endif
