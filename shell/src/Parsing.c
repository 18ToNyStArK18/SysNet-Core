#include<stdio.h>
#include<regex.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<linux/limits.h>
#include"../include/include.h"
// llm code begins
int name(char* input) {
	regex_t regex;
	int result;

	const char* pattern = "^[^|&><;]+$";

	if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
		fprintf(stderr, "Failed to compile regex\n");
		return 0;
	}

	result = regexec(&regex, input, 0, NULL, 0);

	regfree(&regex);

	return (result == 0) ? 1 : 0;
}
//llm code ends
int input(char *inp){
	int i=0;
	int n = strlen(inp);
    if(i<n && inp[i] == ' ')
		i++;
	if(i == n )
		return 0;
	if(inp[i] == '<'){
		i++;
		while(inp[i] == ' ')
			i++;
		return name(inp+i);
	}
	return 0;
}
int output(char *oup){
	int i=0;
	int n = strlen(oup);
    if(i<n && oup[i] == ' ')
		i++;
	if(i == n )
		return 0;
	if(i+1 < n && oup[i] == '>'&&oup[i+1] != '>'){
		i++;
		if(i<n && oup[i] == ' ')
			i++;
		return name(oup+i);
	}
	if(i+1 < n && oup[i] == '>'&&oup[i+1] == '>'){
		i=i+2;
		if(i<n && oup[i] == ' ')
			i++;
		return name(oup+i);
	}
	return 0;

}
int atomic(char *at){
	if(strlen(at)==0)
		return 0;
	int flag = 0;
	int n = strlen(at);
	int i=0;
	char *buff2 = (char*)malloc(sizeof(char)*(n+1));
	int buff2_counter = 0;
	while(i<n && at[i] != ' '){
		buff2[buff2_counter++] = at[i++];
		flag = 1;
	}
	buff2[buff2_counter]='\0';
	if(!name(buff2))
		return 0;
	if(i<n && at[i] == ' ')
		i++;	
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter = 0;
		while(i<n && (at[i] != ' ' || at[i-1] == '<' || at[i-1] == '>')){
			buff[buff_counter++] = at[i++];
			flag = 1;
		}
		buff[buff_counter]='\0';
		if(!name(buff) && !input(buff) && !output(buff))
			return 0;
		if(i<n && at[i] == ' ')
			i++;
	}
	if(flag==0)
		return 0;
	return 1;
}
int cmd_group(char *cmd_g){
	int flag =0;
	int n = strlen(cmd_g);
	int i=0;
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter=0;
		while(i<n && cmd_g[i] != '|'){
			buff[buff_counter++] = cmd_g[i++];
			flag=1;
		}
		buff[buff_counter]='\0';
		if(atomic(buff)==0)
			return 0;
		free(buff);
		int temp = i;
		i++;
		if(i<n && cmd_g[i] == ' ')
			i++;
		if(cmd_g[temp]=='|' && i == n)
			return 0;
	}
	if(flag == 0)
		return 0;
	return 1;
}
int validate(char *cmd){
	int n = strlen(cmd);
	n= n-1;
	if(cmd[n] == ' ')
		n--;
	n++;
	if(cmd[n-1] == ';')
		return 0;
	int i=0;
	int flag=0;
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter=0;
		while(i<n && (cmd[i] != '&'&&cmd[i] !=';')){
			buff[buff_counter++]=cmd[i++];
			flag=1;
		}
		i++;
		buff[buff_counter]='\0';
		if(cmd_group(buff)==0)
			return 0;
		free(buff);
		if(i<n && cmd[i]==' ')
			i++;

	}
	if(flag==0)
		return 0;
	return 1;
}


