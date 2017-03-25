#include <string.h>
#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <math.h>
#define MAX_STRING_SIZE 128
#define EPS 0.000001

typedef struct MessageData {
    long clientId;
    long transId;
    int amount;
    char command[MAX_STRING_SIZE];
} MessageData;

void PrintHelp() {
    printf("Usage:\n\
help                        -- print help\n\
check                       -- check balance\n\
cr_check                    -- check credit acc balance\n\
deposit [amount]            -- put money on the account\n\
withdraw [amount]           -- get money from the account\n\
transfer [amount] [ID]      -- transfer money on another account\n\
cr_deposit [amount]         -- put money on credit account\n\
cr_withdraw [amount]        -- get money from credit account\n\
exit                        -- exit program\n");
}

void SendCommand(MessageData* md, void* senderSocket) {
    zmq_msg_t reply;
    zmq_msg_init(&reply);

    zmq_msg_t zmqMessage;
    zmq_msg_init_size(&zmqMessage, sizeof(MessageData));

    memcpy(zmq_msg_data(&zmqMessage), md, sizeof(MessageData));
    zmq_msg_send(&zmqMessage, senderSocket, 0);
    zmq_msg_recv(&reply, senderSocket, 0);
    printf("%s\n", (char*)zmq_msg_data(&reply));
    zmq_msg_close(&zmqMessage);
    zmq_msg_close(&reply);
}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [port] [client ID]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    long id = atol(argv[2]);
    printf("Init ID = %li\n", id);
    if (!port || port > 49151) {
        printf("Error: invalid port!\n");
        return 2;
    }

    void* context = zmq_ctx_new();
    void* senderSocket = zmq_socket(context, ZMQ_REQ);
    char* address = (char*)malloc(sizeof(char) * 24);
    memcpy(address, "tcp://localhost:", 16);
    memcpy(address + 16, argv[1], strlen(argv[1]) + 1);
    zmq_connect(senderSocket, address);

    MessageData init;
    init.clientId = id;
    memcpy(init.command, "init\0", 5);

    zmq_msg_t initMessage;
    zmq_msg_init_size(&initMessage, sizeof(MessageData));
    memcpy(zmq_msg_data(&initMessage), &init, sizeof(MessageData));
    zmq_msg_send(&initMessage, senderSocket, 0);
    zmq_msg_close(&initMessage);

    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, senderSocket, 0);
    if(!strcmp((char*)zmq_msg_data(&reply), "ok"))
        printf("The client(%li) is initialized on the bank(%d)\n", id, port);
    else
        printf("%s\n", (char*)zmq_msg_data(&reply));
    zmq_msg_close(&reply);

    char command[MAX_STRING_SIZE];
    int active = 1, stat;
    PrintHelp();
    while (active) {
        printf("Enter the command: ");
        stat = scanf("%s", command);

        MessageData md;
        md.clientId = id;

        if (!strcmp(command, "help"))
            PrintHelp();
        else if (!strcmp(command, "check")) {
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
        }
        else if (!strcmp(command, "cr_check")) {
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
        }
        else if (!strcmp(command, "deposit")) {
            scanf("%d", &md.amount);
            if (md.amount < 0) {
                printf("Error: invalid amount!\n");
                continue;
            }
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
        }
        else if (!strcmp(command, "withdraw")) {
            scanf("%d", &md.amount);
            if (md.amount < 0) {
                printf("Error: invalid amount!\n");
                continue;
            }
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
        }
        else if (!strcmp(command, "cr_deposit")) {
            scanf("%d", &md.amount);
            if (md.amount < 0) {
                printf("Error: invalid amount!\n");
                continue;
            }
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);

        }
        else if (!strcmp(command, "cr_withdraw")) {
            scanf("%d", &md.amount);
            if (md.amount < 0) {
                printf("Error: invalid amount!\n");
                continue;
            }
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
        }
        else if(!strcmp(command, "transfer")){
            scanf("%d", &md.amount);
            if(md.amount < 0) {
                printf("Error wrong amount");
                continue;
            }
            scanf("%li", &md.transId);
            memcpy(md.command, command, strlen(command)+1);
            SendCommand(&md, senderSocket);
        }
        else if (!strcmp(command, "exit")) {
            memcpy(md.command, command, strlen(command) + 1);
            SendCommand(&md, senderSocket);
            active = 0;
        }
        else
            printf("Error: invalid command!\n");
    }
    zmq_close(senderSocket);
    zmq_ctx_destroy(context);
    return 0;
}