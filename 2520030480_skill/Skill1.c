/*
To Install Linux VM, Configure GCC, Setup Git Repository, Create Project Structure, Understand Shell Architecture, Build Initial Makefile

To Analyze process abstraction, execute fork(), understand exec() family, analyze parent-child relationships, inspect process tree, practice system call tracing
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("[Parent] Initial PID: %d, PPID: %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child execution path
        printf("[Child] PID: %d, Parent PID: %d\n", getpid(), getppid());
        char *args[] = {"ps", "-f", "--forest", NULL};
        execvp(args[0], args);
        
        // Executes only if execvp fails
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent execution path
        int status;
        wait(&status);
        printf("[Parent] Child %d terminated with exit status %d.\n", pid, WEXITSTATUS(status));
    }
    return 0;
}