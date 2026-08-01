/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];

    // Main Interactive Shell Loop
    while (1) {
        // 1. Display Prompt
        printf("myshell> ");
        fflush(stdout); // Flush buffer to ensure prompt shows immediately

        // 2. Read User Input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle EOF / Ctrl+D
            printf("\nExiting shell...\n");
            break;
        }

        // 3. Remove trailing newline character ('\n')
        input[strcspn(input, "\n")] = '\0';

        // 4. Handle empty input (User just hit Enter)
        if (strlen(input) == 0) {
            continue;
        }

        // 5. Handle Exit Condition
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // 6. Temporary echo for testing user input capture
        printf("You entered: %s\n", input);
    }

    return 0;
}
*/

/*------------------------------------------------------------------------------------*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        // 1. Display Prompt
        printf("myshell> ");
        fflush(stdout);

        // 2. Read User Input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }

        // 3. Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // 4. Handle empty input
        if (strlen(input) == 0) {
            continue;
        }

        // 5. Tokenize Input into Arguments (e.g. "ls -l" -> ["ls", "-l", NULL])
        int arg_count = 0;
        char *token = strtok(input, " \t");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL; // NULL-terminate the arguments array

        // 6. Handle Exit Condition
        if (strcmp(args[0], "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // 7. Process Execution using fork() & execvp()
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork failed");
        } else if (pid == 0) {
            // Child Process: Execute command
            if (execvp(args[0], args) < 0) {
                perror("Command failed");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent Process: Wait for child to complete
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
*/

/*------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        // 1. Display Prompt
        printf("myshell> ");
        fflush(stdout);

        // 2. Read User Input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }

        // 3. Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // 4. Handle empty input
        if (strlen(input) == 0) {
            continue;
        }

        // 5. Tokenize Input into Arguments (e.g. "ls -l" -> ["ls", "-l", NULL])
        int arg_count = 0;
        char *token = strtok(input, " \t");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL; // NULL-terminate the arguments array

        // 6. Handle Exit Condition
        if (strcmp(args[0], "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        // 7. Process Execution using fork() & execvp()
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork failed");
        } else if (pid == 0) {
            // Child Process: Print PIDs to demonstrate parent-child relationship
            printf("[Child Process] PID: %d | Parent PID: %d\n", getpid(), getppid());
            
            if (execvp(args[0], args) < 0) {
                perror("Command failed");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent Process: Wait for child to complete
            int status;
            waitpid(pid, &status, 0);
        }
    }