/*
To Apply Escape Sequences, Store Command History, Navigate Previous Commands, Navigate Next Commands, Update Input Buffer, Test Recall Functionality.
To Allocate Buffers Dynamically, Resize Arrays, Prevent Buffer Overflow, Manage Linked Lists, Release Memory Correctly, Verify with Valgrind.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define PROMPT "myshell> "
#define BUFFER_SIZE 1024

// Doubly linked list node for command history
typedef struct HistoryNode {
    char *cmd;
    struct HistoryNode *prev;
    struct HistoryNode *next;
} HistoryNode;

HistoryNode *history_head = NULL;
struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Add a command to the history doubly linked list
void push_history(const char *cmd) {
    if (!cmd || strlen(cmd) == 0) return;
    HistoryNode *node = malloc(sizeof(HistoryNode));
    node->cmd = strdup(cmd);
    node->prev = NULL;
    node->next = history_head;
    if (history_head) {
        history_head->prev = node;
    }
    history_head = node;
}

// Read raw characters with ANSI escape sequence handling (Up/Down arrow navigation)
int read_line_with_history(char *buffer, int max_len) {
    int idx = 0;
    char c;
    HistoryNode *history_curr = NULL;
    buffer[0] = '\0';

    enable_raw_mode();

    while (1) {
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            disable_raw_mode();
            return -1; // EOF (Ctrl+D)
        }

        // Enter key
        if (c == '\n' || c == '\r') {
            buffer[idx] = '\0';
            printf("\r\n");
            break;
        }
        // Escape sequence (Arrow keys: \033[A, \033[B)
        else if (c == '\033') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                if (seq[1] == 'A') { // Up Arrow (Older commands)
                    if (history_curr == NULL && history_head != NULL) {
                        history_curr = history_head;
                    } else if (history_curr && history_curr->next != NULL) {
                        history_curr = history_curr->next;
                    }

                    if (history_curr) {
                        strncpy(buffer, history_curr->cmd, max_len - 1);
                        buffer[max_len - 1] = '\0';
                        idx = strlen(buffer);
                        // Clear current line and re-render prompt + command
                        printf("\33[2K\r%s%s", PROMPT, buffer);
                        fflush(stdout);
                    }
                } 
                else if (seq[1] == 'B') { // Down Arrow (Newer commands)
                    if (history_curr && history_curr->prev != NULL) {
                        history_curr = history_curr->prev;
                        strncpy(buffer, history_curr->cmd, max_len - 1);
                        buffer[max_len - 1] = '\0';
                        idx = strlen(buffer);
                        printf("\33[2K\r%s%s", PROMPT, buffer);
                        fflush(stdout);
                    } else if (history_curr && history_curr->prev == NULL) {
                        history_curr = NULL;
                        buffer[0] = '\0';
                        idx = 0;
                        printf("\33[2K\r%s", PROMPT);
                        fflush(stdout);
                    }
                }
            }
        }
        // Backspace key
        else if (c == 127 || c == 8) {
            if (idx > 0) {
                idx--;
                buffer[idx] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        }
        // Standard printable characters
        else if (c >= 32 && c <= 126) {
            if (idx < max_len - 1) {
                buffer[idx++] = c;
                buffer[idx] = '\0';
                putchar(c);
                fflush(stdout);
            }
        }
    }

    disable_raw_mode();
    return idx;
}

// Dynamically allocate and resize argument arrays
char **tokenize_dynamic(char *line, int *token_count) {
    int capacity = 4;
    int count = 0;
    char **tokens = malloc(capacity * sizeof(char *));
    if (!tokens) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[count++] = strdup(token);
        if (count >= capacity) {
            capacity *= 2;
            char **new_tokens = realloc(tokens, capacity * sizeof(char *));
            if (!new_tokens) {
                perror("Reallocation failed");
                exit(EXIT_FAILURE);
            }
            tokens = new_tokens;
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[count] = NULL;
    *token_count = count;
    return tokens;
}

// Free dynamically allocated tokens
void free_tokens(char **tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

// Free the history linked list to prevent leaks
void free_history(HistoryNode *head) {
    HistoryNode *curr = head;
    while (curr) {
        HistoryNode *temp = curr;
        curr = curr->next;
        free(temp->cmd);
        free(temp);
    }
}

int main() {
    char input[BUFFER_SIZE];
    char temp_line[BUFFER_SIZE];

    while (1) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        if (read_line_with_history(input, sizeof(input)) == -1) {
            printf("\nExiting shell...\n");
            break;
        }

        if (strlen(input) == 0) {
            continue;
        }

        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Save command to dynamic history
        push_history(input);

        // Copy input for tokenization (strtok modifies source string)
        strncpy(temp_line, input, sizeof(temp_line));
        temp_line[sizeof(temp_line) - 1] = '\0';

        int token_count = 0;
        char **args = tokenize_dynamic(temp_line, &token_count);

        printf("Parsed %d tokens:\n", token_count);
        for (int i = 0; i < token_count; i++) {
            printf("  arg[%d] = %s\n", i, args[i]);
        }

        // Release dynamic token allocations
        free_tokens(args, token_count);
    }

    // Complete cleanup before process exits
    free_history(history_head);
    return 0;
}