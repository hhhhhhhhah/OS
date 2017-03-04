#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/file.h>
#include <dirent.h>
#include <ctype.h>

typedef struct File {
    int pos;
    int fd;
    int linesSize;
    int lineCounter;
    int* lines;
    long pageSize;

    off_t offset;
    off_t fileSize;

    char* path;
    char* page;

}File;

void Open(File* file);
void rewindFile(File* file);
char getChar(File* file);
void printLine(File* file,int line);
char* getString(File* file, int line, int x1, int x2);
int checkIfOpened(char * pathToFile);
void zFunc(int* z, char* pattern, int patternLength);
void SPCount(int* z, int* sp2, int patternLength);
void findPosition(File* file, int position);
void KMP(File* file, char* pattern, int patternLength);
void replace(File* file, int line, int pos, char* str, int strlen);
int scanFile(File* file);
void printMenu();
void printUsage();

int main(int argc, char* argv[]) {
    File* file = (File*)malloc(sizeof(File));
    bool fileOpened = false;

    flock(file->fd, LOCK_EX);

    int q = 0;
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(argv[i][0] == '?') {
                printUsage();
                return 0;
            }
            if(argv[i][0] == 'f' && argv[i][1] == ':') {
                char* path = (char*)malloc(sizeof(char) * 256);
                int j = 2;
                while(argv[i][j] != '\0') {
                    path[j - 2] = argv[i][j];
                    j++;
                }
                path[j - 2] = '\0';
                if(j == 2) {
                    printf("ERROR: INCORRECT FILENAME\n");
                    exit(1);
                }

                int newfd = open(path, O_RDONLY | O_CREAT);
                if(newfd == -1) {
                    printf("ERROR: CAN'T OPEN FILE\n");
                    free(path);
                    continue;
                }

                struct stat file_info;
                fstat(newfd, &file_info);
                off_t fs = file_info.st_size;

                if(fileOpened) {
                    free(file->path);
                    close(file->fd);
                }
                file->path = path;
                file->fd = newfd;
                fileOpened = true;
                file->fileSize = fs;
                file->pageSize = sysconf(_SC_PAGESIZE);
            }

            if(argv[i][0] == 'l' && argv[i][1] == ':') {
                if(!fileOpened) {
                    printf("ERROR: NO OPEN FILE\n");
                    continue;
                }
                int j = 2;
                int line = 0;
                while(argv[i][j] != '\0') {
                    if(argv[i][j] >= '0'  && argv[i][j] <= '9') {
                        line *= 10;
                        line += (argv[i][j] - '0');
                        j++;
                    }
                    else {
                        printf("ERROR: WRONG ARGUMENT\n");
                        exit(1);
                    }
                }
                if(line == 0) {
                    printf("ERROR: WRONG ARGUMENT\n");
                    continue;
                }
                printLine(file,line);
                q++;
                break;
            }

            if(argv[i][0] == 's' && argv[i][1] == ':') {
                if(!fileOpened) {
                    printf("ERROR: NO OPEN FILE\n");
                    continue;
                }
                printf("Enter length of your pattern:\n");
                int len;
                scanf("%d", &len);
                char c;
                scanf("%c", &c);
                char* pattern = (char*)malloc(len * sizeof(char));
                printf("Enter your pattern:\n");
                for(int i = 0; i < len; ++i) {
                    scanf("%c", &pattern[i]);
                }
                scanf("%c", &c);
                KMP(file, pattern, len);
                q++;
                break;
            }
        }
    }


    if(q == 0) {
        //IDE FUNCTIONALITY
        if (!fileOpened) {
            printf("Enter filename:\n");
            file->path = (char *) malloc(256 * sizeof(char));
            scanf("%s", file->path);
            Open(file);
        }

        printMenu();
        while (1) {
            int cmd;
            scanf("%d", &cmd);

            if (cmd == 1) {
                printf("Enter number of your line:\n");
                int line;
                scanf("%d", &line);
                printLine(file, line);
            }

            if (cmd == 2) {
                printf("Enter length of your pattern:\n");
                int len;
                scanf("%d", &len);
                char c;
                scanf("%c", &c);
                char *pattern = (char *) malloc(len * sizeof(char));
                printf("Enter your pattern:\n");
                for (int i = 0; i < len; ++i) {
                    scanf("%c", &pattern[i]);
                }
                scanf("%c", &c);
                KMP(file, pattern, len);
            }

            if (cmd == 3) {
                printf("Enter line:\n");
                int line, pos, y1, y2;
                scanf("%d", &line);
                printf("Enter insert position: \n");
                scanf("%d", &pos);
                printf("Enter fragment [y1, y2]: \n");
                scanf("%d %d", &y1, &y2);

                int cnt = y2 - y1;
                if (cnt < 0 || pos + cnt >= y1) {
                    continue;
                }
                char *str = (char*)malloc(cnt*sizeof(char));
                str = getString(file, line, y1, y2);
                replace(file, line, pos, str, cnt);
                //  free(str);
            }

            if (cmd == 4) {
                if (checkIfOpened(file->path)) {
                    printf("FILE IS OPENED\nIt is blocked from other processes with flock\n");
                } else printf("FILE IS NOT OPENED\n");
            }


            if (cmd == 5) {
                char *newFile = (char *) malloc(sizeof(char) * 256);
                printf("Enter file name:\n");
                scanf("%s", newFile);
                int newfd = open(newFile, O_RDONLY | O_CREAT);
                if (newfd == -1) {
                    printf("ERROR: CAN'T OPEN FILE\n");
                    free(newFile);
                    continue;
                }

                flock(file->fd, LOCK_UN);
                flock(newfd, LOCK_EX);

                struct stat file_info;
                fstat(file->fd, &file_info);

                free(file->path);
                file->path = newFile;
                close(file->fd);
                file->fd = newfd;

                flock(newfd, LOCK_UN);
                flock(file->fd, LOCK_EX);
            }

            if (cmd == 6) {
                printf("\n1. Default\n");
                printf("2. Your size\n");
                long cmd1;
                scanf("%ld", &cmd1);
                if (cmd1 == 1) {
                    file->pageSize = sysconf(_SC_PAGESIZE);
                }
                if (cmd1 == 2) {
                    printf("How many pages to map:\n");
                    scanf("%ld", &cmd1);
                    file->pageSize = cmd1 * sysconf(_SC_PAGE_SIZE);
                }
            }

            if (cmd == 7) {
                printf("Filesize: %d\n", (int) file->fileSize);
                int cnt = scanFile(file);
                if (cnt != 0) {
                    free(file->lines);
                }
                rewindFile(file);
                if (cnt == 0) {
                    cnt++;
                }
                printf("Number of lines: %d\n", cnt - 1);
            }
            if (cmd == 8) {
                printMenu();
            }

            if (cmd == 9) {
                free(file->path);
                close(file->fd);
                free(file);
                break;
            }

        }
    }
    flock(file->fd, LOCK_UN);
    return 0;
}
void Open(File* file) {
    file->fd = open(file->path, O_RDONLY | O_CREAT);
    if(file->fd == -1) {
        printf("Error while opening file\n");
        exit(1);
    }
    struct stat file_info;
    fstat(file->fd, &file_info);
    file->fileSize = file_info.st_size;
    file->pageSize = sysconf(_SC_PAGESIZE);
}

void rewindFile(File* file) {
    file->pos = 0;
    file->offset = 0;
    file->linesSize = 0;
    file->lineCounter = 0;
}

char getChar(File* file) {
    if(file->pos == file->fileSize) {
        if(munmap(file->page, (size_t)file->pageSize)) {
            printf("\nUNMAPPING ERROR!\n");
            exit(-1);
        }
        return -1;
    }
    if(file->pos % file->pageSize) {
        char c = file->page[file->pos % file->pageSize];
        file->pos++;
        return c;
    }
    else {
        if(file->pos) {
            if(munmap(file->page, (size_t)file->pageSize)) {
                printf("\nUNMAPPING ERROR!\n");
                exit(-1);
            }
            file->offset += file->pageSize;
        }

        file->page = (char*)mmap(NULL, (size_t)file->pageSize, PROT_READ, MAP_PRIVATE, file->fd, file->offset);
        if(file->page == MAP_FAILED) {
            printf("\nMAP FILED!\n");
            exit(1);
        }
        char c = file->page[file->pos % file->pageSize];
        file->pos++;
        return c;
    }
}

void printLine(File* file,int line) {
    rewindFile(file);
    if(file->fileSize == 0) {
        printf("File is empty\n");
        return;
    }
    if(line <= 0) {
        printf("Wrong line number\n");
        return;
    }
    bool printed;
    int lineCnt = 1;
    char text;
    while(1) {
        text = getChar(file);
        if(text == -1) {
            break;
        }
        if(text == '\n') {
            if(lineCnt == line) {
                printf("%c",text);
                printed = true;
            }
            lineCnt++;
            continue;
        }
        if(lineCnt == line) {
            printf("%c",text);
            printed = true;
        }
    }
    if(!printed) {
        lineCnt--;
        printf("Wrong line number. Number of lines: %d\n", lineCnt);
    }
    rewindFile(file);
    printf("\n");
}

char* getString(File* file, int line, int x1, int x2) {
    rewindFile(file);
    if (file->fileSize == 0){
        printf("ERROR: EMPTY FILE\n");
        exit(1);
    }
    if (line <= 0) {
        printf("ERROR: INCORRECT LINE\n");
        exit(1);
    }

    int lineCnt = 1;
    int posCnt = 0;
    bool foundLine;
    bool foundString;
    char text;
    int length = x2 - x1;

    char* result = (char*)malloc(length*sizeof(char));

    while(1) {
        text = getChar(file);
        if(text == -1) {
            break;
        }
        if(text == '\n') {
            if(lineCnt == line) {
                posCnt++;
                if ((posCnt == x1)||(posCnt > x1 && foundLine == true &&posCnt <= x2)) {
                    foundString = true;
                    printf("%c",text);
                    result[posCnt-x1] = text;
                }
                foundLine = true;
            }
            lineCnt++;
            posCnt = 0;
            continue;
        }
        if(lineCnt == line) {
            posCnt++;
            if((posCnt == x1)||(posCnt > x1 && foundLine == true &&posCnt <= x2)) {
                foundString = true;
                printf("%c",text);
                result[posCnt-x1] = text;
            }
            foundLine = true;
        }
    }

    if(!foundLine || !foundString) {
        lineCnt--;
        exit(1);
    }
    rewindFile(file);
    printf("\n");
    return result;
}

int checkIfOpened(char * pathToFile) {
    DIR *d = opendir("/proc/self/fd");
    if (d) {
        struct dirent *entry;
        struct dirent *result;
        entry = malloc(sizeof(struct dirent) + NAME_MAX + 1);
        result = 0;
        while (readdir_r(d, entry, &result) == 0) {
            if (result == 0) break;
            if (isdigit(result->d_name[0])) {
                char path[NAME_MAX+1];
                char buf[NAME_MAX+1];
                snprintf(path, sizeof(path), "/proc/self/fd/%s",
                         result->d_name);
                ssize_t bytes = readlink(path, buf, sizeof(buf));
                buf[bytes] = '\0';
                if (strcmp(pathToFile, buf) == 0) break;
            }
        }
        free(entry);
        closedir(d);
        if (result) return 1;
    }
    return 0;

}

void zFunc(int* z, char* pattern, int patternLength) {
    for(int i = 0; i < patternLength;i++) {
        z[i] = 0;
    }
    for(int i = 1; i < patternLength;i++) {
        for(int j = i; j < patternLength; ++j) {
            if(pattern[i] == pattern[j - i]) {
                z[i]++;
            }
            else {
                break;
            }
        }
    }
}

void SPCount(int* z, int* sp2, int patternLength) {
    sp2[0] = 0;
    for(int j = patternLength - 1; j > 0; j--) {
        int i = j + z[j] - 1;
        sp2[i] = z[j];
    }
}

void findPosition(File* file, int position) {
    int i = file->lineCounter - 1;
    while(file->lines[i] > position) {
        i--;
        if(i == 0) {
            break;
        }
    }
    int k = position - file->lines[i];
    printf("%d %d\n",i + 2,k);
}

void KMP(File* file, char* pattern, int patternLength) {
    if(patternLength == 0) {
        return;
    }
    rewindFile(file);
    int* z = (int*)malloc(patternLength * sizeof(int));
    int* sp2 = (int*)malloc(patternLength * sizeof(int));

    zFunc(z,pattern,patternLength);
    SPCount(z,sp2,patternLength);
    free(z);

    char text;
    bool entries = false;

    int n = patternLength - 1;
    int pCnt = 0;

    file->linesSize = 100;
    file->lines = (int*)malloc(file->linesSize * sizeof(int));
    while(1) {
        text = getChar(file);

        if(text == '\n') {
            file->lines[file->lineCounter] = file->pos;
            file->lineCounter++;
            if(file->lineCounter == file->linesSize) {
                file->linesSize *= 2;
                file->lines = (int*)realloc(file->lines,(size_t)file->linesSize);
                if(file->lines == NULL) {
                    printf("\nRealloc failed\n");
                    exit(11);
                }
            }
        }
        if(text == -1) {
            break;
        }
        if((pCnt <= n) && (pattern[pCnt] == text)) {
            pCnt++;
            while(1) {
                if(pCnt == n + 1) {
                    if(!entries) {
                        printf("Entries:\n");
                    }
                    findPosition(file,file->pos - n);
                    entries = true;
                    break;
                }
                text = getChar(file);
                if(text == -1) {
                    if(!entries) {
                        printf("No entries\n");
                        free(file->lines);
                        rewindFile(file);
                        return;
                    }
                }
                if(text == '\n') {
                    file->lines[file->lineCounter] = file->pos;
                    file->lineCounter++;
                    if(file->lineCounter == file->linesSize) {
                        file->linesSize *= 2;
                        file->lines = (int*)realloc(file->lines,(size_t)file->linesSize);
                        if(file->lines == NULL) {
                            printf("\nRealloc failed\n");
                            exit(11);
                        }
                    }
                }

                if(pattern[pCnt] == text) {
                    pCnt++;
                }
                else {
                    break;
                }
            }
        }
        if(pCnt == 0) {
            continue;
        }
        else {
            pCnt = sp2[pCnt - 1];
        }
    }
    if(!entries) {
        printf("No entries\n");
    }
    free(file->lines);
    rewindFile(file);
}

int scanFile(File* file) {
    if(file->fileSize == 0) {
        return 0;
    }
    rewindFile(file);
    file->linesSize = 100;
    file->lines = (int*)malloc(file->linesSize * sizeof(int));
    while(1) {
        char text = getChar(file);
        if(text == -1) {
            break;
        }
        if(text == '\n') {
            file->lines[file->lineCounter] = file->pos;
            file->lineCounter++;
            if(file->lineCounter == file->linesSize) {
                file->linesSize *= 2;
                file->lines = (int*)realloc(file->lines,file->linesSize * sizeof(int));
            }
        }
    }
    file->lines[file->lineCounter] = (int)file->fileSize;
    file->lineCounter++;
    int res = file->lineCounter;
    rewindFile(file);
    return res;
}

void replace(File* file, int line, int pos, char* str, int strlen) {
    rewindFile(file);
    if(file->fileSize != 0 && line == 1 && pos == 0) {
        int tmp = open("tmpLab4", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(tmp == -1) {
            printf("ERROR: CAN'T OPEN TMP\n");
            exit(0);
        }
        write(tmp,str,strlen*sizeof(char));
        file->pos += strlen;

        char text;
        while(1) {
            text = getChar(file);
            if(text == -1) {
                break;
            }
            write(tmp,&text, sizeof(char));
        }

        close(file->fd);
        close(tmp);
        if(remove(file->path)) {
            printf("ERROR: CAN'T DELETE FILE\n");
            exit(1);
        }
        if(rename("tmpLab4",file->path)) {
            printf("ERROR: CAN'T RENAME\n");
            exit(1);
        }
        Open(file);
        rewindFile(file);
        free(str);
        return;
    }

    if(file->fileSize == 0 && ((line == 0 && pos == 0) || (line == 1 && pos == 1))) {
        int tmp = open("tmpLab4", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(tmp == -1) {
            printf("ERROR: CAN'T OPEN TMP\n");
            exit(0);
        }

        write(tmp, str, strlen * sizeof(char));
        close(file->fd);
        close(tmp);
        if(remove(file->path)) {
            printf("ERROR: CAN'T DELETE FILE\n");
            exit(1);
        }
        if(rename("tmpLab4",file->path)) {
            printf("ERROR: CAN'T RENAME\n");
            exit(1);
        }
        free(str);
        Open(file);
        rewindFile(file);
        return;
    }
    if(file->fileSize == 0) {
        printf("Wrong position\n");
        free(str);
        return;
    }

    file->lineCounter = scanFile(file);
    if(line > file->lineCounter + 1 || line <= 0) {
        printf("Wrong line\n");
        free(file->lines);
        free(str);
        return;
    }
    else {
        if(pos == 0) {
            line--;
            pos = file->lines[line - 1] - file->lines[line - 2];
        }
        int cnt;
        if(line > 1) {
            cnt = file->lines[line - 1] - file->lines[line - 2];
        }
        else {
            cnt = file->lines[line - 1];
        }

        if( pos <= 0 || pos > cnt ) {
            printf("Wrong position\n");
            free(file->lines);
            free(str);
            return;
        }
        int tmp = open("tmpLab4", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(tmp == -1) {
            printf("ERROR: CAN'T OPEN TMP\n");
            exit(0);
        }

        int posCnt = 0;
        int lineCnt = 1;
        bool inserted = false;
        while(1) {
            char text = getChar(file);
            posCnt++;
            if(text == -1) {
                close(file->fd);
                close(tmp);
                if(remove(file->path)) {
                    printf("ERROR: CAN'T DELETE FILE\n");
                    exit(1);
                }
                if(rename("tmpLab4",file->path)) {
                    printf("ERROR: CAN'T RENAME\n");
                    exit(1);
                }
                Open(file);
                free(file->lines);
                free(str);
                return;
            }

            if(!inserted &&lineCnt == line && posCnt == pos) {
                write(tmp,&text,sizeof(char));
                write(tmp,str,strlen*sizeof(char));
                inserted = true;
                file->pos += strlen;
                continue;
            }

            if(!inserted && text == '\n') {
                posCnt = 0;
                lineCnt++;
            }

            write(tmp,&text,sizeof(char));
        }
    }
}

void printMenu(){
    printf("------------------------------------\n");
    printf("| 1. Print line                    |\n");
    printf("| 2. Search for pattern            |\n");
    printf("| 3. Replace string                |\n");
    printf("| 4. Check if opened               |\n");
    printf("| 5. Open another file             |\n");
    printf("| 6. Set pagesize                  |\n");
    printf("| 7. Size of the file              |\n");
    printf("| 8. Print menu                    |\n");
    printf("| 9. Exit                          |\n");
    printf("------------------------------------\n");
}

void printUsage() {
    printf("\t?       \t-  \thelp\n");
    printf("\tf:<path>\t-  \topen file\n");
    printf("\tl:<number>\t-  \tprint line\n");
    printf("\ts: \t        -    \tsearch for the pattern\n");
}
