#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    //reading from file and writing to it
    char buffer[2048];
    int file;
    file = open(argv[1], O_RDWR);
    read(file, buffer, 2048);
    printf("file:\n%s\n", buffer);
    memset(buffer, 0, 2048);
    printf("add something: ");
    scanf("%s", buffer);
    write(file, buffer, strlen(buffer));
    memset(buffer, 0, 2048);
    close(file);
    
    //creating new process
    if(fork() == 0){
        execl("/bin/date", "date", 0, 0);
        perror("exec one failed");
        exit(1);
    }

    //creating and deleting a new directory
    int dir = mkdir("newdir", O_RDWR);
    if(!dir){
        printf("new directory created\n");
    }
    rmdir("newdir");
    printf("directory deleted\n");
    
    //time and date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    printf("current date and time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    //creating and deleting new file
    int file2;
    file2 = open("newfile", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    write(file2, "that's new file", 20);
    lseek(file2,0,SEEK_SET);
    read(file2, buffer, 2048);
    printf("file2:\n%s\n", buffer);
    close(file2);
    remove("newfile");
    printf("newfile removed\n");
}
