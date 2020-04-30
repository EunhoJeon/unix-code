#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(){
	pid_t pid;
	pid=fork();
	if(pid>0){
		sleep(10);
		printf("You immoral! \n");
	}
	else if(pid==0){
		kill(getppid(), SIGKILL);
		printf("I killed parent \n");
	}
	return 0;
}

