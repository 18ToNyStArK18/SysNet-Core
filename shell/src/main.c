#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<linux/limits.h>
#include"../include/Parsing.h"
#include"../include/hop.h"
#include"../include/reveal.h"


#define BOLD    "\x1b[1m"
#define RESET   "\x1b[0m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
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
		printf("<" BOLD BLUE"%s"RESET,username);
		if(uname(&sys)==0){
			printf(BOLD BLUE"@%s"RESET":",sys.sysname);

		}
		printf(BOLD GREEN"%s"RESET">",path_req);
		char *command=(char *)malloc(sizeof(char)*1024);
		scanf(" %[^\n]",command);
		if(!validate(command)){
			printf("Invalid Syntax!\n");
			continue;
		}	
		if(hop(command,prev)==-1){
			printf("No such directory!\n");
			continue;
		}
		else{
			path_req = FindPath();
		}
		int temp = my_reveal(command,prev);
		if(temp == -1){
			printf("No such directory!\n");
		}
	
	}

}

