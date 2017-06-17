#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/type.h>


int main(int argc, char const *argv[])
{
	int sockid;
	struct sockaddr_in addrport;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(5100);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport))!= -1) {}
}
