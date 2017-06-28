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
 * ================== Constantes ==================
 */

#define BUFFER_SIZE     1024
#define QUEUE_SIZE      3
#define KEYWORD_STOP    "quit"

#define SCS_SOCKET      "-> Socket foi criado...\n"
#define SCS_BIND        "-> Foi dado bind no socket...\n"
#define ERR_SOCKET      "ERRO: Não foi possível criar o socket!"
#define ERR_BIND        "ERRO: Não foi possível dar bind no socket!"

#define ERR_PORT_RANGE  "ERRO: A porta deve ser maior que 1024 " \
                         "e menor que 65535!\n"

/*
 * ---- Servidor ----
 */

#define MSG_SERVER_DEFAULT    "SERVIDOR: Digite 'quit' para encerrar a conexão!\n"
#define MSG_CLOSE_CONNECT     "SERVIDOR: Recebi sua mensagem! Estou finalizando a conexão..."

#define SCS_LISTEN            "-> Esperando conexões...\n"
#define ERR_LISTEN            "ERRO: Não foi possível escutar conexões no socket!\n"
#define WARM_CLIENT_LEFT      "-> Opa... Parece que o cliente saiu e não avisou... " \
                              "Encerrando servidor."

/*
 * ---- Cliente ----
 */
#define ERR_CONNECT           "ERRO: Não foi possível se conectar!\n"
#define SCS_CONNECT           "-> Cliente se conectou...\n"

#define MSG_CLIENT_DEFAULT    "Olá! Desejo conversar."
/*
 * Trata-se de uma estrutura que engloba a estrutura do socket e também
 * algumas flags.
 *
*/
typedef struct {
    int socketFD;               /* File descriptor do socket de conexão              */
    char mensagem[BUFFER_SIZE]; /* Buffer que será utilizado para troca de mensagens */
    bool is_Server;             /* Flag que define se é servidor                     */
    bool is_TCP;                /* Flag que define se o protocolo será TCP           */
    struct sockaddr_in socket;  /* Estrutura do socket da conexão                    */

} config;

/*
 * Recupera os parâmetros passados por linha de comando e retorna em uma
 * estrutura que os engloba. Caso, ocorra algum erro na recuperação dos
 * parâmetros ou esteja faltando algum parâmetro será retornado NULL.
 */
config *recuperar_parametros(int, char **);

/*
 * Função que abstrai a inicialização da aplicação. Nela que é decidido se o
 * programa rodará como servidor ou cliente.
 */
void run(config *);

/*
 * Rotina que faz o programa rodar como servidor.
 */
void runAsServer(config *);

/*
 * Rotina que faz o programa rodar como cliente.
 */
void runAsClient(config *);

/*
 * Rotina que apenas printa na tela quais são as configurações que o programa
 * irá rodar.
 */
void print_config(config *);

#endif
