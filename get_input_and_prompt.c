#include "shell.h"

/**
 * get_input_and_prompt - Reads user input and displays the prompt
 * @buffer: Pointer to buffer to store the command
 * @len: Pointer to the length of the buffer
 * @argv: Array to store command arguments
 *
 * Return: 0 on success, -1 on failure
 */

int get_input_and_prompt(char **buffer, size_t *len, char **argv)
{
	ssize_t b_read;/* stock le nombre d'octets lus par read()*/
	char *prompt = "$ ";/* Prompt affiche pour l'utilisateur*/
	int i = 0;	/*index pour parcourir le tableau argv*/
	char *token;	/*pointeur pour stoker chaque argument*/

	/*(mode interactif) si l'entre standard est un terminal affiche le prompt*/
	if (isatty(STDIN_FILENO))
	{
		printf("%s", prompt);/* affiche le prompt */
		/* vide le buffer pour que le prompt s'afficher immediatement */
		fflush(stdout);
	}
	/*Lire l'entré de l'utilisateur*/
	b_read = getline(buffer, len, stdin);
	if (b_read == -1)/*verifie si il y a une erreur de lecture */
	{
		if (feof(stdin))
			return (-1);
		else if (errno)
		{
			perror("getline");
			return (-1);
		}
	}

	(*buffer)[b_read - 1] = '\0';	/*Retirer le retour a la ligne*/

	/*  Divise la commande en argument (tokens) en utilisant strok */
		token = strtok(*buffer, " ");	/*  Separe la commande par les espaces */
	/*  continue tant qu'il reste des tokens */
		while (token != NULL && i < SIZE_ARG - 1)
		{
			argv[i++] = token;	/*  stock chaque argument dans argv */
			token = strtok(NULL, " ");	/* passe au token suivant */
		}
		argv[i] = NULL;	/*  Terminer la liste des arguments par NULL pour execve*/
		return (0);
}
