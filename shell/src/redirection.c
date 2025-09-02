#include "../include/Input.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>	

int createfile(char * file){
	return 1;
}
int redirect(char*command){
	int n = strlen(command);
	/*  int std_in = dup(STDIN_FILENO);
		dup2(fd,STDIN_FILENO);
		close(fd);

	//code ends here
	dup2(std_in,STDIN_FILENO);
	close(std_in)
	*/

	char *refined = (char *)malloc(n +1);
	refined[0] = '\0';
	int i=0;
	while(i<n){
		int j=0;
		char *buff = (char *)malloc(n +1);
		buff[0]='\0';
		while( i < n && command[i] != '>' && command[i] != '<')
			buff[j++] = command[i++];
		buff[j]='\0';
		if(i<n && command[i]=='>' &&  ((i < n-1 && command[i+1] != '>') || i==n-1)){
			i++;
			if(j)
			strcat(refined,buff);
			strcat(refined,">");
			buff[0]='\0';
			j=0;
			while(i < n){
				if( i < n && command[i] == '<' || (i < n-1 && command[i] == '>' && command[i+1] == '>') ){
					buff[j] = '\0';
					strcat(refined,buff);
					j=0;
					break;
				}
				else if(command[i]=='>'){
					int a = createfile(buff);
					if(a == -1)
						return -1;
					buff[0]='\0';
					j=0;
					i++;
				}
				if(i<n){
					buff[j++] = command[i++];
				}
			}
			if(j){
				buff[j]='\0';
				strcat(refined,buff);
			}
		}
		else if(i<n-1 && command[i+1] == '>' && command[i] == '>'){
			i = i+2;
			buff[j]='\0';
			strcat(refined,buff);
			strcat(refined,">>");
			buff[0]='\0';
			j=0;
			while(i<n){
				if( i < n && command[i] == '<' ||( i < n-1 && (command[i] == '>' && command[i+1] != '>'))){
					buff[j]='\0';
					strcat(refined,buff);
					j = 0;
					break;
				}
				else if(i < n-1 && command[i]=='>' && command[i+1] =='>'){

					int a =  createfile(buff);
					if(a==-1)
						return -1;
					buff[0] = '\0';
					j = 0;
					i = i +2 ;
				}
				if(i<n)
					buff[j++] =  command[i++];
			}
			if(j){
				buff[j]='\0';
				strcat(refined,buff);
			}
		}
		else if(i < n  && command[i] == '<'){
			i++;
			buff[j]='\0';
			strcat(refined,buff);
			strcat(refined,"<");
			buff[0] = '\0';
			j=0;
			while(i < n){
				if(i<n && command[i] == '>'){
					buff[j] = '\0';
					strcat(refined,buff);
					j = 0;
					break;
				}
				else if(i < n && command[i] == '<'){
					buff[0] = '\0';
					j=0;
					i = i +1;
				}
				if(i<n)
					buff[j++] = command[i++];
			}
			if(j){
				buff[j]='\0';
				strcat(refined,buff);
			}
		}
		free(buff);
	}
	printf("%s\n",refined);
	return 1;
}
