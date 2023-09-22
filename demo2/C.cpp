#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    unlink("myfifo1");
    unlink("myfifo2");
    if (mkfifo("myfifo1",0777) < 0) { printf("cannot create fifo...\n");  exit(1); }
    if (mkfifo("myfifo2",0777) < 0) { printf("cannot create fifo...\n");  exit(1); }

    system("g++ A.cpp -o A");
    system("g++ B.cpp -o B");
}