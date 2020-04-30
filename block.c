#include <stdio.h>
#include <signal.h>

void sig_int(int signo){
	printf("in SIGINT handler()\n");
	sleep(5);
}

int main(){
	signal(SIGINT, sig_int);
	printf("1 pause()\n");
	pause();
	printf("2 pause()\n");
	pause();
}

