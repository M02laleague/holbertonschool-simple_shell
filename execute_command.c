#include "shell.h"

/**
 * execute_command - Executes a command by forking a child process
 * @cmd: Command to be executed
 * @argv: Array of command arguments
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *cmd, char **argv)
{
		int stat;/*variable pour stocker le status du processus enfant*/
		pid_t pid = fork();/*Cree un processur enfant pour executer la commande*/

		if (pid == -1)/*  si la creation du processur echoue*/
		{
			perror("fork");/*Affiche un message d'erreur si fork echoue*/
			return (-1);
		}
		if (pid == 0)/*  processus enfan */
		{
			/*Remplace le processur enfant par la commande specifié*/
			if (execve(cmd, argv, environ) == -1)
			{
				perror("./shell");/*Affiche un message d'erreur si execve echoue*/
				exit(EXIT_FAILURE);	/* Si execve echoue, termine le processus enfant */
			}
		}
		else	/*Processus parent */
		{
			wait(&stat);	/*  Atend que le processus enfant se termine */
		}
		return (0);
}
