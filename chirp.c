#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char *argv[ ]){
	
	// Checks the arguments
	if (argc < 2)
		printf("Error: the passed message is null.\n");
	else if(argc > 2)
		printf("Error: the message must be enclosed within quotes.\n");

	// Creates the message

	char message[1000]="";
	char *username = strdup(getlogin()); 
	if(username == NULL)
		printf("Error: %s\n" + errno);
	else if(strlen(username) > 16)
		printf("Error: username longer than 16 char.\n");

	strcat(message, "FROM ");
	strcat(message, username); // login username
	strcat(message, "\n");
	strcat(message, argv[1]); // actual message
	strcat(message, "\n");
	
	// Creates a UDP socket
	
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		printf("Error: creation of UDP socket failed.\n");
		exit(1);
	}
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "224.0.0.22", &(addr.sin_addr.s_addr));
	addr.sin_port = htons(5010);
     
 	// Sends the message	 

	int buflen = sizeof(message);	

	if(sendto(fd, message, buflen, 0, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		printf("Sending datagram message error.\n");

	close(fd);
	
	return 0;
}

