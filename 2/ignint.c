#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

int main(){
	int i;
	printf("SIG-iGN : %p \n",SIG_IGN);

	signal (SIGINT, SIG_IGN);
	for(i=1;i<=10;++i){
		sleep(1);
		printf("Slept %d sec \n",i);
	}
	return 0;
}
