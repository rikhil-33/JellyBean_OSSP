/* 
CASE STUDY – 1: Creating a New Process
A Linux developer executes the following program:

</> C
pid = fork();

The program successfully creates a child process.
1A. 5 Marks
Arrange the major components involved in executing the fork() operation in the order in which control passes through them, starting from the user application and ending with the hardware resource. Relate each component to its position in the Linux software stack.
1B. 7 Marks
A student is asked to investigate the execution of fork() using a Linux system-monitoring/tracing tool. Construct an investigation plan that identifies:
the user-space program involved, 
the system call associated with process creation, 
the transition between User Mode and Kernel Mode, 
the major kernel service involved in creating the process, 
and the information that can be observed to confirm successful process creation. 
1C. 3 Marks
A malicious program attempts to create an unlimited number of processes to exhaust system resources. Analyze how Linux process-management and protection mechanisms can restrict or control such behavior.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("Parent PID: %d\n", getpid());
    
    // System Call Trigger
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child Process
        printf("Child Process Created! Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        sleep(2); // Pause for tracing/proc inspection
        exit(0);
    } else {
        // Parent Process
        printf("Parent process created child with PID: %d\n", pid);
        wait(NULL); // Prevent zombie state
    }

    return 0;
}