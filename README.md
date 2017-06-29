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

* Decidimos usar *write* e *read* ao invés de *send* *recv* 


## Limitações e Bugs

* O servidor não utiliza multithreading. Portanto, só consegue receber requisições de um usuário por vez.
* Ao sair de maneira drástica no protocolo UDP, o servidor continua rodando, esperando outra mensagem.
* Ao enviar uma mensagem com mais de 1024 caracteres, o servidor quebra a mensagem em 1024 caracteres
