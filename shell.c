#include "shell.h"

/**
 * main - Entry point of the shell
 *
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    char *args[2];

    while (1)
    {
        printf("#cisfun$ ");
        fflush(stdout);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            else
            {
                perror("./shell");
                continue;
            }
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("./shell");
            continue;
        }
        if (pid == 0)
        {
            args[0] = line;
            args[1] = NULL;
            if (execve(line, args, environ) == -1)
            {
                fprintf(stderr, "./shell: No such file or directory\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
    return (0);
}
