#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){

	if(argc!=2) fprintf(stderr,"remove file\n");	
	int user;

	if(access(argv[1],W_OK)==-1){
		printf("Do you want to remove this file?\n");
		printf("yes:1 no:0\n");
		scanf("%d",&user);
		if(user==0) exit(0);
	}

	if(unlink(argv[1])==-1){
		perror("unlink failed");
		exit(1);
	}

	printf("Succeeded!\n");
	exit(0);
}
