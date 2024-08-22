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
	if (b_read == -1) /* Vérifie si getline retourne une erreur ou un EOF */
	{
		if (isatty(STDIN_FILENO))
		{
			printf("\n"); /* Pour ne pas laisser le prompt sur la même ligne */
		}
		return (-1); /* Retourne -1 pour quitter la boucle principale */
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

/**
 * handle_elements - Handles built-in commands like "exit" and "env"
 * @buffer: Input command
 * @env: Environment variables
 *
 * Return: 1 if a built-in command was executed, 0 otherwise
 */
int handle_elements(char *buffer, char **env)
{
	/*verifie si l'utilisateur a tapé "exit" pour quitter le shell*/
		if (strcmp(buffer, "exit") == 0)
		{
			return (1);	/*sort de la boucle est termine le prog*/
		}
	/*si l'itulisateur a tapé "env" pour afficher les variable d'environement*/
		if (strcmp(buffer, "env") == 0)
		{
			while (*env)/*parcours toute les variable d'envronement */
			{
				printf("%s\n", *env);
				env++;/*passe a la varible suivante*/
			}
			return (1);/*sort de la boucle est termine le prog*/
		}
		return (0);/*si aucune commande intern est detecté*/
}
/**
 * find_command - Checks if a command is executable or finds it in the 'PATH'
 * @cmd: Command to be checked or found
 *
 * Return: The full path of the command if found, or NULL if not found
 */
char *find_command(char *cmd)
{
	struct stat st;
	char *path, *path_cpy, *dir = NULL;
	char **env = environ;
	char *path_prefix = "PATH=";
	static char full_path[BUFFER_SIZE];/*buffer pour stoket le chemin complet*/
	size_t prefix_len = strlen(path_prefix);

	while (*env != NULL)
	{
		if (strncmp(*env, path_prefix, prefix_len) == 0)
		{
			path = *env + prefix_len;
			break;
		}
		env++;
	}
	if (!path)
		return (NULL);

	path_cpy = strdup(path);
	dir = strtok(path_cpy, ":");
	/*verifie si la command est un chemin absolue ou relatif*/
	if (stat(cmd, &st) == 0 && st.st_mode & S_IXUSR)
		return (cmd);/*return la commande si elle est executable*/

	while (dir != NULL)
	{
		strncpy(full_path, dir, BUFFER_SIZE - 1);
		full_path[BUFFER_SIZE - 1] = '\0';
		strncat(full_path, "/", BUFFER_SIZE - strlen(full_path) - 1);
		strncat(full_path, cmd, BUFFER_SIZE - strlen(full_path) - 1);
		if (stat(full_path, &st) == 0 && st.st_mode & S_IRUSR)
		{
			free(path_cpy);/*libere la memoire utiliser pour la copie de 'PATH'*/
			return (full_path);/*Return le chemin complet de la commande*/
		}
		dir = strtok(NULL, ":");/*passe au repertoir suivant*/
	}
	free(path_cpy);/*Libere la memoire utiliser pour la copie*/
	return (NULL);/*NULL si la commande n'a pas ete trouver*/
}

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
/**
 * main - Entry point of the shell
 *
 * Return: Always 0
 */
int main(void)
{
	char *buffer = NULL;	/* stock la commande lue */
	char *cmd; /* Pointeur pour stocker le chemin de la commande */
	char *argv[SIZE_ARG]; /* Tableau pour stoker les arguments de la commande */
	size_t len = 0;

	while (1) /* boucle infinie pour maintenir le shell actif */
	{
		/* Lire la commande entrée et afficher le prompt */
		if (get_input_and_prompt(&buffer, &len, argv) == -1)
		{
			break;
		}

		/* Gérer les commandes intégrées */
		if (handle_elements(buffer, environ))
		{
			break;
		}

		cmd = find_command(argv[0]); /* Le premier argument est une commande */
		/* Si la commande n'est pas un chemin absolu, la chercher dans le 'PATH' */
		if (cmd)
			execute_command(cmd, argv);
		else
		{
			printf("%s: 1: %s: command not found\n", argv[0], buffer);
		}

		if (!isatty(STDIN_FILENO))/*Sortir après exécution en mode non interactif*/
			break;
	}

	free(buffer);	/* Libérer la mémoire allouée par getline */
	return (0);	/* Le programme termine avec succès */
}
