#include "shell.h"

/**
 * find_command - Checks if a command is executable or finds it in the 'PATH'
 * @cmd: Command to be checked or found
 *
 * Return: The full path of the command if found, or NULL if not found
 */
char *find_command(char *cmd)
{
	struct stat st;
	char *path = getenv("PATH");/*recurepere la variable 'PATH'*/
	char *path_cpy = strdup(path);/*creé une copie de 'PATH' pour strtok*/
	char *dir = strtok(path_cpy, ":");/*divise 'PATH' en repertoire*/
	static char full_path[BUFFER_SIZE];/*buffer pour stoket le chemin complet*/

	/*verifie si la command est un chemin absolue ou relatif*/
	if (stat(cmd, &st) == 0 && st.st_mode & S_IXUSR)
	{
		free(path_cpy);
		return (cmd);/*return la commande si elle est executable*/
	}

	/*Parcour chaque repertoir dans 'PATH'*/
	while (dir != NULL)
	{
		strcpy(full_path, dir);	/* Copy directory path to full_path */
		strcat(full_path, "/");	/* Append slash */
		strcat(full_path, cmd);	/* Append command */
		/*Verifie si le chemin complet est executable*/
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
