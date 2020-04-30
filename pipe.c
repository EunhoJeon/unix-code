#include <stdio.h>
#include <stdlib.h>
#include <error.h>


void fatal(char *str){
	perror(str);
	exit(1);
}

int join(char*com1[], char*com2[]){
	int p[2],status;

	switch (fork()){
		case -1:fatal("1st fork call in join");
		case 0:break;
		default: wait(&status);
			 return (status);
	}
	if (pipe(p)==-1) fatal("pipe call in join");

	switch(fork()){
		case -1:fatal("2nd fork call in join");
		case 0:
			dup2(p[1],1);
			close(p[0]);
			close(p[1]);
			execvp(com1[0],com1);
			fatal("1st execvp call in join");
		default:
			dup2(p[0],0);
			close(p[0]);
			close(p[1]);
			execvp(com2[0],com2);
			fatal("2nd execvp call in join");
	}
}

void main(){
	char *one[4] = {"ls", "-l", "/usr/lib",NULL};
	char *two[3]={"grep","^d",NULL};

	int ret;

	ret=join(one,two);
	printf("join returned %d\n", ret);
	exit(0);
}
