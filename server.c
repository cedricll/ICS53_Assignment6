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
#include <pthread.h>


#define PORT 30000
#define MAXLINE 30

///////////////////////////////////////////////////////////// make file operation functions here ////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////// end of file operation functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////

void *server_handler (void *fd_pointer);

int main(int argc, char const *argv[]) {
    printf("server started\n\n");
    // printf("user input1: %s\n", argv[0]); // ./server
    // printf("user input2: %s\n", argv[1]); // PORT

    int server_fd, new_socket, *new_sock; // valread,
    struct sockaddr_in address; 
    // struct sockaddr_in newAddr;
    struct sockaddr_in cliaddr;
    // int opt = 1; 
    // int addrlen = sizeof(address); 
    // int new_addrlen = sizeof(newAddr);

    socklen_t clilen;

    // pid_t childpid;

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

    bzero(&address, sizeof(address)); 
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

    // new code
    clilen = sizeof(cliaddr);

    while (1) 
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&cliaddr, &clilen)) < 0 ) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // printf("Connection accepted\n");
        printf("Connection accepted from %s: %d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        pthread_t server_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
        pthread_create(&server_thread, NULL, server_handler, (void *) new_sock);
    }

    return 0;
}

void *server_handler(void *fd_pointer) 
{
    // printf("Server Handler()\n");
    int sock = *(int *)fd_pointer;
    free(fd_pointer);

    char client_message[1024] = {0};
    
    size_t read_size; // , write_size; (int)
    // char *message;
    // char client_message[100] = {0};
    // char client_copy[100] = {0};
    // message = "\nHello server Handler\n";

    // static int send_once = 0;
    // if (send_once < 1) 
    // {
    //     // write(sock, message, strlen(message))

    //     send_once++;
    // }

    
    while (( read_size = read(sock, client_message, sizeof(client_message)) ) > 0) // recv -- read
    {
        // msg_size += read_size;
        // if statement with msg_size > buffer size
        // recv(sock, client_message, 1024, 0);

        if (strcmp(client_message, "quit") == 0) {
            break;
        }

        // strcpy(client_copy, client_message);
        // printf("From client: %s", client_copy);
        printf("Client: %s\n", client_message);

        char *buf = client_message;
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
        printf("cmd %s\n", cmd);
        printf("arg %s\n", arg);

//////////////////////////////////// call file functions here//////////////////////////////////////////////////////////

        // assign result with strcpy, strcpy(result, _____)

        // cmd is the command
        // arg is the argument
        // exaple: cmd = openRead, arg = a.txt; openRead(a.txt)
        

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

///////////////////////////////// end of calling file functions //////////////////////////////////////////////////////////

        // else if (strcmp(cmd, "quit") == 0) {
        //     printf("Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        //     break;
        // }

        strcpy(result, "result sent from server");              // DELETE THIS AFTER IMPLEMENTING FILE OPERATIONS ABOVE
        

        // write(sock, client_message, strlen(client_message));
        send(sock , result , strlen(result), 0);         
        bzero(client_message, sizeof(client_message));
        bzero(result, sizeof(result));
    }

    printf("A Client Disconnected");
    fflush(stdout);
    close(sock);
    return NULL;
}

// else {
//     // printf("Invalid command");
//     strcpy(result, "Invalid Command");
//     break;
// }

// if (strcmp(result, "Invalid syntax") != 0) {
//     printf("%s\n", buffer_copy );
// }

// star of old code
//         while (1) {
    //             char buffer[1024] = {0}; // placement before or in while loop?
    //             recv(new_socket, buffer, 1024, 0);
    //             // printf("server buffer: %s", buffer);
    //             if (strcmp(buffer, "quit") == 0) {
    //                 printf("Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    //                 break;
    //             }
    //             else { 
    //                 if (strcmp(buffer, "") == 0) {
    //                     continue;
    //                 } 
    //                 else {
    //                     printf("%s\n", buffer); // reads content from client
    //                 }

    //                 char buffer_copy[1024];
    //                 strcpy(buffer_copy, buffer);

    //                 char *buf = buffer;
    //                 // printf("buf: %s\n", buf);
    //                 char result[1024] = {0};

    //                 char *found;
    //                 char cmd[30];
    //                 char arg[30];

    //                 int curr = 0;

    //                 while( (found = strsep(&buf," ")) != NULL ) {
    //                     // process what client sent
    //                     if (curr == 0) {
    //                         strcpy(cmd, found);
    //                         // printf("command: %s\n", cmd);
    //                     }
    //                     else if (curr == 1) {
    //                         strcpy(arg, found);
    //                         // printf("arg: %s\n", arg);
    //                     }
    //                     else {
    //                         break;
    //                     }
    //                     curr++;
    //                 }

    //                 // printf("cmd %s\n", cmd);

    //                 //////////////////////////////////////////////////////////  FILE OPERATIONS STARTED  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //                 // assign result with strcpy and use arg parameter
    //                 if (strcmp(cmd, "openRead") == 0) { 
    //                     printf("openRead Function\n");
    //                 }

    //                 else if (strcmp(cmd, "openAppend") == 0) {
    //                     printf("openAppend Function\n");
    //                 }

    //                 else if (strcmp(cmd, "read") == 0) {
    //                     printf("read Function\n");
    //                 }
                    
    //                 else if (strcmp(cmd, "append") == 0) {
    //                     printf("append Function\n");
    //                 }

    //                 else if (strcmp(cmd, "close") == 0) {
    //                     printf("close Function\n");
    //                 }

    //                 else if (strcmp(cmd, "quit") == 0) {
    //                     printf("Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    //                     break;
    //                 }

                    

    //                 strcpy(result, "result sent from server");              // DELETE THIS AFTER IMPLEMENTING FILE OPERATIONS ABOVE

    //                 /////////////////////////////////////////////////////////  FILE OPERATIONS ENDED  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //                 send(new_socket , result , strlen(result) , 0 );
                    
    //                 bzero(buffer, sizeof(buffer));
    //                 // bzero(buffer_copy, sizeof(buffer_copy));
    //             }
    //         }
    //     }
    // }

    // close(new_socket);
    ///// end of old code