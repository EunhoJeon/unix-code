#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define NAMELENGTH 4
#define NROOMS 10

char namebuf[NAMELENGTH];
int infile = -1;

char *getoccupier(int roomno){
	off_t offset;
	ssize_t nread;

	if(infile==-1 && (infile = open("residents", O_RDONLY))== -1) return NULL;

	offset=(roomno -1)*NAMELENGTH;

	if(lseek(infile, offset, SEEK_SET)==-1) return NULL;

	if((nread=read(infile, namebuf, NAMELENGTH))<=0) return NULL;
	else if(nread==0) return ("free");

	namebuf[nread-1] = '\0';
	return (namebuf);
}

int findfree(){
	int i;
	for(i=1;i<NROOMS;i++){
		if(strcmp(getoccupier(i),"free")==0) {
			printf ("Room %d is free.\n",i);
			return i;}
	}
	return 1;
}
void freeroom(int roomno){
	int i;
	off_t offset;
	int free;
	char buf[NAMELENGTH]="freeroom";

	if(free=open("residents",O_RDWR)==-1){
		printf("open fail\n");
		return NULL;}

	offset=(roomno-1)*NAMELENGTH;

	if(lseek(free,offset,SEEK_SET)==-1){
		printf("lseek error\n");
		return NULL;}
	else{
		for(i=0;i<NROOMS;i++) namebuf[i]=' ';
	}
	namebuf[NAMELENGTH-1]='\n';

	write(free, namebuf, NAMELENGTH); 
}

void addguest(char* guest){
	int roomno;
	int outfile;
	off_t offset;

	if(outfile=open("residents",O_WRONLY | O_CREAT, 0644)==-1){
		printf("open fail\n");
		return NULL;}

	roomno=findfree();

	offset = (roomno -1)*NAMELENGTH;
	
	if(lseek(outfile, offset, SEEK_SET)==-1){
		printf("lseek error\n");
		return NULL;}

	write(outfile, namebuf, NAMELENGTH);
}


void roominfo(){
	int j;
	char *getoccupier (int), *p;

	for(j=1;j<NROOMS;j++){
		if(p=getoccupier(j)) printf("Room %2d, %s\n",j,p);
		else printf("Error on room %d\n",j);
	}
}


void main(){
	int menu;
	int j;
	int room;

	char name[NAMELENGTH];

	while(1){
		printf("===========================\n");
		printf("1.room list\n");
		printf("2.free room\n");
		printf("3.add guest\n");
		printf("4.end the program");
		printf("===========================\n");

		scanf("%d", &menu);

		switch(menu){
			case 1:
				roominfo();
				break;
			case 2:
				printf("Room number?");
				scanf("%d", &room);
				freeroom(room);
				break;
			case 3:
				printf("Guest name?");
				scanf("%s",name);
				addguest(name);
				break;
			case 4:
				break;}
	}
}
