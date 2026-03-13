#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main () {

    char *inputStringPtr = NULL;
    int inputIndex = 0;
    size_t len = 0;

    size_t currentSize = 5;
    char **subStringsPtr = NULL;
    int subStrIndex = 0;
    int lastIndex = 0;


    while (1) {
        // Take Input
        printf(">> ");
        getline(&inputStringPtr, &len, stdin);
        inputIndex = 0;

        // Parse Input and get substrings
        subStringsPtr = (char **)malloc(sizeof(char *) * currentSize);
        subStrIndex = -1;
        while (inputStringPtr[inputIndex] != '\n') {
            
            if (inputStringPtr[inputIndex] == ' ') {
                inputIndex += 1;
                continue;
            }

            subStrIndex += 1;
            
            // Check for realloc
            if (subStrIndex == currentSize) {
                currentSize *= 2;
                char **temp = (char **)realloc(subStringsPtr, currentSize * sizeof(char *));
                if (temp == NULL) {
                    printf("Reallocation Failed\n");
                    return -1;
                }
                subStringsPtr = temp;
            }

            subStringsPtr[subStrIndex] = &inputStringPtr[inputIndex];

            while (inputStringPtr[inputIndex] != ' ' && inputStringPtr[inputIndex] != '\n') {
                inputIndex += 1;
            }
            if (inputStringPtr[inputIndex] == '\n') {
                inputStringPtr[inputIndex] = '\0';
                break;
            }
            inputStringPtr[inputIndex] = '\0';
            inputIndex += 1;
        }

        
        // Null terminating substring after checking if resizing is required
        subStrIndex += 1;
        if (subStrIndex == currentSize) {
            currentSize *= 2;
            char **temp = (char **)realloc(subStringsPtr, currentSize * sizeof(char *));
            if (temp == NULL) {
                printf("Reallocation Failed\n");
                return -1;
            }
            subStringsPtr = temp;
        }
        subStringsPtr[subStrIndex] = NULL;
        lastIndex = subStrIndex - 1;


        /*
        Make sense of the instruction

        if subStringsPtr[0] is a builtin:
            handle it directly
        else:
            fork → execvp(subStringsPtr[0], subStringsPtr) → wait
        */ 
       
        // Handling Built-in Instructions
        if (matchStrings(subStringsPtr[0] , "cd") == 0) {
            if (subStringsPtr[1] == NULL) {
                chdir(getenv("HOME"));
            } else if ( chdir(subStringsPtr[1]) != 0) {
                printf("Could not Change Directory");
            } 

        } else if (matchStrings(subStringsPtr[0] , "exit") == 0) {
            free(inputStringPtr);
            free(subStringsPtr);
            exit(0);
            // break;
            // Using exit(0) should be used here. as exit makes the intent clear that this is the end of the program.
            // exit(0) means everything ended all right. The argument of that function is the exit status.
        } else if (matchStrings(subStringsPtr[0] , "export") == 0) {
            if (subStringsPtr[1] == NULL) {
                printf("Wrong Input - Add a Key=Value pair\n");
            } else {
                char *a = subStringsPtr[1];
                int i = 1;
                int foundEqualToSign = 0;
                while (a[i] != '\0' && foundEqualToSign != 1) {
                    if (a[i] == '=') {
                        foundEqualToSign = 1;
                    }
                    i += 1;                }
                if (a[i] == '\0' && foundEqualToSign == 0) {
                    printf("No Equal To Sign Found\n");
                } else if (a[i] == '\0' && foundEqualToSign == 1) {
                    printf("Enter a value for your key\n");
                } else {
                    a[i-1] = '\0';
                    char *b = a + i;
                    setenv(a, b, 1);
                }
            }
        } else {
            // Handle external commands
            // fork, exec, wait
            pid_t pid = fork();
            if (pid < 0) {
                printf("Fork Failed\n");
            } else if (pid == 0) {
                if ( execvp(subStringsPtr[0], subStringsPtr) == -1) {
                    fprintf(stderr, "Execvp Failed - [%s] Command not Found\n", subStringsPtr[0]);
                    exit(127);
                }
            } else if (pid > 0) {
                // This is the parent process
                // Wait for child's completion
                int status;
                if (matchStrings(subStringsPtr[lastIndex], "&") == 0) {
                    subStringsPtr[lastIndex] = NULL;
                    waitpid(pid, &status, WNOHANG);
                } else {
                    waitpid(pid, &status, 0);
                }
            }
            
        }

        // Signal Handling



        // Free Resources and restore defaults
        free(inputStringPtr);
        inputStringPtr = NULL;
        inputIndex = 0;
        len = 0;
        
        free(subStringsPtr);
        subStringsPtr = NULL;
        subStrIndex = 0;
        currentSize = 5;
    }
    return 0;
}