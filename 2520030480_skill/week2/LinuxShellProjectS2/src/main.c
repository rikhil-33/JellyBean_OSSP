#include <stdio.h>
#include <string.h>

int main()
{
    char input[100];
    
    while (1)
    {
        printf("myshell> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        
        if (strcmp(input, "exit\n") == 0)
        {
            printf("Exiting Shell...\n");
            break;
        }
        
        printf("You entered: %s", input);
    }
    
    return 0;
}