#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "IO.h"

IO io;

int main(int argc, char *argv[]) {
    int fd1, fd2;
    printf("open begin\n");
    if ((fd1 = open("myfifo1", O_WRONLY)) < 0) { printf("cannot open fifo...\n");  exit(1); }
    if ((fd2 = open("myfifo2", O_RDONLY)) < 0) { printf("cannot open fifo...\n");  exit(1); }
    printf("open finish\n");

    io.fd_read = fd2;
    io.fd_write = fd1;

    for (int i=0;i<5;i++) {
        int x = rand() % 100 + 1;
        io.writeInt(x);
        io.writeChar('\n');
        printf("send %d\n", x);
        
        int y = io.readInt('\n');
        printf("recv %d\n", y);
    }

    io.writeInt(0);
    io.writeChar('\n');

    close(fd1);
    close(fd2);
}