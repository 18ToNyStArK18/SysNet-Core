#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include"../include/include.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<linux/limits.h>

//didnt implement this 
//Do not store a command if it is identical to the previously executed command in the log. Here identical can mean syntactically or exactly. Take it to mean exactly.


char *my_log(char *command,char * home_path){
	if(strncmp(command,"log ",4)!=0 && strcmp(command,"log")!=0)
		return NULL;
	int n = strlen(command);
	n--;
	while(command[n]==' ')
	n--;
	n++;
	char *file_data[16];
	for(int i=0;i<16;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char * dir = (char *)malloc(PATH_MAX);
	strcpy(dir,home_path);
	strcat(dir,"/file.txt");
	int fd = open(dir,O_CREAT | O_RDWR,0777);
	char *buff = (char *)malloc(sizeof(char) * 4097 * 15);
	int file_count = 0;
	int bytes;

	while((bytes = read(fd, buff, 4096)) > 0) {
		buff[bytes] = '\0'; // null terminate
		char *line = strtok(buff, "\n");
		while(line != NULL && file_count < 16) {
			strcpy(file_data[file_count++], line);
			line = strtok(NULL, "\n");
		}
	}

	if(n == 3){
		for(int i=0;i<file_count ;i++)
			printf("%s\n",file_data[i]);
		return NULL;
	}
	int i=3;
	if(command[i] == ' ')
		i++;
	if(strcmp(command + i,"purge")==0){
		ftruncate(fd, 0);
		return NULL;
	}
	else if(strncmp(command+i,"execute",7)==0){
		while(command[i]!=  ' ')
			i++;
		if(command[i]== ' ')
			i++;
		if(i==n-1)
			return file_data[file_count - command[i]+'0'];
		else
			return file_data[file_count-((command[i] - '0')*10 + command[i+1] - '0')];
	}
	return NULL;
}
int add_cmd(char *cmd,char *home_path){
	char *file_data[16];
	for(int i=0;i<16;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char * dir = (char *)malloc(PATH_MAX);
	strcpy(dir,home_path);
	strcat(dir,"/file.txt");
	int fd = open(dir,O_CREAT | O_RDWR,0777);
	char *buff = (char *)malloc(sizeof(char) * 4097);
	int file_count = 0;
	int bytes;

	while((bytes = read(fd, buff, 4096)) > 0) {
		buff[bytes] = '\0'; // null terminate
		char *line = strtok(buff, "\n");
		while(line != NULL && file_count < 16) {
			strcpy(file_data[file_count++], line);
			line = strtok(NULL, "\n");
		}
	}

	lseek(fd, 0, SEEK_SET);
	for(int i=0;i<file_count;i++){
		if(strcmp(cmd,file_data[i])==0)
			return -1;
	}

	ftruncate(fd, 0);
	if(file_count != 15){
		for(int i =0;i<file_count;i++){
			write(fd,file_data[i],strlen(file_data[i]));
			write(fd,"\n",1);
		}
		write(fd,cmd,strlen(cmd));
		return 1;
	}
	else{
		for(int i =1;i<file_count;i++){
			write(fd,file_data[i],strlen(file_data[i]));
			write(fd,"\n",1);
		}
		write(fd,cmd,strlen(cmd));
		return 1;

		}
	return 1;
}
