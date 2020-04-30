#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(){
	char *filename="afile";
	if(access(filename, R_OK)==-1){
		perror("Can't read file");
		exit(1);}
	printf("%s readable, proceeding\n", filename);

	if(access(filename, W_OK)==-1){
		perror("Can't write file");
		exit(1);}
	printf("%s writable, proceeding\n", filename);

	if(access(filename, X_OK)==-1){
		perror("Can't execute file");
		exit(1);}
	printf("%s executable, proceeding\n", filename);
}
