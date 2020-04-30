#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void catchint(int signo){
    printf("hello world!");
}

int main(){


    struct sigaction act;
    sigfillset(&(act.sa_mask));
    act.sa_handler = catchint;


    sigaction(SIGINT, &act, NULL);

    while(1);

    return 0;
}