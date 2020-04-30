#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	int mode =0;

	if(argc!=3){
		fprintf(stderr, "usage: %s file name, access mode.\n",argv[0]);
		exit(1);
	}

	if(access(argv[1],F_OK)!=0){
		printf("not existed file.\n");
		exit(1);}

	sscanf(argv[2],"0%o",&mode);

	if(chmod(argv[1],mode)!=0){
		printf("change mode failed.\n");
		exit(1);
	}
	else{
		printf("change mode successed!\n");
		exit(0);
	}
}
