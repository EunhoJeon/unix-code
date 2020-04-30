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

int freeroom(int roomno){
	int i;
	int free;
	off_t offset;
	char* buf;
	offset = (roomno-1)*NAMELENGTH;

	if(strcmp(getoccupier(roomno),"   ")==0) {
		printf("This is freeroom.\n");
	}
	else{
		buf = getoccupier(roomno);
		free = open("residents",O_WRONLY);
		
		for(i=0; i<(NAMELENGTH-1); i++) buf[i]=' ';
		buf[NAMELENGTH-1] = '\0';

		lseek(free,offset,SEEK_SET);
		write(free,buf,NAMELENGTH);
}
}
int addguest(int roomno, char* name){
	int i;
	int guest;
	off_t offset;
	char* buf;
	offset = (roomno-1)*NAMELENGTH;

	if(strcmp(getoccupier(roomno), "   ")==0){
		buf = getoccupier(roomno);
		guest = open("residents",O_WRONLY);
		
		for(i=0; i<(NAMELENGTH-1); i++) buf[i]=name[i];
		buf[NAMELENGTH-1]='\0';

		lseek(guest,offset,SEEK_SET);
		write(guest,buf,NAMELENGTH);
	}
	else printf("This room is not free.\n");}

void main(){
	int j;
	char newname[NAMELENGTH];
	int newroom;
	int room;
	int froom;

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
	printf("============Enter the Room Number===========\n");
	scanf("%d",&room);
	freeroom(room);
	printf("================Room List===================\n");

	for(j=1;j<=NROOMS;j++){
		if(p=getoccupier(j)) printf("Room %2d, %s\n", j, p);
		else printf("Error on room %d\n",j);
	}

	printf("======Enter the Room Number and Guest Name=======\n");
	scanf("%d %s",&newroom, newname);
	addguest(newroom, newname);

	for(j=1;j<=NROOMS;j++){
		if(p=getoccupier(j)) printf("Room %2d, %s\n", j, p);
		else printf("Error on room %d\n",j);
	}

}

