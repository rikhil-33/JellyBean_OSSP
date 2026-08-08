#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char command[100];

    // 1. Accept a Linux command as input
    printf("Enter a Linux command to execute (e.g., ls, pwd, date): ");
    if (fgets(command, sizeof(command), stdin) == NULL) {
        perror("Error reading input");
        exit(EXIT_FAILURE);
    }

    // Remove the trailing newline character from fgets
    command[strcspn(command, "\n")] = '\0';

    printf("\n[Parent] Process PID: %d\n", getpid());

    // 2. Create a child process using fork()
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // 3. Child process code execution
        printf("[Child]  Process PID: %d (Parent PID: %d)\n", getpid(), getppid());
        printf("[Child]  Executing command: '%s'\n", command);
        printf("----------------------------------------\n");

        // Execute command using execlp()
        execlp(command, command, (char *)NULL);

        // execlp only returns if an error occurred
        perror("Execution failed (command not found)");
        exit(EXIT_FAILURE);
    } 
    else {
        // 4. Parent process code - wait for child
        int status;
        wait(&status); // Waiting for the child process to terminate
        printf("----------------------------------------\n");
        printf("[Parent] Child process terminated. Resuming parent execution.\n");
    }

    return 0;
}