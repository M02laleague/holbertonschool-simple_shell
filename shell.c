#include "shell.h"
/**
 * read_and_divid - Reads user input and splits it into tokens
 * @buffer: Buffer to store the command
 * @argv: Array to store command arguments
 * Return: 0 on success, -1 on failure
 */
int read_and_divid(char *buffer, char **argv)
{
	ssize_t b_read;  /* Variable to store the number of bytes read */
	int i = 0;  /* Index for argv array */
	char *token;  /* Pointer to store tokens */

	b_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	/* Read input from stdin */
	if (b_read == -1)  /* Check for read error */
	{
		perror("read");  /* Print error message if read fails */
		return (-1);
	}
	if (b_read == 0)  /* Check for EOF (Ctrl+D) */
	{
		if (isatty(STDIN_FILENO))  /* Check if stdin is a terminal */
			write(STDOUT_FILENO, "\n", 1);
			/* Print newline in interactive mode */
		return (-1);
	}
	buffer[b_read - 1] = '\0';
	/* Replace newline with null terminator */

	token = strtok(buffer, " ");
	/* Tokenize the input using space as delimiter */
	while (token != NULL && i < SIZE_ARG - 1)
	{
		argv[i++] = token;  /* Store each token in argv */
		token = strtok(NULL, " ");  /* Continue tokenizing */
	}
	argv[i] = NULL;  /* Null-terminate the argv array */
	return (0);
}

/**
 * handle_elements - Handles built-in commands
 * @buffer: Input command
 * @env: Environment variables
 * Return: 1 if a built-in command was executed, 0 otherwise
 */
int handle_elements(char *buffer, char **env)
{
	if (strcmp(buffer, "exit") == 0)  /* Check if the command is "exit" */
		return (2);  /* Return 2 to signal shell exit */

	if (strcmp(buffer, "env") == 0)  /* Check if the command is "env" */
	{
		while (*env)  /* Loop through environment variables */
		{
			write(STDOUT_FILENO, *env, strlen(*env));
			/* Print each environment variable */
			write(STDOUT_FILENO, "\n", 1);
			/* Print newline after each variable */
			env++;
		}
		return (1);  /* Return 1 to indicate built-in command executed */
	}
	return (0);  /* Return 0 if no built-in command was executed */
}

/**
 * find_command - Finds the full path of a command
 * @command: Command to find
 * Return: Full path of the command if found, NULL otherwise
 */
char *find_command(char *command)
{
	char *path = NULL, *path_copy, *path_token, *file_path;
	int command_length, directory_length, i;
	struct stat buffer;

	for (i = 0; environ[i]; i++)/*Search for PATH environnement variable*/
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;/* extract PATH value*/
			break;
		}
	}
	if (path)
	{
		path_copy = strdup(path);/*duplicate PATH tokenization*/
		command_length = strlen(command);/*get length of command*/
		path_token = strtok(path_copy, ":");/*tokenize PATH */
		while (path_token != NULL)
		{directory_length = strlen(path_token);/*get length of current directory*/
			file_path = malloc(command_length + directory_length + 2);
			strcpy(file_path, path_token);/*copy directory path*/
			strcat(file_path, "/");/*Add slash*/
			strcat(file_path, command);/* Append command name*/
			if (stat(file_path, &buffer) == 0)/*check if the command exists*/
			{
				free(path_copy);/*Free memory if command not found*/
				return (file_path);/*Return full path of the command*/
			}
			else
			{
				free(file_path);/*Free memory if command not found*/
				path_token = strtok(NULL, ":");/*Move to the next directory in PATH*/
			}
		}
		free(path_copy);/*Free duplicated PATH after use*/
		if (stat(command, &buffer) == 0)/*if command is a valid path*/
			return (command);/*return the command itself*/
		return (NULL);/*Return NULL if PATH is not found*/
	}
	return (NULL);
}
/**
 * execute_command - Executes a command
 * @cmd: Command to execute
 * @argv: Command arguments
 */
void execute_command(char *cmd, char **argv)
{
	pid_t pid;/* Variable to store process ID*/
	int status;/* Variable to store child's exit status*/

	pid = fork();/* create a new process*/
	if (pid == -1)/* Check for fork error*/
	{
		perror("fork");/*Print error message if fork fails*/
		return;
	}
	if (pid == 0)/* child process*/
	{
		if (execve(cmd, argv, environ) == -1)/*Execute the command*/
		{
			perror("./shell");/* Print error if execve fails*/
			exit(EXIT_FAILURE);/* Exit child process with faillure status*/
		}
	}
	else
	{
		wait(&status);/* Wait for child process to finish*/
	}
}

/**
 * main - Entry point of the shell
 * Return: Always 0
 */
int main(void)
{
	char buffer[BUFFER_SIZE], *argv[SIZE_ARG], *cmd, *temp;
	int should_exit = 0, result;

	while (!should_exit)/* Main shell loop*/
	{
		if (isatty(STDIN_FILENO))/* Check if shell is in interactive mode*/
			write(STDOUT_FILENO, "#cisfun$ ", 9);/* Display prompt*/

		if (read_and_divid(buffer, argv) == -1)/* Read and split user input*/
			break;

		if (!argv[0])/* If no command was entered , continue*/
			continue;

		result = handle_elements(argv[0], environ);
		if (result == 2)/* If command is 'exit'*/
		{
			should_exit = 1;/* exit the loop and quit the shell*/
			continue;
		}
		else if (result == 1)/* If a built-in command was executed */
			continue;/* skip to the next iteration of the loop*/

		cmd = find_command(argv[0]);/* Find the path of the command */
		if (cmd)/* If the command was found */
		{
			temp = argv[0];
			argv[0] = cmd;/* Replace the command with its full path */
			execute_command(cmd, argv);/* execute the command*/
			if (cmd != temp)
				free(cmd);/* Free the memory if necessary */
			argv[0] = temp;/* Restore the original command name */
		}
		else
			write(STDERR_FILENO, "./shell: No such file or directory\n", 35);
	}
	return (0);/* return success*/
}
