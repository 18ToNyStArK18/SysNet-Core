#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fcntl.h"

int main(){
	char buf[1000];
	int i_count,f_count,fd;
	int bytes;
	i_count = getreadcount();
	printf("Initial count = %d\n",i_count);
	fd = open("lol.txt",O_CREATE | O_WRONLY);
	if(fd < 0){
		printf("Error Creating / opening file");
		exit(1);
	}
	write(fd,"....................................................................................................",100);
	close(fd);
	fd = open("lol.txt",O_RDONLY);

	bytes = read(fd,buf,100);
	printf("Bytes read = %d\n",bytes);
	close(fd);
	f_count = getreadcount();
	printf("Final count = %d\n difference = %d\n",f_count,f_count - i_count);

	if(f_count - i_count == bytes){
		printf("Worked\n");
	}
	else
		printf("Failed\n");

	exit(0);

}
