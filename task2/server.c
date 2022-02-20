#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <pthread.h>
#include <unistd.h>

#include "evalPostfix.h"

#define maxLen 500

/** Parameters which are passed in thread_function ( thread_handler )
 * @param socketfd - connected_client_fd
 */
struct parameters_to_thread_handler{
    int sock_fd;
};
typedef struct parameters_to_thread_handler parameters_to_thread_handler;


int portNumber; /* portNumber of the server */


FILE *fptr;
pthread_mutex_t lock;


void logMessage(const char *msg, bool terminate);

void server();
void *thread_handler(void *arg);

void writeInFile(int connected_client_fd, char exp[], char res[]);


int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "For server -> Usage : %s [PORT_NUMBER]\n", argv[0]);
        exit(1);    
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        logMessage("mutex init has failed. Terminating...!", true);
    }

    portNumber = atoi(argv[1]);
    server();

    return 0;
}

/** This functions prints messages on the screen
 * @param msg message to be printed
 * @param terminate whether to terminate after printing this message
 * @return none
 */
void logMessage(const char *msg, bool terminate){
    if(terminate){ /* failure */
        perror(msg);
        exit(1);  
    }
    printf("%s\n", msg);
}

/**
 * @brief This function creates server 
 * and handles all server operations
 * 
 */
void server(){
    logMessage("Starting the server...", false);
    int sock_fd, connected_client_fd;

    struct sockaddr_in server_address;


    // @Step 1 : Create a socket using socket( ) system call.

    /* Creating socket and verification */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
        logMessage("Failed to create socket. Terminating", true);
    }
    logMessage("Server socket created sucessfully...", false);


    // @Step 2 : Bind server’s address and port using bind( )  system call.

    /* Bind a name to the socket. */
    bzero( (char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portNumber);

    // Binding newly created socket to given IP and verification.
    if(bind(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        logMessage("Binding Failed", true);
    logMessage("Server socket binded sucessfully...", false);


    // @Step 3 : Convert the socket into a listening socket using listen() system call.

	// Now server is ready to listen and verification
    if (listen(sock_fd, 10) < 0) {
        logMessage("Listen call on the socket failed. Terminating.", true); /* terminate */
    }
    logMessage("Listening for client connections...", false);

    /* Listens indefinitely */
    while(true){
        struct sockaddr_in client_address; /* client adress */
        int client_len = sizeof(client_address); /* address length could change */    
    
        logMessage("Waiting for incoming connections...\n", false);
        connected_client_fd = accept(sock_fd, (struct sockaddr*)&client_address, &client_len ); /* accept blocks */
        if (connected_client_fd < 0) {
            logMessage("accept() failed. Terminating.", true); 
        }
        printf("Connected Succesfully with client_id: %d\n", connected_client_fd);

        pthread_t active_id;
        parameters_to_thread_handler arg;
        arg.sock_fd = connected_client_fd;

        /* Start a worker thread to handle the received connection. */
        logMessage("SERVER: Creating a worker thread...\n", false);
        
        int status = pthread_create(&active_id, NULL, thread_handler, &arg);
        if (status) {
            logMessage("SERVER: Failed to create thread.", false);
        }
    }

    // Closing the socket.
    close(sock_fd);
}

/** THREAD_FUNCTION handles request of a client (takes postfix expressions from the cilent and evaluates them).
 * @param arg arguments
 * @return NULL
 */
void *thread_handler(void *arg){
    parameters_to_thread_handler *ptr = (parameters_to_thread_handler*)arg;
    int connected_client_fd = ptr->sock_fd;

    printf("Starting server thread for connected client_id: %d\n\n", connected_client_fd);

    char buffer[maxLen];

    /* taking request from client */
    // int temporary = 1;
    // while(temporary--){
    while(true){
        bzero(buffer, sizeof(buffer));
        printf("Waiting for expression from client_id: %d..\n", connected_client_fd);

        // read the expression from client and copy it in buffer
        int numBitsRead = read(connected_client_fd, buffer, sizeof(buffer));

        if(numBitsRead <= 0){
            printf("Failure reading expression from client_id: %d...\n", connected_client_fd);
            continue;
        }

        if(
            0 == strncmp(buffer, "quit", 4) || 
            0 == strncmp(buffer, "exit", 4)
        ){
            printf("Client_id: %d...EXITED\n", connected_client_fd);
            break;
        }

        printf("Expression read from client_id %d -> %s", connected_client_fd, buffer);

        float value = evaluatePostfix(buffer);
        char res[maxLen];
        sprintf(res, "%.2f", value);

        pthread_mutex_lock(&lock);
        // save result for current query in "server_records.txt"
        writeInFile(connected_client_fd, buffer, res);
        pthread_mutex_unlock(&lock);

        // sending the result back to the client.
        write(connected_client_fd, res, strlen(res));
    }

    printf("SERVER: thread_function: Done. !Terminating worker thread for client_fd: %d !!\n", connected_client_fd);
    close(connected_client_fd);

    pthread_exit(NULL);  /* Must be the last statement */
    return NULL;
}


/**
 * @brief This function writes the query & query result
 * in below format
 * "<client_id> <query> <answer> <time_elapsed>"
 * 
 * @param connected_client_fd client_id
 * @param exp Input postfix expression
 * @param res value of input postfix expression
 */
void writeInFile(int connected_client_fd, char exp[], char res[]){
    fptr = fopen("server_records.txt", "a");

    char record[maxLen];
    bzero(record, sizeof(record));
    
    /* Prepare record in format "<client_id> <query> <answer> <time_elapsed>" */
    sprintf(record, "%d", connected_client_fd);

    record[strlen(record)] = ' ';

    int idx = strlen(record);
    int itr = 0;
    while(exp[itr] != '\n'){
        record[idx++] = exp[itr++];
    }

    record[idx++] = ' ';

    itr = 0;
    while(res[itr] != '\0'){
        record[idx++] = res[itr++];
    }
    record[idx++] = '\n';

    fprintf(fptr, record);
    fclose(fptr);
}
