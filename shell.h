#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define SIZE_ARG 64

extern char **environ;

int read_and_divid(char *buffer, char **argv);
int handle_elements(char *buffer, char **env);
char *find_command(char *cmd);
int execute_command(char *cmd, char **argv);

#endif /* SHELL_H */
