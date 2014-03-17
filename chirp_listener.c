#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

void checkForIllegalChar(char *message){

	// Checks for invalid characters

	unsigned int i;
	for(i = 0; i < strlen(message); i++)
		if(!isprint(message[i]))
			message[i]='*';

}

int main (){

	// Creates a UDP socket

	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		printf("Error %d: creation of UDP socket failed\n", errno);
		exit(1);
	}

	// Binds to the port

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(5010);

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr))){
		printf("Error %d: binding of UDP socket failed\n", errno);
		close(fd);
		exit(1);
	}

	// Joins the multicast group

	struct ip_mreq imr;
	inet_pton(AF_INET, "224.0.0.22", &(imr.imr_multiaddr.s_addr));
	imr.imr_interface.s_addr = INADDR_ANY;

	if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr)) < 0){
		printf("Error %d: joining multicast group failed\n", errno);
		close(fd);
		exit(1);
	}	

	// Reads the message

	char buffer[1000];
	int buflen = sizeof(buffer);
	socklen_t alen = sizeof(addr);
	time_t currtime;

	while(1){
		int rlen = recvfrom(fd, buffer, buflen, 0, (struct sockaddr*)&addr, &alen);
		if (rlen < 0)
			printf("Error %d: reading message failed\n", errno);

		else{
			char *tok, *id, *msg, *datetime;
			tok = strtok(buffer, " "); 
			if(strncmp(tok, "FROM", 4) != 0)
				printf("Error: badly formatted message; you need to include 'FROM'\n");
			
			tok = strtok(NULL, "\n");
			printf(tok);
			checkForIllegalChar(tok);
			id = strdup(tok);

			tok = strtok(NULL, "\n");
			checkForIllegalChar(tok);

			msg = strdup(tok);
			currtime = time(NULL);
			datetime = strdup(ctime(&currtime));
			if(strftime(datetime, 20, "%d-%m-%Y %H:%M:%S",localtime(&currtime)))
				printf("%s - %s - %s %c", datetime, id, msg, '\n');			
			else
				printf("Error: total number of resulting bytes is more than maxsize\n");
		
		}

	}

	close(fd);
	return 0;

}
	

