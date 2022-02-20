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

void client(char *ip_address);
void clientOperation(int sock_fd);


int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "For client -> Usage : %s [PORT_NUMBER] [server_ipAddress]\n", argv[0]);
        exit(1);    
    }
    portNumber = atoi(argv[1]);

    client(argv[2]);

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
 * @brief This function connects to server 
 * and handles all client operations
 * 
 * @param ip_address IP address of server host.
 */
void client(char *ip_address){
    int sock_fd, connected_client_fd;
    struct sockaddr_in server_address;
    struct hostent *server;

    /* Creating socket and verification */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
        logMessage("Error in socket creation. Terminating.", true);
    logMessage("Client socket created sucessfully...", false);

    /* Gathering server info */
    server = gethostbyname(ip_address);
    if(server == NULL)
        logMessage("Error, no such host exist.Terminating.", true);
    logMessage("Host identified...", false);

    bzero((char *)&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(portNumber);

    /* Setting connection with the server */
    if(connect(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)  
        logMessage("Connection Failed.", true);

    logMessage("CLIENT: Connected to server, about to write some stuff...\n", false);

    // Calling function for sending string from client and receiving reversed from server.
    clientOperation(sock_fd);

    // Closing the socket.
    close(sock_fd);
}

/**
 * @brief Function to send string from client and receive reversed string 
 * from the server.
 * 
 * @param connected_client_fd host's connection fd.
 */
void clientOperation(int sock_fd){
    char buffer[maxLen];

    // Reading indefinitely
    while(true){
        printf("Enter new expression to server:\n");

        bzero(buffer, maxLen);
        fgets(buffer, maxLen, stdin);

        // Sending postfix expression to server.
        write(sock_fd, buffer, strlen(buffer));

        if(
            0 == strncmp(buffer, "quit", 4) || 
            0 == strncmp(buffer, "exit", 4)
        ){
            logMessage("Client Exit...\n", false);
            break;
        }

        // read the reverse string from the server.
        bzero(buffer, maxLen);
        int numBitsRead = read(sock_fd, buffer, sizeof(buffer));

        printf("Server Replied: %s\n", buffer);
    }
}
