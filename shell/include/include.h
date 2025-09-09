#ifndef include_h
#define include_h
#include<unistd.h>
int hop(char *command,char *prev,char * home_path);
char * redirect(char *command);
int createfile(char *file);
char *my_log(char *command,char * home_path);
int add_cmd(char *cmd,char * home_path);
int validate(char *cmd);
int cmd_group(char *cmd_g);
int atomic(char *at);
int name(char *input);
int input(char *inp);
int output(char *oup);
int my_reveal(char *command , char *prev,char * home_path);
int my_exec(char* cmd,char *prev,char *home_path,char *path_req,int log_exec);
int atomic_exec(char *cmd_g,char *prev,char *home_path,char *path_req);
int cmd_exec(char *cmd,char *prev,char *home_path,char *path_req);
char * FindPath(char * home_path);
int ping(char *cmd,char * home_path);
void activ(char * home_path);
void add_proc(char*cmd,pid_t pid,char * home_path);
void kill_jobs(char * home_path);
void sigtstp_handler(int sig);
void bg_command(char *command, char *home_path);
typedef struct {
    pid_t pid;
    char data[256];
	int jid;
	int state; // 1 = running 2 = stopped 0 = terminated
} Job;
void fg_command(char *command,char *home_path);
extern Job jobs[1000];
extern int job_count;
#endif
