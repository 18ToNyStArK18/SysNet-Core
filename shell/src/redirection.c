#include "../include/include.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int createfile(char * file){
    return 1;
}

char * redirect(char *command){
    int n = strlen(command);
    
    char *input = NULL;
    char *output = NULL;
    int output_type = 0; 
    
    char *base_command = (char *)malloc(n + 1);
    int base_len = 0;
    
    int i = 0;
    while(i < n && command[i] != '>' && command[i] != '<') {
        base_command[base_len++] = command[i++];
    }
    base_command[base_len] = '\0';
    while(i < n) {
        if(command[i] == '>') {
            i++; // skip first '>'
            int is_append = 0;
            
            if(i < n && command[i] == '>') {
                is_append = 1;
                i++; // skip second '>'
            }
            while(i < n && (command[i] == ' ' || command[i] == '\t')) {
                i++;
            }
            
            int start = i;
            while(i < n && command[i] != '>' && command[i] != '<' && 
                  command[i] != ' ' && command[i] != '\t') {
                i++;
            }
            
            if(i > start) {
                if(output) {
					createfile(output);
                    free(output);
                }
                int curr_len = i - start;
                output = (char *)malloc(curr_len + 1);
                strncpy(output, command + start, curr_len);
                output[curr_len] = '\0';
                output_type = is_append ? 2 : 1;
            }
        }
        else if(command[i] == '<') {
            i++; // skip '<'
            
            while(i < n && (command[i] == ' ' || command[i] == '\t')) {
                i++;
            }
            
            int start = i;
            while(i < n && command[i] != '>' && command[i] != '<' && 
                  command[i] != ' ' && command[i] != '\t') {
                i++;
            }
            
            if(i > start) {
                
                if(input) {
                    free(input);
                }
                
                
                int curr_len = i - start;
                input = (char *)malloc(curr_len + 1);
                strncpy(input, command + start, curr_len);
                input[curr_len] = '\0';
            }
        }
		else
			i++;
    }
    
    char *refined = (char *)malloc(n + 100);     
	strcpy(refined, base_command);
    if(input) {
        strcat(refined, " < ");
        strcat(refined, input);
    }
    
	if(output) {
        if(output_type == 2) {
            strcat(refined, " >> ");
        } else {
            strcat(refined, " > ");
        }
        strcat(refined,output);
    }
    
    free(base_command);
    if(input) free(input);
    if(output) free(output);
    return refined;

}

