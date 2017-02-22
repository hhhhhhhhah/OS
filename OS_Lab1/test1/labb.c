#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char buff[2048];
    int file = open(argv[1], O_RDWR);
    read(file, buff, 2048);
    printf("File1:\n%s\n", buff);
    memset(buff, 0, 2048);

    printf("Add smth\n");
    scanf("%s", buff);
    write(file, buff, strlen(buff));
    printf("Written\n");

    char *filename = "test2.txt";
    int fd;
    fd = open(filename, O_RDWR|O_CREAT|O_TRUNC);
    memset(buff, 0, 2048);
    printf("F2 created\n");

    scanf("%s", buff);
    lseek(fd,1,SEEK_SET);
    write(fd, buff, strlen(buff));

    close(file);
    remove(filename);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    if (fork() == 0) {
        execl("/bin/echo", "echo", "this is", "message one", (char *) 0);
        perror("exec one failed");
        exit(1);
    }
    int res = mkdir("newdir",O_RDWR);
    if (!res) printf("New dir created!\n");
    rmdir("newdir");
    printf("Dir removed\n");
}
