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

void redirecin(char **cmd){
    int i, fd;
    int flag = 0;

    for(i=0;cmd[i]!=NULL; i++){
        if(!strcmp(cmd[i],"<")){
            flag = 1;
            break;
        }
    }

    if(flag == 1){
        if((fd=open(cmd[i+1],O_RDONLY|O_CREAT,0644))==-1){
            fatal("open error : ");
        }
    dup2(fd, STDIN_FILENO);
    close(fd);

    for(i=i;cmd[i]!=NULL; i++) cmd[i]=cmd[i+2];
    }

}

void redirecout(char **cmd){
    int i, fd;
    int flag = 0;

    for(i=0; cmd[i]!=NULL; i++){
        if(!strcmp(cmd[i],">")){
            flag = 1;
            break;
        }
    }

    if(flag == 1){
        if((fd = open(cmd[i+1],O_RDWR|O_CREAT|O_TRUNC, 0644))==-1){
            fatal("open error : ");
        }
    
    dup2(fd, STDOUT_FILENO);
    close(fd);

    for(i=i; cmd[i]!=NULL; i++) {
        cmd[i]=cmd[i+2];
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

        int count=makelist(cmdline, "|", cmdpipe, MAX_CMD_GRP);

        int fd[2];
        int pid;

        if(pipe(fd)==-1) fatal("pipe error");

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
            
            if(count>0){
            for(int j=0; j<count-1; j++) {
                int k=0;
                switch(pid=fork()){
                    case 0:
                    while(cmdvector[k]!=NULL){
                        char* arg ;
                        char *temp=cmdvector[k];
                        switch(*temp)
                            {
                            case '<':
                                arg = strtok(&cmdvector[k], " \t");
                                if( (fd[0] = open(cmdvector[k+1], O_RDONLY | O_CREAT, 0644)) < 0)
                                        fatal("file open error");
                                dup2(fd[0], STDIN_FILENO);
                                close(fd[0]);
                                close(fd[1]);
                                cmdvector[i] = '\0';
                                break;
                            case '>':
                                arg = strtok(&cmdvector[k], " \t");
                                if( (fd[1] = open(cmdvector[k+1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
                                    fatal("file open error");
                                dup2(fd[1], STDOUT_FILENO);
                                close(fd[1]);
                                close(fd[0]);
                                cmdvector[i] = '\0';
                            break;
                        default:
                            break;
                    }
                    k++;
                }

                    execvp(cmdvector[0],cmdvector);
                }
                }
            }
            redirecin(cmdvector);
            redirecout(cmdvector);
            
            execvp(cmdvector[0],cmdvector);
            fatal("1st fork child");

            case -1:
            fatal("1st fork error");

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
