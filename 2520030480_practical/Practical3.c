/*
Develop a C program using fork() that creates a parent and child process. Display the Process ID (PID), Parent Process ID (PPID), and process states at different stages of execution.
Design an experiment to observe process state transitions (Ready, Running, Waiting, Terminated) using Linux monitoring tools such as ps, top, and /proc. Document the observations.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("[Initial] Single Process | PID: %d | PPID: %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child Process
        printf("[Child - Running] PID: %d | PPID: %d\n", getpid(), getppid());
        printf("[Child - Transition] Entering Sleep (Waiting/Interruptible Sleep: 'S')...\n");
        sleep(4);
        printf("[Child - Running] Resumed execution.\n");
        printf("[Child - Terminating] Child exiting now.\n");
        exit(0);
    }
    else {
        // Parent Process
        printf("[Parent - Running] PID: %d | Child PID: %d\n", getpid(), pid);
        printf("[Parent - Transition] Sleeping to let child terminate (Creating Zombie state: 'Z')...\n");
        sleep(8); // Child will exit at t=4s and stay a zombie until parent reaps it at t=8s
        
        printf("[Parent - Reaping] Calling wait() to clean up child process entry...\n");
        wait(NULL);
        printf("[Parent - Completed] Child reaped. Exiting.\n");
    }

    return 0;
}