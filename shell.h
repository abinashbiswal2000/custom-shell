#ifndef SHELL_H
#define SHELL_H

// #include <signal.h>

int matchStrings(char *a, char *b);
// extern volatile sig_atomic_t is_waiting_for_input;
void ctrlCSignalHandler(int sig);

#endif
