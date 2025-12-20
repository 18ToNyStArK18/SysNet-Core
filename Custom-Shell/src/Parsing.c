#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <linux/limits.h>
#include "../include/include.h"
// Idea is to split recurssively and check for everything
int log_found = 0;
int name(char *input)
{
	regex_t regex;
	int result;
	const char *pattern = "^[^|&><;]+$";

	if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
	{
		fprintf(stderr, "Failed to compile regex\n");
		return 0;
	}

	result = regexec(&regex, input, 0, NULL, 0);

	regfree(&regex);

	return (result == 0) ? 1 : 0;
}
int input(char *inp)
{
	int i = 0;
	int n = strlen(inp);
	while (i < n && inp[i] == ' ')
		i++;
	if (i == n)
		return 0;
	if (inp[i] == '<')
	{
		i++;
		while (inp[i] == ' ')
			i++;
		return name(inp + i);
	}
	return 0;
}
int output(char *oup)
{
	int i = 0;
	int n = strlen(oup);
	while (i < n && oup[i] == ' ')
		i++;
	if (i == n)//if everything is space
		return 0;
	if (i + 1 < n && oup[i] == '>' && oup[i + 1] != '>')
	{
		i++;
		if (i < n && oup[i] == ' ')
			i++;
		return name(oup + i); // 
	}
	if (i + 1 < n && oup[i] == '>' && oup[i + 1] == '>')
	{
		i = i + 2;
		if (i < n && oup[i] == ' ')
			i++;
		return name(oup + i);
	}
	return 0;
}
int atomic(char *at)
{
	if (strlen(at) == 0)
		return 0;
	int flag = 0;
	int n = strlen(at);
	int i = 0;
	while(i < n && at[i]==' ')
		i++;
	char *buff2 = (char *)malloc(sizeof(char) * (n + 1));
	int buff2_counter = 0;
	if(strncmp(at,"log ",4)==0 || strcmp(at,"log")==0)
		log_found =1;

	while (i < n && at[i] != ' ')
	{
		buff2[buff2_counter++] = at[i++];
		flag = 1;
	}
	buff2[buff2_counter] = '\0';
	if (!name(buff2)) // first one should be name
		return 0;
	while (i < n && at[i] == ' ') // skipping the spaces
		i++;
	while (i < n)
	{
		char *buff = (char *)malloc(sizeof(char) * (n + 1));
		int buff_counter = 0;
		while (i < n && (at[i] != ' ' || at[i - 1] == '<' || at[i - 1] == '>'))
		{
			buff[buff_counter++] = at[i++];
			flag = 1;
		}
		buff[buff_counter] = '\0';
		if (!name(buff) && !input(buff) && !output(buff)) // the buff can either be input or output or name
			return 0;
		while (i < n && at[i] == ' ')
			i++;
	}
	if (flag == 0)
		return 0;
	return 1;
}
int cmd_group(char *cmd_g)
{
	int flag = 0;
	int n = strlen(cmd_g);
	// removing the trailing spaces
	n = n - 1;
	while (cmd_g[n] == ' ')
		n--;
	n++;
	int i = 0;
	while(i < n && cmd_g[i]==' ')
		i++;
	while (i < n)
	{
		char *buff = (char *)malloc(sizeof(char) * (n + 1));
		int buff_counter = 0;
		while (i < n && cmd_g[i] != '|')
		{ // atomics are seperated by '|'
			buff[buff_counter++] = cmd_g[i++];
			flag = 1; // making sure atleast one is present
		}
		buff[buff_counter] = '\0';
		if (atomic(buff) == 0)
			return 0; // if it's not a buff return 0
		free(buff);
		int temp = i;
		i++;
		if (i < n && cmd_g[i] == ' ')
			i++;
		if (cmd_g[temp] == '|' && i == n) // if every char is space after pipe its wrong
			return 0;
	}
	if (flag == 0)
		return 0;
	return 1;
}
int validate(char *cmd)
{
	int n = strlen(cmd);
	log_found = 0;
	n--;
	if (cmd[n] == ' ') // removing trailing spaces
		n--;
	n++;
	if (cmd[n - 1] == ';') // it should not end with ;
		return 0;
	int i = 0;
	while(i < n && cmd[i]==' ')
		i++;
	int flag = 0;
	while (i < n)
	{
		char *buff = (char *)malloc(sizeof(char) * (n + 1));
		int buff_counter = 0;
		while (i < n && (cmd[i] != '&' && cmd[i] != ';'))
		{
			buff[buff_counter++] = cmd[i++];
			flag = 1; // making sure that atleast one cmd_grp exists
		}
		i++; // skipping the & and ;
		buff[buff_counter] = '\0';
		if (cmd_group(buff) == 0) // recurssively checks the cmd_grp
			return 0;
		free(buff);
		if (i < n && cmd[i] == ' ')
			i++;
	}
	if (flag == 0)
		return 0;
	if(log_found)
		return 118;
	return 1;
}
