#include "shell.h"
/**
 * read_and_divid - Reads user input and splits it into tokens
 * @buffer: Buffer to store the command
 * @argv: Array to store command arguments
 * Return: 0 on success, -1 on failure
 */
int read_and_divid(char *buffer, char **argv)
{
	ssize_t b_read;
	int i = 0;
	char *token;

	b_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	if (b_read == -1)
	{
		perror("read");
		return (-1);
	}
	if (b_read == 0)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		return (-1);
	}
	buffer[b_read - 1] = '\0';

	token = strtok(buffer, " ");
	while (token != NULL && i < SIZE_ARG - 1)
	{
		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL;
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
	if (strcmp(buffer, "exit") == 0)
		return (2);

	if (strcmp(buffer, "env") == 0)
	{
		while (*env)
		{
			write(STDOUT_FILENO, *env, strlen(*env));
			write(STDOUT_FILENO, "\n", 1);
			env++;
		}
		return (1);
	}
	return (0);
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

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{ path = environ[i] + 5;
			break;
		}
	}
	if (path)
	{
		path_copy = strdup(path);
		command_length = strlen(command);
		path_token = strtok(path_copy, ":");
		while (path_token != NULL)
		{
			directory_length = strlen(path_token);
			file_path = malloc(command_length + directory_length + 2);
			strcpy(file_path, path_token);
			strcat(file_path, "/");
			strcat(file_path, command);
			if (stat(file_path, &buffer) == 0)
			{
				free(path_copy);
				return (file_path);
			}
			else
			{
				free(file_path);
				path_token = strtok(NULL, ":");
			}
		}
		free(path_copy);
		if (stat(command, &buffer) == 0)
			return (command);
		return (NULL);
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
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		if (execve(cmd, argv, environ) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
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

	while (!should_exit)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		if (read_and_divid(buffer, argv) == -1)
			break;

		if (!argv[0])
			continue;

		result = handle_elements(argv[0], environ);
		if (result == 2)
		{
			should_exit = 1;
			continue;
		}
		else if (result == 1)
			continue;

		cmd = find_command(argv[0]);
		if (cmd)
		{
			temp = argv[0];
			argv[0] = cmd;
			execute_command(cmd, argv);
			if (cmd != temp)
				free(cmd);
			argv[0] = temp;
		}
		else
			write(STDERR_FILENO, "./shell: No such file or directory\n", 35);
	}
	return (0);
}
