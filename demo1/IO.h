#ifndef IO_H
#define IO_H

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct IO {
    #define BUFFER_SIZE 1024
    #define isdigit(x) ('0' <= (x) && (x) <= '9')

    char buf[BUFFER_SIZE], *head = buf, *tail = buf;
    int fd_read = 0, fd_write = 1;
    bool init = false;

    char nextChar() {
        if (head == tail) {
            int n = read(fd_read, buf, BUFFER_SIZE);
            if (n < 0) {
                printf("Read Failed\n");
                exit(1);
            }
            if (n == 0) {
                printf("Read EOF\n");
                exit(1);
            }
            head = buf;
            tail = buf + n;
        }
        // printf("nextChar [0x%2x, %c]\n", *head, *head);
        return *head++;
    }

    int readInt(char endsWith) {
        char c = nextChar();
        if (!isdigit(c)) {
            printf("readInt Failed, FtrstChar = [0x%02x, %c]\n", c, c);
            exit(1);
        }
        int x = 0;
        while (isdigit(c)) {
            x = x * 10 + c - '0';
            c = nextChar();
        }
        if (endsWith != (char)-1 && endsWith != c) {
            printf("readInt Failed, endsWith = [0x%02x, %c]\n", c, c);
            exit(1);
        }
        return x;
    }

    int readSpace() {
        if (nextChar() != ' ') {
            printf("readSpace Failed\n");
            exit(1);
        }
    }

    int readEoln() {
        if (nextChar() != '\n') {
            printf("readEoln Failed\n");
            exit(1);
        }
    }

    void writeInt(int x) {
        char tmp[10];
        sprintf(tmp, "%d", x);
        write(fd_write, tmp, strlen(tmp));
        // for (char *c=tmp;*c;c++) writeChar(*c);
    }

    void writeChar(char c) {
        // printf("writeChar [0x%2x, %c]\n", c, c);
        write(fd_write, &c, 1);
    }
};

#endif