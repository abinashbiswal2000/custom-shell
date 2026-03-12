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
