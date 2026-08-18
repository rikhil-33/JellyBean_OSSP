#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>

void limit_max_processes() {
    struct rlimit rl;
    // Set maximum user processes limit to 10
    rl.rlim_cur = 10;
    rl.rlim_max = 10;

    if (setrlimit(RLIMIT_NPROC, &rl) != 0) {
        perror("Failed to set rlimit");
        exit(1);
    }
    printf("RLIMIT_NPROC successfully set to 10 processes.\n");
}

int main() {
    limit_max_processes();

    int count = 0;
    while (1) {
        pid_t pid = fork();

        if (pid < 0) {
            // Process creation restricted by kernel protection
            perror("\nfork() blocked by Linux kernel");
            printf("Total child processes created before limit hit: %d\n", count);
            break;
        } else if (pid == 0) {
            // Child process sleeps briefly
            sleep(5);
            exit(0);
        } else {
            count++;
            printf("Created process #%d (PID: %d)\n", count, pid);
        }
    }

    return 0;
}