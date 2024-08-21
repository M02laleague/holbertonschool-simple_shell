#include "shell.h"

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
