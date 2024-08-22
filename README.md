# Simple Shell Project

![C Programming](https://img.shields.io/badge/C-Programming-blue.svg)
![Holberton School](https://img.shields.io/badge/Holberton-School-red.svg)

In this project, I implemented several different sorting algorithms.

* **Team:** Project to be done in teams of 2 people
  - Jeanpaul Dijeont
  - Salomon Diallo
* **Due time** 2 week 

## Description

This project implements a simple Unix command line interpreter (shell) that can execute commands, handle user input, and manage the environment. The shell mimics basic functionalities of the `/bin/sh` shell, allowing users to run commands and see outputs in both interactive and non-interactive modes.

## Learning Objectives

By the end of this project, you should be able to:

- Explain how a shell works and its components.
- Understand process management including PID and PPID.
- Manipulate the environment of the current process.
- Differentiate between functions and system calls.
- Create processes and execute programs using `execve`.
- Handle end-of-file (EOF) conditions and errors.

## Requirements

- Allowed editors: `vi`, `vim`, `emacs`.
- All files will be compiled on Ubuntu 20.04 LTS using `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -o hsh`.

- Code must adhere to the Betty style guidelines.
- Ensure no memory leaks occur in the implementation.
- Each header file must be include guarded.
- Use system calls only when necessary.
  
  List of the used functions:
- read_and_divid
- handle_elements
- find_command
- execute_command
- main
- read (fonction système)
- perror
- isatty
- write
- strtok
- strcmp
- strlen
- strncmp
- strdup
- strcpy
- strcat
- stat
 -malloc
- free
- fork
- execve
- exit
- wait
  
## Functions Overview

1. read_and_divid

  Purpose: Reads user input and splits it into tokens.
  Parameters:
  buffer: A buffer to store the command input.
  argv: An array to store the command arguments.
  Returns: 0 on success, -1 on failure.
  Description: This function reads input from the user, handles errors, and tokenizes the input string into separate command arguments.

3. handle_elements

  Purpose: Manages built-in commands.
  Parameters:
  buffer: The input command.
  env: The environment variables.
  Returns: 1 if a built-in command was executed, 0 otherwise.
  Description: This function checks for built-in commands such as "exit" and "env". It executes the appropriate action based on the command.

4. find_command

  Purpose: Finds the full path of a command.
  Parameters:
  command: The command to find.
  Returns: The full path of the command if found, NULL otherwise.
  Description: This function searches the system's PATH for the specified command and returns its full path if it exists.
6. execute_command

  Purpose: Executes a command.
  Parameters:
  cmd: The command to execute.
  argv: The command arguments.
  Description: This function forks a new process and uses execve to execute the command in the child process. It handles errors during the forking and execution

## Usage

To run the shell in interactive mode, execute:
```bash
./hsh
```
To run the manual page:

```bash
man ./man_1_simple_shell.1
```

## Built-in Commands

-exit: Exits the shell.
-env: Prints the current environment variables.

## File Structure

- README.md: Project description and instructions.
- AUTHORS: List of contributors.
- man_1_simple_shell: Manual page for the shell.
- shell.c: Core functionalities of the shell.
- shell.h: Header file containing function declarations.

## Flowcharts
![Blank diagram](https://github.com/user-attachments/assets/d709d3ab-eac8-4d92-98ad-1110d74841bf)

## Testing
![Capture d’écran 2024-08-22 191218](https://github.com/user-attachments/assets/43eb95ce-df45-4a6f-ac03-03e634cecb28)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


<p align="center">
  <img
   src="https://cdn.prod.website-files.com/64107f65f30b69371e3d6bfa/65c6179aa44b63fa4f31e7ad_Holberton-Logo-Cherry.svg"
   alt="Holberton School logo">
</p>
