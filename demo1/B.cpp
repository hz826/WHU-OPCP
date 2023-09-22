#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "IO.h"

IO io;

int main(int argc, char *argv[]) {
    while (1) {
        int x;
        scanf("%d", &x);
        if (x == 0) break;

        printf("%d\n", x-1);
        fflush(stdout);
    }
}