#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main () {

    char *inputStringPtr = NULL;
    int inputIndex = 0;
    size_t len = 0;

    size_t currentSize = 5;
    char **subStringsPtr = NULL;
    int subStrIndex = 0;


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


        /*
        Make sense of the instruction
        
        if subStringsPtr[0] is a builtin:
            handle it directly
        else:
            fork → execvp(subStringsPtr[0], subStringsPtr) → wait
        */ 

        // fork, exec, wait code to be written


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