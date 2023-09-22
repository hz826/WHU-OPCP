#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "IO.h"

IO io;

int main(int argc, char *argv[]) {
    int fd1, fd2;
    printf("open begin\n");
    if ((fd1 = open("myfifo1", O_RDONLY)) < 0) { printf("cannot open fifo...\n");  exit(1); }
    if ((fd2 = open("myfifo2", O_WRONLY)) < 0) { printf("cannot open fifo...\n");  exit(1); }
    printf("open finish\n");

    io.fd_read = fd1;
    io.fd_write = fd2;

    while (1) {
        int x = io.readInt('\n');
        printf("recv %d\n", x);
        if (x == 0) break;

        io.writeInt(x-1);
        io.writeChar('\n');
    }

    close(fd1);
    close(fd2);
}