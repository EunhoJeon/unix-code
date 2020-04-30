#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void fpe(int _){
	printf("SIGFPE caught \n");
	exit(1);
}

int main(){
	int err;
	printf("Address of fpe(): %p \n",fpe);

	signal(SIGFPE, fpe);
	err= 4/0;

	return 0;}

