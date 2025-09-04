#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include"../include/include.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<linux/limits.h>

char *my_log(char *command,char * home_path){
	if(strncmp(command,"log ",4)!=0 && strcmp(command,"log")!=0)
		return NULL;
	int n = strlen(command);
	n--;
	while(command[n]==' ')
	n--;
	n++;
	char *file_data[15];
	for(int i=0;i<15;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char * dir = (char *)malloc(PATH_MAX);
	strcpy(dir,home_path);
	strcat(dir,"/file.txt"); // storing in a file 
	FILE *fp = fopen(dir, "r");
	if (!fp) {
        return NULL;
    }
	int file_count = 0;
	char line[4096];
	while (fgets(line, sizeof(line), fp) != NULL && file_count < 16) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';
        strcpy(file_data[file_count++], line);
    }
	fclose(fp);
	if(n == 3){
		for(int i=0;i<file_count ;i++)
			printf("%s\n",file_data[i]);
		return NULL;
	}
	int i=3;
	if(command[i] == ' ')
		i++;
	if(strcmp(command + i,"purge")==0){
		fp = fopen(dir,"w");
		fclose(fp);
		return NULL;
	}
	else if(strncmp(command+i,"execute",7)==0){
		while(command[i]!= ' ')
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
	char *file_data[15];
	for(int i=0;i<15;i++)
		file_data[i]= (char *)malloc(sizeof(char)*4097);
	char * dir = (char *)malloc(PATH_MAX);
	strcpy(dir,home_path);
	strcat(dir,"/file.txt");
	FILE *fp = fopen(dir, "r");
	int file_count = 0;
	if (!fp) {
		goto create;
    }
	
	char line[4096];
	while (fgets(line, sizeof(line), fp) != NULL && file_count < 16) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';
        strcpy(file_data[file_count++], line);
    }
	fclose(fp);

	for(int i=0;i<file_count;i++){
		if(strcmp(cmd,file_data[i])==0)
			return 1;
	}
	create:
	fp = fopen(dir,"w");
	if(file_count != 15){
		for(int i =0;i<file_count;i++){
			fprintf(fp,"%s\n",file_data[i]);
		}
		fprintf(fp,"%s\n",cmd);
		fclose(fp);
		return 1;
	}
	else{
		for(int i =1;i<file_count;i++){
			fprintf(fp,"%s\n",file_data[i]);
		}
		fprintf(fp,"%s\n",cmd);
		fclose(fp);
		return 1;

		}
	return 1;
}
