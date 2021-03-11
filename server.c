// Shuvam Raj Satyal, 13102529
// Cedric Lim, 24026891

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <netinet/in.h>
#include <string.h> 
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>


#define PORT 30000
#define MAXLINE 30

///////////////////////////////////////////////////////////// make file operation functions here ////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////// end of file operation functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    printf("server started\n\n");
    // printf("user input1: %s\n", argv[0]); // ./server
    // printf("user input2: %s\n", argv[1]); // PORT

    int server_fd, new_socket, valread, *new_sock; 
    struct sockaddr_in address; 
    struct sockaddr_in newAddr;
    int opt = 1; 
    int addrlen = sizeof(address); 
    int new_addrlen = sizeof(newAddr);

    pid_t childpid;

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // == or < ? 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 30000 
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, // | SO_REUSEPORT
    //                                               &opt, sizeof(opt))) 
    // { 
    //     perror("setsockopt"); 
    //     exit(EXIT_FAILURE); 
    // }

    memset(&address, '\0', sizeof(address));

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; //or inet_addr("127.0.0.1")
    address.sin_port = htons( atoi(argv[1]) ); // PORT: atoi(argv[1])

    // Forcefully attaching socket to the port 30000 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    if (listen(server_fd, 4) < 0) // 2nd param: defines the maximum length to which the queue of pending connections for sockfd may grow. 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
    //                    (socklen_t*)&addrlen))<0) 
    // { 
    //     perror("accept"); 
    //     exit(EXIT_FAILURE); 
    // } 

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&newAddr,  
                       (socklen_t*)&new_addrlen))<0) // (socklen_t*)&addrlen
        { 
            // perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        printf("Connection accepted from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        
        
        if ((childpid = fork()) == 0) {
            close(server_fd);

            while (1) {
                char buffer[1024] = {0}; // placement before or in while loop?
                recv(new_socket, buffer, 1024, 0);
                // printf("server buffer: %s", buffer);
                if (strcmp(buffer, "quit") == 0) {
                    printf("Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else { 
                    if (strcmp(buffer, "") == 0) {
                        continue;
                    } 
                    else {
                        printf("%s\n", buffer); // reads content from client
                    }

                    char buffer_copy[1024];
                    strcpy(buffer_copy, buffer);

                    char *buf = buffer;
                    // printf("buf: %s\n", buf);
                    char result[1024] = {0};

                    char *found;
                    char cmd[30];
                    char arg[30];

                    int curr = 0;

                    while( (found = strsep(&buf," ")) != NULL ) {
                        // process what client sent
                        if (curr == 0) {
                            strcpy(cmd, found);
                            // printf("command: %s\n", cmd);
                        }
                        else if (curr == 1) {
                            strcpy(arg, found);
                            // printf("arg: %s\n", arg);
                        }
                        else {
                            break;
                        }
                        curr++;
                    }

                    // printf("cmd %s\n", cmd);

                    //////////////////////////////////////////////////////////  FILE OPERATIONS STARTED  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    // assign result with strcpy and use arg parameter
                    if (strcmp(cmd, "openRead") == 0) { 
                        printf("openRead Function\n");
                    }

                    else if (strcmp(cmd, "openAppend") == 0) {
                        printf("openAppend Function\n");
                    }

                    else if (strcmp(cmd, "read") == 0) {
                        printf("read Function\n");
                    }
                    
                    else if (strcmp(cmd, "append") == 0) {
                        printf("append Function\n");
                    }

                    else if (strcmp(cmd, "close") == 0) {
                        printf("close Function\n");
                    }

                    else if (strcmp(cmd, "quit") == 0) {
                        printf("Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                        break;
                    }

                    

                    strcpy(result, "result sent from server");              // DELETE THIS AFTER IMPLEMENTING FILE OPERATIONS ABOVE

                    /////////////////////////////////////////////////////////  FILE OPERATIONS ENDED  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    send(new_socket , result , strlen(result) , 0 );
                    
                    bzero(buffer, sizeof(buffer));
                    // bzero(buffer_copy, sizeof(buffer_copy));
                }
            }
        }
    }

    close(new_socket);

    return 0;
}

// else {
//     // printf("Invalid command");
//     strcpy(result, "Invalid Command");
//     break;
// }

// if (strcmp(result, "Invalid syntax") != 0) {
//     printf("%s\n", buffer_copy );
// }