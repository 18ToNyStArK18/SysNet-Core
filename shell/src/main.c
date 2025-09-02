#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<sys/wait.h>
#include<linux/limits.h>
#include"../include/Parsing.h"
#include"../include/hop.h"
#include"../include/reveal.h"
#include"../include/log.h"
#include <fcntl.h>  
#include <unistd.h>
#include "../include/Input.h"
char * FindPath(){
	char *working_dir = (char *)malloc(sizeof(char)*PATH_MAX);
	if(getcwd(working_dir,PATH_MAX) == NULL){
		perror("Error finding the curr_dir\n");
	}
	char *Home_dir= (char *)malloc(sizeof(char)*PATH_MAX);
	struct passwd *inf = getpwuid(getuid());
	if(inf == NULL){
		perror("Error getting user info");
	}
	strcpy(Home_dir,inf->pw_dir);
	char *req_dir = (char * )malloc(sizeof(char*)*PATH_MAX);
	req_dir[0]='~';
	int h=strlen(Home_dir),w=strlen(working_dir);
	if(strncmp(Home_dir,working_dir,h)==0){
		for(int i = h;i<w;i++){
			req_dir[i - h + 1] = working_dir[i];
		}
		req_dir[w - h + 1] = '\0';
		return req_dir;
	}
	else
		return working_dir;
}	



int main(){
	char* username = getlogin();
	struct utsname sys;
	char *path_req = FindPath();
	char *prev =(char *)malloc(sizeof(char)*PATH_MAX);
	prev[0] = '\0';
	while(1){
		printf("<%s",username);
		if(uname(&sys)==0){
			printf("@%s:",sys.sysname);

		}
		printf("%s>",path_req);
		char *command=(char *)malloc(sizeof(char)*4097);
		char *cmd_refined=(char *)malloc(sizeof(char)*4097);


		scanf(" %[^\n]",command);
running:
		int counter = 0,i=0;
		int n = strlen(command);
		int flag = 0;
		while(i<n){
			flag = 0;
			if(command[i] == '"')
			{
				cmd_refined[counter++]='"';
				i++;
				while(i < n &&  i < command[i] != '"')
					cmd_refined[counter++] = command[i++];
				if(i !=n)
					cmd_refined[counter++] = command[i++];
			}
			while(command[i]==' '){
				i++;
				flag = 1;
			}
			if(flag)
				cmd_refined[counter++] = ' ';
			else
				cmd_refined[counter++] = command[i++];
		}
		if(cmd_refined[counter-1] == ' '){
			cmd_refined[counter-1] = '\0';
		}
		else
			cmd_refined[counter]='\0';

		if(!validate(cmd_refined)){
			printf("Invalid Syntax!\n");
			continue;
		}
		redirect(cmd_refined);

		if(strncmp(cmd_refined,"log",3) != 0)
			add_cmd(cmd_refined);

		if(strncmp(cmd_refined,"hop",3) == 0 && hop(cmd_refined,prev)==-1){
			printf("No such directory!\n");
			continue;
		}
		else if (strncmp(cmd_refined,"hop",3)==0){
			path_req = FindPath();
		}
		else if(strncmp(cmd_refined,"reveal",6) == 0){
			int temp = my_reveal(cmd_refined,prev);
			if(temp == -1){
				printf("No such directory!\n");
			}
		}
		else if(strncmp(cmd_refined,"log",3)==0){
			cmd_refined=my_log(cmd_refined);
			if(cmd_refined)
				goto running; 
		}
		else{
			int fk = fork();
			if (fk == 0) {
				int devnull = open("/dev/null", O_WRONLY);
				dup2(devnull, STDERR_FILENO);
				close(devnull);
				execlp("bash", "bash", "-c", cmd_refined, NULL);
				_exit(127);
			}
			else
				wait(NULL);
		}
	}
}

