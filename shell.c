#include "shell.h"

int main(void)
{
	char buffer[BUFFER_SIZE]; 	/* stock la commande lue */
	ssize_t b_read; 	/* stock le nombre d'octets lus par read()*/
	pid_t pid; 		/* Identifiant de processur (PID)*/
	int stat, i; 		/*  Variables pour la gestion des boucles et du status du processur*/
	char *prompt = "$ ";	 /* Prompt affiche pour l'utilisateur*/
	char *token;	 /* Pointeur pour diviser la commande en tokens*/
	char *argv[SIZE_ARG];	 /* Tableau pour stoker les argument de la commande*/

	while (1) /* boucle infinie pour maintenir le shell actif*/
	{
		/*  (mode interactif) si l'entre standard est un terminal affiche le prompt*/
		if (isatty(STDIN_FILENO))
		{
			printf("%s", prompt); /* affiche le prompt */
			fflush(stdout); 	/* vide le bffer de sortie pour s'assurer que le prompt est afficher immediatement */
		}
		/*  Lire la commande entree par l'utilisateur */
		b_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		/* gerer les erreur de lecture */
		if (b_read == -1)
		{
			perror("read"); 	/* Affiche un message d'erreur si la lecture echoue */
			continue; 	/* Recommence la boucle */
		}
		if (b_read == 0) /*  Si l'utilisateur entre Ctrl+D (EOF) , quitte la boucle */
		{
			break; /* sort de la boucle et termine le programme*/
		}
		/*  Remplace le retour a la ligne a la fin de la commande par un caractere nul*/
		buffer[b_read - 1] = '\0';
		/*  Si la commande est vide, recommence la boucle*/
		if (buffer[0] == '\0')
		{
			continue;
		}
		/*  verifie si l'utilisateur a tapé "exit" pour quitter le shell*/
		if (strcmp(buffer, "exit") == 0)
		{
			break;	/*sort de la boucle est termine le prog*/
		}
		/*  Divise la commande en argument (tokens) en utilisant strok */
		i = 0;
		token = strtok(buffer, " ");	/*  Separe la commande par les espaces */
		while (token != NULL && i < SIZE_ARG - 1) 	/*  continue tant qu'il reste des tokens */
		{
			argv[i] = token;	/*  stock chaque argument dans argv */
			token = strtok(NULL, " ");	/* passe au token suivant */
			i++;
		}
		argv[i] = NULL;	/*  Terminer la liste des arguments par NULL pour execve*/

		/*  Cree un processur enfant pour executer la commande*/
		pid = fork();
		if (pid == -1)	/*  si la creation du processur echoue, affiche erreur */
		{
			perror("fork");
			continue;	/*  Recommence la boucle */ 
		}
		if (pid == 0)	/*  processus enfan */
		{
			/*  Execute la commande avec les arguments*/
			if (execve(argv[0], argv, NULL) == -1)	/*  Remplace le processur enfant par le programe specifié*/
			{
				perror("./shell");	/*  Affiche un message d'erreur si la commande echoue */
			}
			exit(EXIT_FAILURE);		/*  Si execve echoue, termine le processus enfant */
		}
		else	/*  Processus parent */
		{
			wait(&stat);	/*  Atend que le processus enfant se termine */
		}
	}
	return (0);	/*  le programe termine avec succes */
}