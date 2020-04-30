#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFF_SIZE 1000

struct stat sb;

void cmp(const char *name, time_t last){
	if(stat(name, &sb)==-1||sb.st_mtime!=last){
		printf("%s changed\n", name);

		char buff[BUFF_SIZE];
		int fd;
		ssize_t rd_size;
		if(0<(fd=open(name, O_RDONLY))){
			while(0<(rd_size=read(fd,buff, BUFF_SIZE-1))){
				buff[rd_size]='\0';
				puts(buff);
			}
			close(fd);}
		else printf("file open fail.\n");
		exit(1);
	}
}

int main(int argc, char* argv[]){
	int j;
	int fd;
	time_t last_time;


	if(argc<2){
		fprintf(stderr, "usage : lookout file.\n");
		exit(1);
	}
	if(--argc>1){
		fprintf(stderr, "lookout : too many filenames.\n");
		exit(1);
	}

	fd=open(argv[1], O_RDWR| O_CREAT, 0644);
	fd=close(fd);

	for(j=1; j<argc; j++){
	if(stat(argv[1], &sb) ==-1){
		fprintf(stderr, "lookout : couldn't stat %s\n", argv[j]);
		perror("stat");
		exit(1);
	}}
	last_time = sb.st_mtime;

	for(;;){
		cmp(argv[1], last_time);
		sleep(10);
	}
}
