#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include"../include/include.h"

int str_int(char *str){
	int ans = 0;
	int i = 0;
	while( i < (int)strlen(str) && str[i] != ' '){
		ans = ans*10 + str[i] - '0';
		i++;
	}
	return ans;
}

int ping(char * cmd){

	int i=5;
	int pid = str_int(cmd+5);
	while(i <(int) strlen(cmd) && cmd[i] != ' ')
		i++;
	i++;
	int signal = str_int(cmd+i);
	signal =signal % 32;
	if(kill(pid,signal) == -1){
		return 0;
	}
	return pid;
}
