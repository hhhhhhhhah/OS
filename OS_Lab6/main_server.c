
#include <string.h>
#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#define MAX_STRING_SIZE 128
#define BALANCE_WORD_SIZE strlen("Balance: ")
typedef struct MessageData {
    long clientId;
    long transId;
    int amount;
    char command[MAX_STRING_SIZE];
} MessageData;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [port]", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    if (!port || port > 49151) {
        printf("Error: invalid port!\n");
        return 2;
    }

    void* context = zmq_ctx_new();
    void* serverSocket = zmq_socket(context, ZMQ_REP);
    char* address = (char*)malloc(sizeof(char) * 14);
    memcpy(address, "tcp://*:", 8);
    memcpy(address + 8, argv[1], strlen(argv[1]) + 1);
    zmq_bind(serverSocket, address);
    printf("Bank %d starting\n", port);

    list *clients = (list*) malloc(sizeof(list));
    initList(clients);
    //TVector* clients = Load(argv[1]);
    char answer[MAX_STRING_SIZE];
    while(1) {
        //Save(clients, argv[1]);
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, serverSocket, 0);
        MessageData* md = (MessageData*)zmq_msg_data(&message);
        zmq_msg_t reply;

        if (!strcmp(md->command, "init")) {
            node* elem = searchItem(clients, md->clientId);
            if(elem != NULL){
                memcpy(answer, "Error: client already exist in the bank!\0", 41);
            }
            else if(elem == NULL){
                elem = newNode(md->clientId, 0);
                push(clients, elem);
                printf("Client %li added\n",&md->clientId);
                memcpy(answer, "OK.\0", 4);
            }

            /*if (!searchItem(clients, md->clientId)) {
                TElem* elem = NewElem(md->clientId, 0);
                Push(clients, elem);
                printf("Client %.2lf added\n", md->clientId);
                memcpy(answer, "ok\0", 3);
            }
            else
                memcpy(answer, "Error: client already exist in the bank!\0", 42);*/
        }
        else {
            node* client = searchItem(clients, md->clientId);
            if (client) {
                if (!strcmp(md->command, "check")) {
                    char balance[15];
                    snprintf(balance, sizeof(balance), "%d", client->amount);
                    memcpy(answer, "Balance: ", 9);
                    printf("Client amount: %d\n",client->amount);// char = %s\n", client->amount, balance);
                    memcpy(answer + BALANCE_WORD_SIZE, balance, strlen(balance));

                    //_itoa_s(client->amount, answer + 9, MAX_STRING_SIZE - 9, 10);
                }
                if (!strcmp(md->command, "cr_check")) {
                    char balance[15];
                    snprintf(balance, sizeof(balance), "%d", client->credit);
                    memcpy(answer, "Credit account balance: ", strlen("Credit account balance: "));
                    printf("Client credit: %d\n",client->amount);// char = %s\n", client->amount, balance);
                    memcpy(answer + strlen("Credit account balance: "), balance, strlen(balance));

                    //_itoa_s(client->amount, answer + 9, MAX_STRING_SIZE - 9, 10);
                }
                else if (!strcmp(md->command, "deposit")) {
                    client->amount += md->amount;
                    char balance[15];
                    snprintf(balance, sizeof(balance), "%d", client->amount);
                    memcpy(answer, "Balance: ", 9);
                    printf("Client amount: %d\n", client->amount);
                    memcpy(answer + BALANCE_WORD_SIZE, balance, strlen(balance));
                    //_itoa_s(client->amount, answer + 9, MAX_STRING_SIZE - 9, 10);
                }
                else if (!strcmp(md->command, "withdraw")) {
                    if (md->amount > client->amount) {

                        client->amount -= md->amount;
                        client->credit += client->amount;
                        client->amount = 0;
                        char balance[30] = "\nBalance: ";
                        char char_balance[30];
                        char char_cr_balance[20];
                        char credit_balance[100];

                        memcpy(credit_balance, "Taking cash from credit account!\n",
                               sizeof("Taking cash from credit account!\n"));
                        strcat(credit_balance, "Credit account balance: ");
                        //memcpy(credit_balance, "Credit account balance: ", strlen("Credit account balance: "));
                        snprintf(char_balance, sizeof(balance), "%d", client->amount);
                        snprintf(char_cr_balance, sizeof(char_cr_balance), "%d", client->credit);
                        strcat(credit_balance, char_cr_balance);
                        strcat(balance, char_balance);
                        strcat(credit_balance, balance);
                        memcpy(answer, credit_balance, sizeof(credit_balance));
                        //snprintf(credit_balance + sizeof("Credit account balance: "), sizeof(credit_balance), "%d", client->credit);

                        //memcpy(answer + strlen("Taking cash from credit account!\n"), balance, strlen(balance));
                        //memcpy(answer + strlen("Taking cash from credit account!\n") + sizeof("Credit account balance: ") + sizeof(credit_balance), credit_balance, strlen(credit_balance));
                        printf("%s", answer);
                    }
                    else {
                        client->amount -= md->amount;
                        char balance[15];
                        snprintf(balance, sizeof(balance), "%d", client->amount);
                        memcpy(answer, "Balance: ", 9);
                        printf("client amount: %d\n", client->amount);
                        memcpy(answer + BALANCE_WORD_SIZE, balance, strlen(balance));
                        //_itoa_s(client->amount, answer + 9, MAX_STRING_SIZE - 9, 10);
                    }
                }
                else if (!strcmp(md->command, "cr_deposit")) {
                    client->credit += md->amount;
                    char balance[15];
                    snprintf(balance, sizeof(balance), "%d", client->credit);
                    memcpy(answer, "Credit account balance: ", strlen("Credit account balance: "));
                    printf("Client amount: %d\n", client->amount);
                    memcpy(answer + strlen("Credit account balance: "), balance, strlen(balance));

                }
                else if (!strcmp(md->command, "cr_withdraw")) {
                    client->credit -= md->amount;

                    char balance[15];
                    snprintf(balance, sizeof(balance), "%d", client->credit);
                    memcpy(answer, "Credit account balance: ", strlen("Credit account balance"));
                    printf("client amount: %d\n", client->amount);
                    memcpy(answer + strlen("Credit account balance"), balance, strlen(balance));
                }
                else if(!strcmp(md->command, "transfer")){
                    node* transferClient = searchItem(clients, md->transId);
                    if(transferClient){
                        if(client->amount < md->amount){
                            client->amount -= md->amount;
                            client->credit += client->amount;
                            client->amount = 0;
                            transferClient->amount += md->amount;
                            char balance[30] = "\nBalance: ";
                            char char_balance[30];
                            char char_cr_balance[20];
                            char credit_balance[100];

                            memcpy(credit_balance, "Taking cash from credit account!\n",
                                   sizeof("Taking cash from credit account!\n"));
                            strcat(credit_balance, "Credit account balance: ");
                            snprintf(char_balance, sizeof(balance), "%d", client->amount);
                            snprintf(char_cr_balance, sizeof(char_cr_balance), "%d", client->credit);
                            strcat(credit_balance, char_cr_balance);
                            strcat(balance, char_balance);
                            strcat(credit_balance, balance);
                            memcpy(answer, credit_balance, sizeof(credit_balance));
                        }
                        else{
                            client->amount -= md->amount;
                            transferClient->amount += md->amount;
                            char balance[15];
                            snprintf(balance, sizeof(balance), "%d", client->amount);
                            memcpy(answer, "Balance: ", 9);
                            printf("client amount: %d\n", client->amount);
                            memcpy(answer + strlen("Balance: "), balance, strlen(balance));
                        }
                    }
                    else{
                        memcpy(answer,"Error no such ID\0",18);
                    }
                }
                else if (!strcmp(md->command, "exit")) {
                    removeItem(clients, md->clientId);
                    memcpy(answer, "Good bye!\0", 10);
                    printf("Client %.2li deleted\n", md->clientId);
                    break;
                }
            }
            else
                memcpy(answer, "Error: client not found in the bank!\0", 37);
        }
        zmq_msg_init_size(&reply, strlen(answer) + 1);
        memcpy(zmq_msg_data(&reply), answer, strlen(answer) + 1);

        zmq_msg_send(&reply, serverSocket, 0);
        zmq_msg_close(&reply);
        zmq_msg_close(&message);
        memset(answer, '\0', MAX_STRING_SIZE);
    }
    //CloseHandle(exit);
    zmq_close(serverSocket);
    zmq_ctx_destroy(context);
    free(clients);

    return 0;
}