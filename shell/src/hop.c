#include"../include/include.h"
#include<stdio.h>
#include<regex.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<linux/limits.h>
int hop(char *command,char *prev,char * home_path){
	char *temp = (char *)malloc(sizeof(char)*PATH_MAX);
	if(getcwd(temp,PATH_MAX) == NULL){
		perror("ERROR FINDING THE CURR_PATH");
	}

	int n = strlen(command);
	char *Home_dir= (char *)malloc(sizeof(char)*PATH_MAX);
	strcpy(Home_dir,home_path);
	if(n<3 || command[0] !='h' || command[1] != 'o' || command[2] !='p')
		return 0;
	int i=3;
	int flag=0;
	int temp_ = 3;
	if(temp_ <n && command[temp_] == ' ')
		temp_++;
	if(temp_ == n){
		
		strcpy(prev,temp);
		if(chdir(Home_dir)==-1)
				return -1;

	}
	for(i;i<n;i++){
		if(i<n && command[i] == ' ')
			i++;
		if(i<n && command[i] == '~'){

			if(chdir(Home_dir)==-1)
				return -1;
		}
		else if(i<n && command[i] == '-' && (i+1 == n || command[i+1] == ' ')){
			if(strlen(prev) == 0){
				return 1;
			}
			chdir(prev);
		}
		else{
			if(i<n && command[i] == '.' && (i+1 == n || command[i+1] == ' ')){
				if(chdir(".")==-1)
					return -1;
			}
			else if(i+1<n && command[i] == '.' && command[i+1] == '.' && (i+2 == n || command[i+2]==' ')){
				if(chdir("..")==-1)
					return -1;
			}
			else{
				char *temp =malloc(sizeof(char)*n);
				int temp_counter=0;
				while(i<n && command[i] != ' '){
					temp[temp_counter++] =command[i++]; 
				}
				temp[temp_counter]='\0';
				if(chdir(temp) == -1)
					return -1;

			}


		}

	}
	strcpy(prev,temp);
	return 1;
}
