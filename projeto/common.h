#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*
 * Header que define os protótipos de funções e estruturas que serão utilizadas
 * por todo o projeto.
 */

/*
 * ====== Constantes ======
 */
 #define MSG_SERVER_DEFAULT "A conexão foi feita com sucesso!\n"
 #define BUFFER_SIZE        1024
 #define QUEUE_SIZE         1

/*
 * Trata-se de uma estrutura que engloba a estrutura do socket e também
 * algumas flags.
 *
*/
 typedef struct {
     int  socketFD;              /* File descriptor do socket de conexão */
     bool is_Server;             /* Flag que define se é servidor */
     bool is_TCP;                /* Flag que define se o protocolo será TCP */
     struct sockaddr_in socket;  /* Estrutura do socket da conexão */

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
 void runAsServer(config*);

/*
 * Rotina que faz o programa rodar como cliente.
 */
 void runAsClient(config*);

/*
 * Rotina que apenas printa na tela quais são as configurações que o programa
 * irá rodar.
 */
 void print_config(config*);

#endif
