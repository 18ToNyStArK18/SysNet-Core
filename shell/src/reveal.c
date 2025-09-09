#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<linux/limits.h>
#include"../include/include.h"
#include<dirent.h>

int cmp(const void *a, const void *b) {
    const char *pa = *(const char * const *)a;
    const char *pb = *(const char * const *)b;
    return strcmp(pa, pb);
}

int my_reveal(char *command,char *prev,char * home_path){
	if(strncmp(command,"reveal ",7) != 0 && strcmp(command,"reveal") != 0){
		return 0;
	}
	int n = strlen(command);
	n = n-1;
	while(command[n] == ' ')
		n--;
	char dir[PATH_MAX];
	strcpy(dir,".");
	n++;
	int a=0,l=0;
	for(int i=7;i<n;i++){
		if(command[i] == '-' && i != n-1){
			i++;
			while(i < n && command[i] != ' '){
				if(command[i]=='a')
					a=1;
				else if(command[i] == 'l')
					l=1;
				else{
					printf("Wrong Flag\n");
					return -2;
				}
				i++;
			}
		}
		else if(command[i] == ' '){
			while(i<n && command[i] == ' ')
				i++;
			i--;
		}
		else{
			if(i == n-1 && command[i] == '-'){
				// prev dir
				//
				if(strlen(prev)==0){
					
					return -1;
				}
				strcpy(dir,prev);
			}
			else if(i == n-1 && command[i] == '~'){
				// home dir
				strcpy(dir,home_path);
			}
			else{
				char *buff = (char*)malloc(sizeof(char*)*n);
				int buff_counter = 0;
				while(i < n)
					buff[buff_counter++] = command[i++];
				buff[buff_counter]='\0';
				strcpy(dir,buff);
				// dir given by the user
			}


		}
	}
	//took help to write this part of this code but this is not ai generated code
	if(strlen(dir)==0) // no prev the do cuurent dir
		strcpy(dir,".");
	DIR *dir_ = opendir(dir);
	if(!dir_)
		return -1;
	char **files = NULL;
	int count = 0;
	struct dirent *val;

	while ((val = readdir(dir_)) != NULL) {
		if (!a && val->d_name[0] == '.')
			continue;

		files = realloc(files, (count + 1) * sizeof(char *));
		files[count] = strdup(val->d_name); 
		count++;
	}
	closedir(dir_);

	qsort(files, count, sizeof(char *), cmp);
	for (int i = 0; i < count; i++) {
		if (l)
			printf("%s\n", files[i]);
		else
			printf("%s  ", files[i]);
		free(files[i]); 
	}
	if (!l)
		printf("\n");
	free(files);	return 1;
}

