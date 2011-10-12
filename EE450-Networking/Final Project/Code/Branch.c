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

#define PORT "3570"

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int main(int argc, char *argv[])
{	
	
	int sockfd;
	struct addrinfo hints, *servinfo;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	struct sockaddr_in sin;
	socklen_t sin_len;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
    
	
	int j;
	pid_t childpid;

	
	for (j=1; j<=3; j++) {
		 
		childpid = fork();
		
		if (childpid == 0) {
			if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
				//perror("server: socket");
				exit(1);
			}
			
			if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
				close(sockfd);
				//perror("client: connect");
				exit(1);
			}

			
			inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr), s, sizeof s);
			getsockname(sockfd, (struct sockaddr *)&sin, &sin_len);
			
			FILE *fp;
			
			char line[500];
			char *car_name;
			
			char *in_file;
			char *branchnum_as_str;
			char *branchname;
			char carname_brackets[1024];
			
			switch (j) {
				case 1:
					in_file = "branch1.txt";
					branchnum_as_str = "1";
					branchname = "<Branch1>";
					break;
				case 2:
					in_file = "branch2.txt";
					branchnum_as_str = "2";
					branchname = "<Branch2>";
					break;
				case 3:
					in_file = "branch3.txt";
					branchnum_as_str = "3";
					branchname = "<Branch3>";
					break;
				default:
					break;
			}
			
			
			fprintf(stdout, "%s has TCP port %d and IP address %s for Phase 1\n", branchname, (int) ntohs(sin.sin_port), s);

			fprintf(stdout, "%s is now connected to the database\n", branchname);
			
			// Open branch input file
			fp = fopen(in_file, "r");
			
			// Error opening input file
			if (fp == NULL) {
				//fprintf(stdout, "ERROR: fopen(): Can't open %s for reading.\n", in_file);
				exit(1);
			}
			
			// Send Branch number
			send(sockfd, branchnum_as_str, strlen(branchnum_as_str), 0);
			send(sockfd, "\n", strlen("\n"), 0);
						
			// Read input file and send car info to central database
			while(fscanf(fp, "%[^\n]\n", line) != EOF) {
				car_name = strtok(line, "#"); 
				
				send(sockfd, car_name, strlen(car_name), 0);
				send(sockfd, "\n", strlen("\n"), 0); // send a newline to separate entries
				 
				strcat(carname_brackets, "<");
				strcat(carname_brackets, car_name);
				strcat(carname_brackets, ">");
				
				fprintf(stdout, "%s has sent %s to the database\n", branchname, carname_brackets);
				
				memset(&carname_brackets, 0, sizeof carname_brackets);
			}	
			
			fprintf(stdout, "Updating the database is done for %s\n", branchname);
			
			// Close input file
			fclose(fp);
			
			// Close socket
			close(sockfd);
			
			fprintf(stdout, "End of Phase 1 for %s\n", branchname);
			
			// Exit child process
			exit(0);
		}

	}
	
	int status1, status2, status3;
	wait(&status1);
	wait(&status2);
	wait(&status3);
	
	freeaddrinfo(servinfo); // all done with this structure

	return 0;
}
