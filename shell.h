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

/* Main shell loop */
void shell_loop(void);

/* Input functions */
char *read_line(void);
char **split_line(char *line);

/* Command execution */
int execute_command(char **args);
int launch_program(char **args);

/* Builtin function declarations */
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_env(char **args);

/* Helper functions */
char *_getenv(const char *name);
char *find_command(char *command);
void free_args(char **args);

/* Error handling */
void print_error(char *msg);

/* Environment variables */
extern char **environ;

#endif /* SHELL_H */
