#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"
#define PROMPT "#cisfun$ "
#define SIZE_ARG 100

/* Function prototypes */

int get_input_and_prompt(char **buffer, size_t *len, char **argv);
int handle_elements(char *buffer, char **env);
char *find_command(char *cmd);
int execute_command(char *cmd, char **argv);



/* Environment variables */
extern char **environ;

#endif /* SHELL_H */
