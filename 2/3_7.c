#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int lsoct(char* mode){
	int* perms[4]={0};
	int temp=0;

	for(int i=0;i<9;i++){
		if(mode[i]=='r') temp+=4;
		else if(mode[i]=='w') temp+=2;
		else if(mode[i]=='x') temp+=1;
		else temp+=0;

		if(i%3==2){
			perms[i/3+1]=temp;
			temp=0;
		}
		}
	return (int*)perms;
}

int main(int argc, char *argv[]){
	if(argc!=3) {
		fprintf(stderr, "error");
		exit(1);
	}
	if(open(argv[1],O_RDONLY)==-1){
		perror("can't read file.\n");
		exit(1);}
	if(strlen(argv[2])==9) lsoct(argv[2]);
	else if(strlen(argv[2])!=4){
		fprintf("error");
		exit(0);
	}
	else strcpy(oc,argv[2]);

	long i=strtol(oc,0,8);
	if(chmod(argv[1],i)==-1) perror("chmod failed\n");
}
