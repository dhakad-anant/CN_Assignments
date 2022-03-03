# CN_Assignments

## [Assignment 2](https://drive.google.com/file/d/1B03a-nN1AmRiv6mcNSBhf4aLrboZo-wq/view)

## [Repository Link](https://github.com/dhakad-anant/CN_Assignments)

### Submitter name: Anant Dhakad

### Roll No.: 2019CSB1070

### Course: Computer Networks (CS304)

=================================

### Steps for compilation (TASK 1)

Note: Move to directory "task1\" before executing these commands

1. For server
    
    // ignore warnings after this command
    - gcc -o server server.c    
    - ./server PORTNUMBER

    - Give input "exit" or "quit" from client to exit anytime.

2. For clients 
    
    // ignore warnings after this command
    - gcc -o client client.c    
    - ./client PORTNUMBER ipAddress

    - Give input "exit" or "quit" from client to exit anytime.


Example:

    - gcc -o server server.c
    - ./server 1080
    - gcc -o client client.c
    - ./client 1080 127.0.0.1

### Steps for compilation (TASK 2)

Note: Move to directory "task2\" before executing these commands

1. For server

    // ignore warnings after this command
    - gcc -o server server.c -pthread  
    - ./server PORTNUMBER

    - Give input "exit" or "quit" from client to exit anytime.

2. For clients

    // ignore warnings after this command
    - gcc -o client client.c    
    - ./client PORTNUMBER ipAddress

    - Give input "exit" or "quit" from client to exit anytime.


Example:

    - gcc -o server server.c
    - ./server 1080
    - gcc -o client client.c
    - ./client 1080 127.0.0.1
