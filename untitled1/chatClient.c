#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

const int BUF_SIZE = 1000;

void * receiveMessage(void * socket) {
    int sockfd, ret;
    char buffer[BUF_SIZE];
    sockfd = (int) socket;
    memset(buffer, 0, BUF_SIZE);
    for (;;) {
        ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
        if (ret < 0) {
            printf("Error receiving data!\n");
        } else {
            printf("server: ");
            fputs(buffer, stdout);
            //printf("\n");
        }
    }
}


int main(int argc, char *argv[]){
    int sockfd, ret;
    struct sockaddr_in serv_addr, cli_addr;
    char *server_addr_c, buffer[BUF_SIZE];
    pthread_t rThread;

    if(argc < 2){
        printf("client <ip address\n");
        exit(1);
    }

    server_addr_c = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Could not create socket\n");
    }
    printf("Socket created\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_addr_c);
    serv_addr.sin_port = htons(5000);

    ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(ret == -1){
        printf("Error connecting to the server\n");
        exit(1);
    }
    printf("Connected to the server\n");

    memset(buffer, 0, BUF_SIZE);
    printf("Enter messages one by one and press return\n");

    ret = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);
    if (ret) {
        printf("ERROR: Return Code from pthread_create() is %d\n", ret);
        exit(1);
    }

    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        if (ret < 0) {
            printf("Error sending data!\n\t-%s", buffer);
        }
    }

    close(sockfd);
    pthread_exit(NULL);

    return 0;
}
