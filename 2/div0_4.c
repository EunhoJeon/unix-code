#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void fpe(int unused){
	printf("Sigfpe caught \n");
	sleep(5);
}
int main(){
	int err;
	struct sigaction act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act, sa_mask, SIGINT);
	act.sa_flgs = s_SA_ONESHOT;
	act.sa_handler = fpe;

	sigaction(SIGFPE, &act, NULL);

	aee = 4/0;
	err = 4/0;

	return 0;}

