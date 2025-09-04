#include<stdio.h>
#include<regex.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<linux/limits.h>
#include<sys/wait.h>
#include <fcntl.h>  
#include <unistd.h>
#include"../include/include.h"
int succesfull = 1;
int atomic_exec(char * cmd,char *prev,char *home_path,char *path_req){
	char * new_cmd = (char *)malloc(strlen(cmd)+1);
	new_cmd = redirect(cmd);
	char *input = (char *)malloc(strlen(cmd)+1);
	char *output = (char *)malloc(strlen(cmd)+1);
	char *command = (char *)malloc(strlen(cmd)+1);
	input[0]='\0';
	output[0]='\0';
	command[0]='\0';
	int output_type = 0;
	int i = 0,n = strlen(new_cmd),j=0;
	while(i < n && new_cmd[i] != '>' && new_cmd[i] != '<'){
		command[j++] = new_cmd[i++];
	}
	command[j]='\0';
	if(i < n && new_cmd[i] == '<'){
		i++;
		j=0;
		while(i < n && new_cmd[i] == ' ')
			i++;
		while( i < n && new_cmd[i] != '>')
			input[j++] = new_cmd[i++];
		j--;
		while(input[j]==' ')
			j--;
		j++;
	}
	input[j]='\0';
	if(i < n && new_cmd[i] == '>'){
		i++;
		output_type++;
		if(i < n && new_cmd[i] == '>'){
			i++;
			output_type++;
		}
		j=0;
		while(i < n && new_cmd[i] == ' ')
			i++;
		while(i < n)
			output[j++] = new_cmd[i++];
		j--;
		while(output[j] == ' ')
			j--;
		j++;
	}
	output[j]='\0';
	//printf("cmd:%s input:%s output:%s\n",command,input,output);
	int std_in = dup(STDIN_FILENO);
	int std_out = dup(STDOUT_FILENO);
	int inp = -2 , oup = -2;
	if(strlen(input)){
		inp = open(input,O_RDONLY);
		if(inp == -1){
			printf("No such file exists\n");
			return 0;
		}
		dup2(inp,STDIN_FILENO);
		close(inp);
	}
	if(strlen(output)){
		if(output_type==1)
			oup = open(output,O_WRONLY|O_CREAT|O_TRUNC,0777);
		else
			oup = open(output,O_APPEND|O_WRONLY|O_CREAT,0777);
		dup2(oup,STDOUT_FILENO);
		close(oup);
	}
	if(strncmp(command,"log",3) != 0)
		add_cmd(cmd,home_path);

	if(strncmp(command,"hop",3) == 0 ){
		if( hop(command,prev,home_path)==-1){
			printf("No such directory!\n");
			succesfull = 0;
		}
		return 1;
	}
	else if(strncmp(command,"reveal",6) == 0){
		int temp = my_reveal(command,prev,home_path);
		if(temp == -1){
			printf("No such directory!\n");
			succesfull =0;
		}
	}
	else if(strncmp(command,"log",3)==0){
		command=my_log(command,home_path);
		if(command)
			my_exec(command,prev,home_path,path_req); 
	}
	else{
		int f = fork();
		if(f == 0){
			int devnull = open("/dev/null", O_WRONLY);
			dup2(devnull, STDERR_FILENO);
			close(devnull);
			execlp("bash", "bash", "-c", new_cmd, NULL);
			_exit(127);
		}
		else {
			int status;
			waitpid(f, &status, 0);  // wait for this child
			if (WIFEXITED(status)) {
				int code = WEXITSTATUS(status);
				if (code == 127) {
					succesfull = 0;   // exec failed
				} else {
					succesfull = 1;   // exec ran fine
				}
			} else {
				succesfull = 0;       // abnormal termination (signal, etc.)
			}
		}

	} 
	if(strlen(input)){
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {
			// discard
		}
		dup2(std_in,STDIN_FILENO);
	}
	if(strlen(output))
		dup2(std_out,STDOUT_FILENO);
	return 1;

}
int cmd_exec(char *cmd_g,char *prev,char*home_path,char *path_req){
	int flag =0;
	int n = strlen(cmd_g);
	int i=0;
	int num_pipes=0;
	for(int ii=0;ii<n;ii++){
		if(cmd_g[ii]=='|')
			num_pipes++;
	}
	if(!num_pipes)
		return atomic_exec(cmd_g,prev,home_path,path_req);
	int pipes[num_pipes][2];
	int cmd_index=0;
	for(int i=0;i<num_pipes;i++)
		pipe(pipes[i]);
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter=0;
		while(i<n && cmd_g[i] != '|'){
			buff[buff_counter++] = cmd_g[i++];
			flag=1;
		}
		buff[buff_counter]='\0';
		i++;
		int pid = fork();
		if(pid == 0){
			if(cmd_index > 0)
				dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
			if(cmd_index < num_pipes)
				dup2(pipes[cmd_index][1],STDOUT_FILENO);
			for (int j = 0; j < num_pipes; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			atomic_exec(buff,prev,home_path,path_req);
			exit(0);
		}
		free(buff);
		if(i<n && cmd_g[i] == ' ')
			i++;
		cmd_index++;
	}
	for (int j = 0; j < num_pipes; j++) {
		close(pipes[j][0]);
		close(pipes[j][1]);
	}


	for (int j = 0; j <= num_pipes; j++) {
		wait(NULL);
	}
	return 1;
}
int my_exec(char *cmd,char *prev,char *home_path,char *path_req){
	int n = strlen(cmd);
	int job_count = 0;
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
		if(cmd[i-1] == '&'){
			int fd  = fork();
			if(fd == 0){
				printf("[%d] %d\n",job_count,getpid());
				cmd_exec(buff,prev,home_path,path_req);
				if(succesfull)
					printf("%s with pid %d exited normally\n",buff,getpid());
				else{
					printf("%s with pid %d exited abnormally\n",buff,getpid());
				}
				exit(0);
			}
			else
				job_count++;
		}
		else
			cmd_exec(buff,prev,home_path,path_req);
		free(buff);
		if(i<n && cmd[i]==' ')
			i++;

	}
	if(flag==0)
		return 0;
	return job_count;
}


