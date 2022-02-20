#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define maxLen 500


int portNumber; /* portNumber of the server */


void logMessage(const char *msg, bool terminate);

void server();
void serverOperation(int connected_client_fd);
void reverse(char s[]);

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "For server -> Usage : %s [PORT_NUMBER]\n", argv[0]);
        exit(1);    
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
    if (listen(sock_fd, 2) < 0) {
        logMessage("Listen call on the socket failed. Terminating.", true); /* terminate */
    }
    logMessage("Listening for client connections...", false);


    // @Step 4 : Wait for client connection to complete using accept()system call.

    struct sockaddr_in client_address; /* client adress */
    int client_len = sizeof(client_address); /* address length could change */

    logMessage("Waiting for incoming connections...", false);
    connected_client_fd = accept(sock_fd, (struct sockaddr*)&client_address, &client_len ); /* accept blocks */
    if (connected_client_fd < 0) {
        logMessage("accept() failed. Terminating.", true); 
    }
    logMessage("Connection Established successfully...\n", false);

    // Calling function for receiving string from client and reversing it.
    serverOperation(connected_client_fd);

    // Closing the socket.
    close(sock_fd);
}

/**
 * @brief Function to receive string from client and reverse it.
 * 
 * @param connected_client_fd clients connection fd.
 */
void serverOperation(int connected_client_fd){
    char buffer[maxLen];

    // Reading indefinitely
    while(true){
        logMessage("Waiting for message from client...", false);

        bzero(buffer, maxLen);

        // read the message from client and copy it in buffer
        int numBitsRead = read(connected_client_fd, buffer, sizeof(buffer));
        
        if(
            0 == strncmp(buffer, "quit", 4) || 
            0 == strncmp(buffer, "exit", 4)
        ){
            logMessage("Client Exit...\n", false);
            break;
        }

        printf("String read from client: %s", buffer);

        // reverse the string.
        reverse(buffer);

        printf("Reverse string: %s\n", buffer);

        // sending the reverse string back to the client.
        write(connected_client_fd, buffer, strlen(buffer));
    }
    close(connected_client_fd);
}

/**
 * @brief UTILITY function for reversing the input string.
 * 
 * @param s Input string.
 */
void reverse(char s[]){
    char end = '\0';
    char endline = '\n';

    // getting the length of input string.
    int lastIdx = 0;
    while(lastIdx+1 < maxLen){
        if(s[lastIdx+1] == endline)break;
        lastIdx++;
    }

    // printf("size of input string is %d\n", lastIdx+1);

    // reversing the input string.
    int firstIdx = 0;
    while(firstIdx <= lastIdx){
        char tmp = s[firstIdx];
        s[firstIdx] = s[lastIdx];
        s[lastIdx] = tmp;

        firstIdx++;
        lastIdx--;
    }
}



