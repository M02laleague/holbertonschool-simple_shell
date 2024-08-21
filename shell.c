#include "shell.h"

/**
 * main - Entry point of the shell
 *
 * Return: Always 0
 */
int	main(void)
{
	char *buffer = NULL;/* stock la commande lue */
	char *cmd;	 /* Pointeur pour stocker le chemin de la command*/
	char *argv[SIZE_ARG];	 /* Tableau pour stoker les argument de la commande*/
	size_t len = 0;

	while (1) /* boucle infinie pour maintenir le shell actif*/
	{
		/*  Lire la commande entree et afichier le prompt*/
		if (get_input_and_prompt(&buffer, &len, argv) == -1)
		{
			if (!isatty(STDIN_FILENO) || feof(stdin))
				break;
			continue;
		}

		/*gere le commande integrées*/
		if (handle_elements(buffer, environ))
		{
			break;
		}
		cmd = find_command(argv[0]);/*le premier argument est une commande*/
		/*Si la commande n'est pas un chemin absolu, la chercher dans le 'PATH'*/
		if (cmd)
			execute_command(cmd, argv);
		else
		{
			printf("%s: command not found\n", argv[0]);
		}

		if (!isatty(STDIN_FILENO))/*sortir apres execution mode non interactif*/
			break;
	}
	free(buffer); /*libere la memoire alouer par getline*/
	return (0);	/*  le programe termine avec succes */
}
