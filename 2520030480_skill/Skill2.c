/*
To Create Main Loop, Display Prompt, Read User Input, Handle Exit Conditions, Design Control Flow Diagram, Test Interactive Loop
To Capture Keyboard Input, Handle Backspace, Process Enter Key, Manage Input Buffer, Support Multi-Character Commands, Test User Interaction
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define PROMPT "myshell> "
#define BUFFER_SIZE 1024

struct termios orig_termios;

// Disable raw mode and restore standard terminal settings
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Enable raw mode to capture individual keystrokes (no Enter required, no echo)
void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode); // Ensure standard mode is restored on exit
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Turn off echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Displays the shell prompt
void display_prompt() {
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

// Custom read line function that processes raw keystrokes
int read_line_custom(char *buffer, int max_len) {
    int idx = 0;
    char c;

    enable_raw_mode();
    
    while (1) {
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            // Handle EOF (Ctrl+D)
            disable_raw_mode();
            return -1;
        }

        if (c == '\n' || c == '\r') {
            // Process Enter key
            buffer[idx] = '\0';
            printf("\r\n"); // Move cursor to next line visually
            break;
        } 
        else if (c == 127 || c == 8) {
            // Process Backspace key (ASCII 127 or 8)
            if (idx > 0) {
                idx--;
                printf("\b \b"); // Visually erase character on screen
                fflush(stdout);
            }
        } 
        else if (c >= 32 && c <= 126) {
            // Store standard printable characters
            if (idx < max_len - 1) {
                buffer[idx++] = c;
                putchar(c); // Echo the character manually
                fflush(stdout);
            }
        }
        // Arrow keys/escape sequences are ignored in this iteration
    }
    
    disable_raw_mode();
    return idx;
}

int main() {
    char input[BUFFER_SIZE];

    /* 
     Control Flow Diagram implementation:
     [ Display Prompt ] ---> [ Read User Input ] ---> [ Check Exit Conditions ]
             ^                                                   |
             |                                              (No) | (Yes)
             +----------------- [ Execute Command ] <------------+   v
                                                                 [ Terminate Shell ]
    */
    
    while (1) {
        display_prompt();

        // Capture input using our raw mode processor
        if (read_line_custom(input, sizeof(input)) == -1) {
            printf("\nExiting shell (EOF detected)...\n");
            break;
        }

        // Skip processing if input is just empty spaces or blank
        if (strlen(input) == 0) {
            continue;
        }

        // Handle the built-in exit command
        if (strcmp(input, "exit") == 0) {
            printf("Session closed. Goodbye!\n");
            break;
        }

        // Placeholder for the Execute Command stage
        printf("Executing command: %s\n", input);
    }

    return 0;
}