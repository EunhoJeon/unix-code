#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t fpid; //foreground pid 

void catchint(int signo){
    pid_t tpid; //temp pid
    tpid=wait(NULL);

    if(fpid==tpid){
        printf("God Chanjun!!\n");
    }
}

int main(){

    pid_t pid;

    struct sigaction act;
    sigfillset(&(act.sa_mask));
    act.sa_handler = catchint;


    sigaction(SIGCHLD, &act, NULL);

    switch(pid=fork()){
        case 0:
            sleep(10);
            return 0;
        case -1:
        break;
        default:
            fpid = pid;
            pause();
            break;
    }

    return 0;
}