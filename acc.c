#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];
pid_t fpid=0;

void fatal(char *str){
    perror(str);
    exit(1);
}
int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){
    int i=0;
    int numtokens=0;
    char *snew=NULL;

    if((s==NULL)||(delimiters==NULL)) return -1;

    snew = s +strspn(s, delimiters);
    if((list[numtokens]=strtok(snew, delimiters))==NULL) return numtokens;

    numtokens = 1;

    while(1){
        if((list[numtokens]=strtok(NULL, delimiters))==NULL) break;
        if(numtokens==(MAX_LIST-1)) return -1;
        numtokens++;
    }
    return numtokens;
}

void childdead(int signo){
    pid_t tempid;
    tempid=wait(NULL);

    if(tempid==fpid){
        fpid=0;
    }
}

int main(int argc, char** argv){
    int i=0;
    pid_t pid;

    while(1){
        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZ, stdin);
        cmdline[strlen(cmdline)-1]='\0';

        if(!strcmp("exit",cmdline)) exit(0);

        i=makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
        if(i==0) continue;

        if(!strcmp("cd",cmdvector[0])){
            chdir(cmdvector[1]);
            continue; 
        }

        switch(pid=fork()){
            case 0:

            if(!strcmp("&",cmdvector[i-1])){
                cmdvector[i-1]=(char*)0;
            }
            execvp(cmdvector[0],cmdvector);
            fatal("main()0");

            case -1:
            fatal("main()-1");

            default:
            wait(NULL);
        }
    }
    return 0;
}
