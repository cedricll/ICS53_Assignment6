// Shuvam Raj Satyal, 13102529
// Cedric Lim, 24026891

#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h> //For errno - the error number


struct hostent *gethostbyname(const char *name);

#define PORT 30000
#define MAXLINE 30

void parseline(char* cmdline, const char *argv[]) 
{
	const char delimiter[2] = " ";
	char *token;
	int counter = 0;

	// Remove trailing newline character from input
	char *temp_pos;
	if ((temp_pos = strchr(cmdline, '\n')) != NULL) *temp_pos = '\0';

	// strtok returns one token at a time
	token = strtok(cmdline, delimiter);
	while (token != NULL) {
		argv[counter++] = token;
		token = strtok(NULL, delimiter);
	}
}

int hostname_to_ip(const char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	
	return 1;
}

int main(int argc, char const *argv[]) 
{
    int sock, valread; // sock = 0?
    struct sockaddr_in serv_addr; 

	// char server_reply[1000];
	ssize_t n;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; // exit(1)
    } 

	bzero(&serv_addr, sizeof(serv_addr));
	memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(argv[2])); // PORT 

	const char *hostname = argv[1];
	char ip[100];

	hostname_to_ip(hostname , ip);

	// Convert IPv4 and IPv6 addresses from text to binary form ?
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)  // 2nd param use to be hard-coded as "127.0.0.1"
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; // exit(1)
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; // exit(1)
    } 

	char cmd[MAXLINE]; // will contain content to send to the server 
    char cmdline[MAXLINE]; // for user inputs

	// new code
	while (1) 
	{
		char buffer[1024] = {0};
		printf("> ");
		// scanf("%s", buffer);
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin)) exit(0);  

		parseline(cmdline, argv);

		int buf_size = 100;
		char buf [buf_size];
		int cx;

		if (strcmp(argv[0], "openRead") == 0 ) { // openRead a.txt
			cx = snprintf(buf, buf_size, "%s %s", argv[0], argv[1]); 
			strcpy(cmd, buf);
        }

		else if (strcmp(argv[0], "openAppend") == 0 ) { // openAppend a.txt
            cx = snprintf(buf, buf_size, "%s %s", argv[0], argv[1]); 
            strcpy(cmd, buf);
        }

		else if (strcmp(argv[0], "read") == 0 ) { // read 2
            cx = snprintf(buf, buf_size, "%s %s", argv[0], argv[1]); 
            strcpy(cmd, buf);
        }

		else if (strcmp(argv[0], "append") == 0 ) { // append "hello"
            cx = snprintf(buf, buf_size, "%s %s", argv[0], argv[1]); 
            strcpy(cmd, buf);
        }

		else if (strcmp(argv[0], "close") == 0 ) { // append a.txt
            cx = snprintf(buf, buf_size, "%s %s", argv[0], argv[1]); 
            strcpy(cmd, buf);
        }

		else if (strcmp(argv[0], "quit") == 0) {
			send(sock, "quit", strlen("quit"), 0);
			close(sock);
			printf("Disconnected from server\n");
			exit(1);
			break;
		}

		// // posiiton this code if this snippet is needed
		else {
			printf("Invalid Command\n");
			// strcpy(cmd, "Invalid Command");
			continue;
		}

		printf("cmd sent to server: %s\n", cmd);

		send(sock, cmd, strlen(cmd), 0);

		// if (send(sock, buffer, strlen(buffer), 0) < 0)
		// {
		// 	printf("Error\n");
		// 	return -1;
		// }

		if (recv(sock, buffer, 1024, 0) < 0) 
		{
			printf("error recieving data");
			break;
		}

		printf("Server Reply: %s\n", buffer);
	}

	close(sock);

	return 0;
}