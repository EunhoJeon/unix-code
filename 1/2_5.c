#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1

void main(){
	char buffer[BUFSIZE];
	int filedes;

	ssize_t nread;
	long word =0;
	long line =0;
	long flag =0;

	if((filedes=open("anotherfile", O_RDONLY))==-1){
		printf ("error in opening anotherfile \n");
		exit(1);}

	while((nread=read(filedes, buffer, BUFSIZE))>0){
		if(buffer[0]=='\t'){
			word+=1;
			}
		else if(buffer[0]=='\n'){
			word+=1;
			line+=1;
			}
		else if(buffer[0]==' '){
			word+=1;
			}
		}

	printf("total char in file: %ld\n line in file : %ld\n", word, line);
	exit(0);
}
