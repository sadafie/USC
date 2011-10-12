#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT "3670"
#define MAXSIZE 1024

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo;
    int rv;

	struct sockaddr_storage their_addr;
	char *buf = malloc(MAXSIZE);
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	
	struct sockaddr_in sin;
	socklen_t sin_len;
	
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
		
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
	
	int w;
	pid_t childpid;
	
	for (w=1; w<=2; w++) {
		
		childpid = fork();
		
		if (childpid == 0) {
			FILE *fp;
			
			char line[500];
			int user_index = w;
			char *username;
			char *filename;
			char carname_brackets[1024];
			char data[1024];
			
			if (user_index==1) {
				username = "<User1>";
				filename = "user1.txt";
			} else {
				username = "<User2>";
				filename = "user2.txt";
			}
		
			if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
				//perror("talker: socket");
				exit(1);
			}
				
			inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr), s, sizeof s);
			getsockname(sockfd, (struct sockaddr *)&sin, &sin_len);
			
			printf("%s has UDP port %d and IP address %s for Phase 1\n", username, (int) ntohs(sin.sin_port), s);
			
			addr_len = sizeof their_addr;
			
			// Open branch input file
			fp = fopen(filename, "r");

			// Error opening input file
			if (fp == NULL) {
				//fprintf(stdout, "ERROR: fopen(): Can't open %s for reading.\n", in_file);
				exit(1);
			}

			// Read input file and send car info to central database
			while(fscanf(fp, "%[^\n]\n", line) != EOF) {
				// Wrap carname in brackets to print message on screen
				memset(&carname_brackets, 0, sizeof carname_brackets);
				strcat(carname_brackets, "<");
				strcat(carname_brackets, line);
				strcat(carname_brackets, ">");
				
				//Augment data to be sent to include User number
				//<User1>#Toyota Camry
				memset(&data, 0, sizeof data);
				strcat(data, username);
				strcat(data, "#");
				strcat(data, line);
				
				printf("Checking %s in the database\n", carname_brackets);
				
				sendto(sockfd, data, strlen(data), 0, servinfo->ai_addr, servinfo->ai_addrlen);
				
				recvfrom(sockfd, buf, MAXSIZE , 0, (struct sockaddr *)&their_addr, &addr_len);
				
				printf("Received location info of %s from the database\n", carname_brackets);
			}	
			
			printf("Completed car queries to the database from %s.\n", username);
			
			// Close socket
			close(sockfd);
			
			// Close input file
			fclose(fp);
			
			printf("End of Phase 2 for %s\n", username);
			
			exit(0);
		}
		
		//sleep(1);
		
	}
	
	int status1, status2;
	wait(&status1);
	wait(&status2);
	
	freeaddrinfo(servinfo);
		
    return 0;
}

