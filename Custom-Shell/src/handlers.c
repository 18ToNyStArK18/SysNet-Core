#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include"../include/include.h"
#include<signal.h>
#include<unistd.h>
void kill_jobs(char *home_path){
	char file_path[1024];
	sprintf(file_path,"%s/proc.txt",home_path);
	FILE *fp = fopen(file_path,"r");
	if(!fp)
		return;
	char buffer[1024];
	while(fgets(buffer,1024,fp)!= NULL){
		int pid;
		sscanf(buffer,"%d",&pid);
		kill(pid,SIGKILL);
	}
	fclose(fp);
	fp = fopen(file_path,"w");
	fclose(fp);
	return;
}
