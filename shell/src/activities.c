#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <unistd.h>
#include"../include/include.h"
Job jobs[100];
void activ(){
	for(int i=0;i<job_count;i++)
		printf("%d %s\n",jobs[i].pid,jobs[i].data);
	return;
}
