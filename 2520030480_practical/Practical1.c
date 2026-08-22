/*
Develop a C program that demonstrates how a Linux operating system executes a command entered by a user that 1. Accept a Linux command as input. 2. Create a child process using fork(). 3. Execute the command in the child process using an appropriate exec() system call. 4. Allow the parent process to wait for the child using wait (). 5. Display the Process ID (PID) of both parent and child processes.

Using Linux terminal commands (uname, lscpu, lsblk, ps, top), investigate the relationship between hardware resources and operating system services. Prepare a report explaining how the OS abstracts CPU, memory, storage, and I/O devices.    
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    
    printf("Enter a Linux command (e.g., ls -l, date, whoami): ");
    if (!fgets(input, sizeof(input), stdin)) {
        perror("Failed to read input");
        exit(EXIT_FAILURE);
    }

    // Strip trailing newline
    input[strcspn(input, "\n")] = 0;

    // Tokenize the command string into arguments
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // execvp requires a NULL-terminated array

    if (args[0] == NULL) {
        printf("No command entered.\n");
        return 0;
    }

    printf("\n[Main] Parent Process PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child Process
        printf("[Child] Child PID: %d | Parent PID: %d\n", getpid(), getppid());
        printf("[Child] Executing command: %s\n\n", args[0]);
        
        execvp(args[0], args);
        
        // If execvp returns, an error occurred
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent Process
        int status;
        printf("[Parent] Waiting for Child (PID: %d) to complete...\n", pid);
        wait(&status);
        
        if (WIFEXITED(status)) {
            printf("\n[Parent] Child process terminated normally with exit status %d.\n", WEXITSTATUS(status));
        } else {
            printf("\n[Parent] Child process terminated abnormally.\n");
        }
    }

    return 0;
}