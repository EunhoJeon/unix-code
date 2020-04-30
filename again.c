#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CMD_ARG 10
#define BUFSIZE 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZE];
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

int redirect(char **cmd){
    int i, fd;

    for(i=0;cmd[i]!=NULL; i++){
        if(!strcmp(cmd[i],"|")) break;
        if(!strcmp(cmd[i],"<")){
           if((fd=open(cmd[i+1],O_RDONLY|O_CREAT,0644))==-1){
                fatal("open error in ");
        }
            dup2(fd, STDIN_FILENO);
            close(fd);
            cmd[i]=NULL;
        }

        if(!strcmp(cmd[i],">")){
            if((fd = open(cmd[i+1],O_RDWR|O_CREAT|O_TRUNC, 0644))==-1){
                fatal("open error out ");
            }
    
            dup2(fd, STDOUT_FILENO);
            close(fd);
            cmd[i]=NULL;
        }
    }
    return i;
}

void pipe_cmd(char **cmd){
    pid_t pid;
    int p_fd[2];
    int cmd_i=redirect(cmd);

    if(cmd[cmd_i]==NULL){
        execvp(cmd[0],cmd);
        fatal("pipe not fork");
    }
    else if(!strcmp(cmd[cmd_i],"|")){
        if(pipe(p_fd)==-1) fatal("pipe error");

        switch(pid=fork()){
            case 0:
            dup2(p_fd[0],0);
            close(p_fd[0]);
            close(p_fd[1]);

            pipe_cmd(&cmd[cmd_i+1]);
            execvp(cmd[0],cmd);
            fatal("pipe fork 0");

            case 1:
            fatal("pipe fork -1");

            default:
            cmd[cmd_i]=NULL;

            dup2(p_fd[1],1);
            close(p_fd[0]);
            close(p_fd[1]);

            fatal("pipe default");
        }
    }
}


int main(int argc, char** argv){
    int i=0;
    pid_t pid;

    struct sigaction act;
    sigfillset(&(act.sa_mask));
    act.sa_handler = childdead;


    sigaction(SIGCHLD, &act, NULL);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN); 
    signal(SIGTTOU,SIG_IGN);    //new! 

    while(1){
        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZE, stdin);
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
                signal(SIGQUIT, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                setpgid(0,0);
            
                if(!strcmp("&",cmdvector[i-1])){
                    cmdvector[i-1]=(char*)0;
                }
                else{
                tcsetpgrp(STDIN_FILENO,getpgid(0));
                }
            
                pipe_cmd(cmdvector);
                execvp(cmdvector[0],cmdvector);
                fatal("main()0");

                case -1:
                fatal("main()-1");

                default:
                if(!strcmp("&",cmdvector[i-1])){ //background
                }
                else { //foreground
                    fpid=pid;
                    while(fpid!=0) pause();
                    tcsetpgrp(STDIN_FILENO,getpgid(0));
                    }
                }
    }
    return 0;
}