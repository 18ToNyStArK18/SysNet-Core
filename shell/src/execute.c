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
Job jobs[100000];
void job_finished(int jid, char *home_path){
	Job temp_jobs[100000];
	int file_count =0;
	char  file_path[1024];
	sprintf(file_path,"%s/jobs_list.txt",home_path);
	FILE *fp = fopen(file_path,"r");
	if(!fp)
		return;
	while(fscanf(fp,"%d %d %d %[^\n]",&temp_jobs[file_count].jid,&temp_jobs[file_count].pid,&temp_jobs[file_count].state,temp_jobs[file_count].data) != -1 ){
		file_count++;
	}
	fclose(fp);
	for(int i=0;i<file_count;i++){
		if(temp_jobs[i].jid == jid){
			temp_jobs[i].state = 0;
		}
	}
	fp = fopen(file_path,"w");
	for(int i=0;i<file_count;i++){
		fprintf(fp,"%d %d %d %s\n",temp_jobs[i].jid,temp_jobs[i].pid,temp_jobs[i].state,temp_jobs[i].data);	
	}
	fclose(fp);
}

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
	while(i < n && cmd[i]==' ')
		i++;
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
			printf("No such file or directory\n");
			return(-1);
		}
		dup2(inp,STDIN_FILENO);
		close(inp);
	}
	if(strlen(output)){
		if(output_type==1)
			oup = open(output,O_WRONLY|O_CREAT|O_TRUNC,0777);
		else
			oup = open(output,O_APPEND|O_WRONLY|O_CREAT,0777);
		if(oup == -1){
			printf("Unable to create file for writing\n");
			return -1;
		}
		dup2(oup,STDOUT_FILENO);
		close(oup);
	}
	if(strncmp(command,"hop",3) == 0 ){
		if( hop(command,prev,home_path)==-1){
			printf("No such directory!\n");
			if(strlen(input)){
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {
					// discard
				}
				dup2(std_in,STDIN_FILENO);
			}
			if(strlen(output))
				dup2(std_out,STDOUT_FILENO);
			return(-1);
		}if(strlen(input)){
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {
				// discard
			}
			dup2(std_in,STDIN_FILENO);
		}
		if(strlen(output))
			dup2(std_out,STDOUT_FILENO);

		return(1);
	}
	else if(strncmp(command,"fg",2)==0){
		fg_command(command, home_path);	
	}
	else if(strncmp(command,"bg",2)==0){
		bg_command(command, home_path);	
	}
	else if(strncmp(command,"reveal",6) == 0){
		int temp = my_reveal(command,prev,home_path);
		if(temp == -1){
			printf("No such directory!\n");
			if(strlen(input)){
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {
					// discard
				}
				dup2(std_in,STDIN_FILENO);
			}
			if(strlen(output))
				dup2(std_out,STDOUT_FILENO);
			return -1;
		}
	}
	else if(strncmp(command,"log",3)==0){
		command=my_log(command,home_path);
		if(command)
			my_exec(command,prev,home_path,path_req,0); 
	}
	else if(strncmp(command,"ping ",5)==0){
		int r = ping(command,home_path);
		if(r){
			printf("Sent signal signal_number to process with pid %d\n",r);
		}
		else
			printf("No such process found!\n");
	}
	else if(strcmp(command,"activities") == 0){
		activ(home_path);
	}
	else{
		int devnull = open("/dev/null", O_WRONLY);
		dup2(devnull, STDERR_FILENO);
		close(devnull);
		execlp("bash", "bash", "-c", new_cmd, NULL);
		if(strlen(input)){
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {
				// discard
			}
			dup2(std_in,STDIN_FILENO);
		}
		if(strlen(output))
			dup2(std_out,STDOUT_FILENO);

		exit(127);
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
	return 1;;

}
int cmd_exec(char *cmd_g,char *prev,char*home_path,char *path_req,int is_background){
	int n = strlen(cmd_g);
	int i=0;
	while(i < n && cmd_g[i]==' ')
		i++;
	pid_t pgid = -1; // common pgid for all foreground processes so that i kill all at the same time
	int num_pipes=0;
	for(int ii=0;ii<n;ii++){
		if(cmd_g[ii]=='|')
			num_pipes++;
	}
	if(!num_pipes){
		if(strncmp(cmd_g,"hop ",4) == 0 || strcmp(cmd_g,"hop")==0 || strcmp(cmd_g,"fg")==0 || strncmp(cmd_g,"fg ",3) == 0  || strcmp(cmd_g,"bg")==0 || strncmp(cmd_g,"bg ",3) == 0)
			return atomic_exec(cmd_g,prev,home_path,path_req);
		int f = fork();
		if(f==0){
			signal(SIGINT, SIG_DFL);
			int return_value = atomic_exec(cmd_g,prev,home_path,path_req);
			if (return_value==-1)
				exit(1);
			if(return_value == 2){
				printf("CCommand not found!\n");
				exit(1);
			}

			exit(0);
		}
		int status;
		waitpid(f,&status,0);
		int exit = WEXITSTATUS(status);
		if(WIFEXITED(status) && exit != 0){
			if(exit == 127)
				printf("Command not found!\n");
			return -1;
		}
		return 1;
	}
	int pipes[num_pipes][2];
	int cmd_index=0;
	int last_pid;
	int num_children=0;
	for(int i=0;i<num_pipes;i++)
		pipe(pipes[i]);
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter=0;
		while(i<n && cmd_g[i] != '|'){
			buff[buff_counter++] = cmd_g[i++];
		}
		buff[buff_counter]='\0';
		i++;
		pid_t pid = fork();
		if(pid == 0){
			signal(SIGINT, SIG_DFL);
			if(pgid == -1)
				setpgid(0,0);
			else
				setpgid(0,pgid);
			if(cmd_index > 0)
				dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
			if(cmd_index < num_pipes)
				dup2(pipes[cmd_index][1],STDOUT_FILENO);
			for (int j = 0; j < num_pipes; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			int return_value = atomic_exec(buff,prev,home_path,path_req);
			if (return_value==-1)
				exit(1);

			exit(0);

		}
		if(pgid == -1)
			pgid = pid;
		last_pid = pid;
		num_children++;
		setpgid(pid,pgid);
		add_proc(buff,pid,home_path);
		free(buff);
		if(i<n && cmd_g[i] == ' ')
			i++;
		cmd_index++;
	}
	for (int j = 0; j < num_pipes; j++) {
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	if(!is_background)
		tcsetpgrp(STDIN_FILENO, pgid);
	int status;
	int last_cmd_status = 0;
	pid_t last_cmd_pid = last_pid;  // PID of the last command
	int children_reaped = 0;

	// Wait for all children, but track the last command's status specially
	while (children_reaped < num_children) {
		pid_t waited_pid = waitpid(-pgid, &status, 0);
		if (waited_pid > 0) {
			children_reaped++;
			if (waited_pid == last_cmd_pid) {
				// This is the last command in the pipeline
				last_cmd_status = status;
			}
		}
	}
	if(!is_background)
		tcsetpgrp(STDIN_FILENO, getpgrp());
	if (WIFEXITED(last_cmd_status)) {
		int exit_code = WEXITSTATUS(last_cmd_status);
		if(WEXITSTATUS(last_cmd_status) == 127)
			printf("Command not found!\n");

		return (exit_code == 0) ? 1 : -1;
	} else {
		// Command was terminated by signal
		return -1;
	}
}
int my_exec(char *cmd,char *prev,char *home_path,char *path_req,int log){
	if(!log)
		add_cmd(cmd,home_path);

	int n = strlen(cmd);
	n= n-1;
	if(cmd[n] == ' ')
		n--;
	n++;
	if(cmd[n-1] == ';')
		return 0;
	int i=0;
	while(i < n && cmd[i]==' ')
		i++;
	while(i<n){
		char *buff = (char*)malloc(sizeof(char)*(n+1));
		int buff_counter=0;
		while(i<n && (cmd[i] != '&'&&cmd[i] !=';')){
			buff[buff_counter++]=cmd[i++];
		}
		i++;
		buff[buff_counter]='\0';
		if(cmd[i-1] == '&'){
			pid_t fd  = fork();
			if(fd == 0){
				printf("[%d] %d\n",job_count,getpid());
				setpgid(0,0);
				int successfull = 1;
				if(cmd_exec(buff,prev,home_path,path_req,1)==-1)
					successfull = 0;
				char *temp_path = (char *)malloc(1025);
				temp_path[0]='\0';
				strcat(temp_path,home_path);
				strcat(temp_path,"/jobs.txt");
				FILE *fp = fopen(temp_path,"a");
				if(successfull)
					fprintf(fp,"%swith pid %d exited normally\n",buff,getpid());
				else{
					fprintf(fp,"%swith pid %d exited abnormally\n",buff,getpid());
				}
				fclose(fp);
				job_finished(job_count,home_path);
				exit(0);
			}
			else{
				jobs[job_count].pid = fd;
				jobs[job_count].state = 1;
				jobs[job_count].jid = job_count;
				strcpy(jobs[job_count].data,buff);
				char jobs_file_path [1024];
				sprintf(jobs_file_path,"%s/jobs_list.txt",home_path);
				FILE *fp = fopen(jobs_file_path,"a");
				fprintf(fp,"%d %d %d %s\n",jobs[job_count].jid,jobs[job_count].pid,jobs[job_count].state,jobs[job_count].data);
				job_count++;
				fclose(fp);
				add_proc(buff,fd,home_path);
			}
		}
		else
			cmd_exec(buff,prev,home_path,path_req,0);
		free(buff);
		if(i<n && cmd[i]==' ')
			i++;

	}
	return 1;
}


