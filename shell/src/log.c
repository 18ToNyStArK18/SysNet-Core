#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include"../include/log.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<linux/limits.h>

//didnt implement this 
//Do not store a command if it is identical to the previously executed command in the log. Here identical can mean syntactically or exactly. Take it to mean exactly.


char *my_log(char *command){
	if(strncmp(command,"log ",4)!=0 && strcmp(command,"log")!=0)
		return NULL;
	int n = strlen(command);
	char *file_data[16];
	for(int i=0;i<16;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char *working_dir = (char *)malloc(sizeof(char)*PATH_MAX);
	if(getcwd(working_dir,PATH_MAX) == NULL){
		perror("Error finding the curr_dir\n");
	}
	working_dir = (char *)realloc(working_dir,strlen(working_dir) + 10);
	strcat(working_dir,"/file.txt");
	int fd = open(working_dir,O_CREAT | O_RDWR,0777);
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
		for(int i=0;i<file_count-1 && i < 15;i++)
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
			return file_data[command[i]-'0'-1];
		else
			return file_data[(command[i] - '0')*10 + command[i+1] - '0'-1];
	}
	return NULL;
}
int add_cmd(char *cmd){
	char *file_data[16];
	for(int i=0;i<16;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char *working_dir = (char *)malloc(sizeof(char)*PATH_MAX);
	if(getcwd(working_dir,PATH_MAX) == NULL){
		perror("Error finding the curr_dir\n");
	}
	working_dir = (char *)realloc(working_dir,strlen(working_dir) + 10);
	strcat(working_dir,"/file.txt");
	int fd = open(working_dir,O_CREAT | O_RDWR,0777);
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
	ftruncate(fd, 0);
	for(int i=0;i<file_count;i++){
		if(strcmp(cmd,file_data[i])==0)
			return NULL;
	}
	if(file_count != 16){
		for(int i =0;i<file_count-1;i++){
			write(fd,file_data[i],strlen(file_data[i]));
			write(fd,"\n",1);
		}
		write(fd,cmd,strlen(cmd));
		write(fd,"\n",1);
		write(fd,"0",1);
		return 1;
	}
	else{
		int oldest =-1;
		if(strlen(file_data[15]) == 1)
			oldest = file_data[15][0] - '0';
		else
			oldest = (file_data[15][0] - '0')*10 + file_data[15][1] - '0';
		strcpy(file_data[oldest],cmd);
		oldest = (oldest + 1)%15;
		for(int i =0;i<file_count-1;i++){
			write(fd,file_data[i],strlen(file_data[i]));
			write(fd,"\n",1);
		}
		
		char *old = (char *)malloc(sizeof(char)*3);
		if(oldest > 9){
			old[0] = '1';
			old[1] = oldest % 10 + '0';
			old[2]='\0';
		}
		else{
			old[0] = oldest + '0';
			old[1] = '\0';
		}
		write(fd,old,(strlen(old)));
		return 1;

		}
}
