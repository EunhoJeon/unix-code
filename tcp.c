#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LENGTH INET_ADDRSTRLEN

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

void error_handle(char *msg) {
	perror(msg);
	exit(2);
}

int main(int argc, char **argv){
	int s_sock, c_sock;
	sockaddr_in s_addr, c_addr;
	int c_size;
	char message[] = "Hello, World!\n";

	if(argc != 2){
		printf("Usage : %s <port> \n", argv[0]);
		return 1;}

	s_sock = socket(PF_INET, SOCK_STREAM, 0);

	if(s_sock==-1)
		error_handle("socket() error");

	memset(&s_addr, 0, LENGTH);
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(atoi(argv[1]));

	if(bind(s_sock,(sockaddr*) &s_addr, LENGTH) ==-1)
		error_handle("bind() error");

	if(listen(s_sock, 5) ==-1)
		error_handle("listen() error");

	c_size=LENGTH;
	c_sock=accept(s_sock, (sockaddr*) &c_addr, &c_size);

	if(c_sock ==-1)
		error_handle("accept() error");
	printf("Connected from %s\n",inet_ntoa(c_addr.sin_addr));
	write(c_sock,message,sizeof(message));
	close(c_sock);
	close(s_sock);
	return 0;
}
