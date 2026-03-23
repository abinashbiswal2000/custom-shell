#include <signal.h>
#include <unistd.h>

int matchStrings(char *a, char *b) {
    int i = 0;
    while (a[i] == b[i]) {
        if (a[i] == '\0') {
            return 0;
        }
        i += 1;
    }
    return -1;
}

extern volatile sig_atomic_t is_waiting_for_input;

void ctrlCSignalHandler(int sig) {
    if (is_waiting_for_input == 1) {
        write(1, "\n>> ", 4);
    } else {
        write(1, "\n", 1);
    }
}