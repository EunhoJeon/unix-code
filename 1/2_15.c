#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 512

void main(int argc, char* argv[]){
	ssize_t nread;
	int fd;
	char buf[SIZE];

	if(argc>1){
		if((fd = open(argv[1],O_RDWR))==-1) {
			printf("open error\n");
			exit(1);
		}
		
		while((nread = read(fd, buf, SIZE))>0) write (1,buf,nread);

	}

	else {
	while((nread = read(0,buf,SIZE))>0)
		write(1,buf,nread);
	}
	exit(0);
}
