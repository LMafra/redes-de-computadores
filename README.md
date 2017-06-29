![alt text](http://www.unb.br/images/Imagens/logo_unb.png)

# Redes de Computadores - 1/2017

## Membros
* Éden de Lucas Castelar Vale Medeiros - 12/0151898
* Lucas Mafra Chagas - 12/0126443

## Introdução

A implementação provê um serviço básico de chat através da linha de comando *nhem3*. O aplicativo monta e transmite pacotes entre o cliente e o servidor, e vice-versa dentro de uma sessão.Assim, o que for digitado em um lado será ecoado na tela do outro.

O programa é escrito na linguagem C, funcionando sobre o Sistema Operacional Linux.

## Funcionamento

Para rodar o servidor, você deverá apresentar as seguintes opções:

```c
   % nhem3 -S [port] [-t]
```

Já para o cliente, poderá ter as seguintes opções:

```c
   % nhem3 -host [port] [-t]
```

Onde temos que:

```c
   -S [port]
```
Refere-se ao modo servidor, escutando na porta *PORT*. Se não for especificado, usa-se a porta default *(33333)*.

```c
   -host [port]
```
*HOST* a ser conectado na porta *PORT*.

```c
   -t
```
Usar o protocolo TCP. Se não estiver presente, usar o protocolo UDP (default).


## Decisões de Projeto e Soluções Utilizadas

* Para criar uma abstração maior foi criada uma estrutura chamada *config* que encapsula o socket e algumas flags de configuração como: se a conexão é TCP e se o programa deve rodar como servidor. Com essa solução, foi possível centralizar as informações em comum entre o servidor e o cliente, assim deixando um código mais limpo.

## Compilação do Cdigo

* Para a compilação do código é necessário estar no diretório *projeto* e rodar o seguinte comando:

```c
   gcc nhem3.c -o nhem3
```

## Limitações e Bugs

* O servidor não utiliza multithreading. Portanto, só consegue receber requisições de um usuário por vez.
* Quando o cliente sai de maneira drástica no protocolo UDP sem avisar ao servidor, o servidor continua rodando pois não reconheceu que o cliente não se encontra mais conectado. Enquanto que no TCP essa detecção é feita.
* Quando o cliente sai e avisa ao servidor no protocolo UDP não há garantia que a mensagem vá ser entregue. Portanto, pode entrar no mesmo caso do bug anterior.
* Ao enviar uma mensagem com mais de 1024 caracteres, o servidor quebra a mensagem em 1024 caracteres
