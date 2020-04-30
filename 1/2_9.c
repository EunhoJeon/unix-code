#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NAMELENGTH 4
#define NROOMS 10

char namebuf[NAMELENGTH];
int infile = -1;

char *getoccupier(int roomno){
	off_t offset;
	ssize_t nread;

	if(infile == -1 &&(infile = open("residents", O_RDONLY)) == -1){
		return (NULL);
	}

	offset = (roomno -1) * NAMELENGTH;

	if(lseek(infile, offset, SEEK_SET) == -1) {
		return (NULL);
	}

	if((nread = read(infile, namebuf, NAMELENGTH)) <= 0){
		return (NULL);
	}

	namebuf[nread -1] = '\0';
	return (namebuf);
}

int findfree(){
	int i;
	for(i = 1; i<=NROOMS; i++){
		if(strcmp(getoccupier(i),"   ")==0) {
			return (i);
	}
	}
	return (1);
}

void main(){
	int j;
	char *getoccupier (int), *p;

	for(j=1;j<=NROOMS;j++){
		if(p=getoccupier(j)){
			printf("Room %2d, %s\n", j, p);
			if(strcmp(getoccupier(j),"   ")==0){
				printf("This is freeroom.\n");
			}}
		else
			printf("Error on room %d\n",j);
	
	}
	int froom;
	froom = findfree();
	printf("%d is smallest number.\n",froom);
}

