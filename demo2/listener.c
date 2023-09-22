#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
char buf[BUFFER_SIZE], *buf_head = buf, *buf_tail = buf;

void get_next() {
    if (buf_head != buf_tail) { ++buf_head;  return ; }
    int n = read(0, buf, BUFFER_SIZE);
    if (n < 0) {
        printf("Read Failed\n");
        exit(1);
    }
    if (n == 0) {
        printf("Read EOF\n");
        exit(1);
    }
    buf_tail = buf + n;
}

int main(int argc, char *argv[]) {
    get_next();

    while (1) {
        printf("[0x%02x, %c]\n", *buf_head, *buf_head);
        get_next();
    }
}