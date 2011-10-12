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

#define TCP_PORT "3570"
#define UDP_PORT "3670"

#define BACKLOG 10  
#define MAXSIZE 1024

typedef struct car CAR;

struct car {
    char *model;
    int branches[3];
    CAR *next;
};

// Pointer into linked list
CAR *list_head = NULL;
CAR *list_tail = NULL;

void printList() {
	CAR *tmpcar;
	
	tmpcar = list_head;
	
	while (tmpcar) { 
		printf("NAME: %s\n", tmpcar->model);
		printf("BRANCH1: %d\n", tmpcar->branches[0]);
		printf("BRANCH2: %d\n", tmpcar->branches[1]);
		printf("BRANCH3: %d\n", tmpcar->branches[2]);
		printf("----------\n\n");
		tmpcar = tmpcar->next;
	}
	
}

char *lookupCar(char *el) {
	CAR *tmpcar;
	int j;
	int index;
	char *branch;
	
	tmpcar = list_head;
	index = 0;
	
	while (tmpcar) { 
		if (strcmp(el, tmpcar->model) == 0) {
			for (j=0; j<3; j++) {
				if (tmpcar->branches[j] == 1) {
					index = j+1;
					break;
				}
			}
		}
		tmpcar = tmpcar->next;
	}
	
	if (index == 1) {
		branch = "1";
	} else if (index == 2) {
		branch = "2";
	} else if (index == 3) {
		branch = "3";
	} else {
		branch = "0";
	}
	
	return branch;
}

void storeCar(char *el, int branch) {	
	// Store data in "database" (linked list of car structs)
	CAR *tmp_ptr = list_head;
	
	while (tmp_ptr) { 
		if (strcmp(tmp_ptr->model, el) == 0) { 
			// Car model found in database, update branch info
			break;
		}  else {
			tmp_ptr = tmp_ptr->next; 
		}
	}
	 
	if (!tmp_ptr) {
		// New car model, not in database. Add new entry
		// Allocate memory for new entry
		CAR *new_car = malloc(sizeof(CAR)); 
	 
		// Initialize the struct values
		if (new_car) {
			new_car->model = el;
			new_car->branches[0] = 0;
			new_car->branches[1] = 0;
			new_car->branches[2] = 0;
			new_car->branches[branch-1] = 1;
			new_car->next = NULL;
		}
	
		// Update head and tail pointers of linked list
		if (!list_head) {
			list_head = new_car;
			list_tail = new_car;
		} else {
			list_tail->next = new_car;
			list_tail = new_car;
		}
	} else {
		tmp_ptr->branches[branch-1] = 1;
	}
}

void *sigchld_handler(int n)
{
	wait3(NULL,WNOHANG,NULL);
}

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
	
	pid_t childpid;
	
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
	
	if ((rv = getaddrinfo(NULL, TCP_PORT, &hints, &servinfo)) != 0) {
		//fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
		exit(1);
	}
	

	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
		//perror("server: socket");
		exit(1);
	}
	
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		//perror("setsockopt");
		exit(1);
	}
	
	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		//perror("server: bind");
		exit(1);
	}
	
	struct hostent *hostip;
	char host[1024];
	gethostname(host, 1023);
	hostip = gethostbyname(host);
	printf("The central database has TCP port %s and IP address %s\n", TCP_PORT, inet_ntoa(*(struct in_addr *)hostip->h_addr_list[0]));

	freeaddrinfo(servinfo); // free the linked-list
	
    // Listen for incoming connections
    if (listen(sockfd, BACKLOG) == -1) {
        // fprintf(stderr, "ERROR: listen(): Can't listen on socket.");
        exit(1);
    }  
	
	// Reap all dead processes
	// Note: Beej's guide showed how to do this, but not for Solaris
	// Found a reference online on how to handle zombie processes (http://www.linuxhowtos.org/C_C++/socket.htm)
	signal(SIGCHLD, sigchld_handler);
   
    // Accept incoming connections
    //while(1) {		
	int num_connections;
	
	for (num_connections = 0; num_connections < 3; num_connections++) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        	
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
        
		childpid = fork();
		
        // Fork child process
        if (childpid == 0) {
			
			int bytes_received;
			int j;
			char *buf = malloc(MAXSIZE);
			char *car;
			char *branch;
			
			
            // Child process doesn't need the listener, so close it.
            close(sockfd);
            
            // Receive car data
            bytes_received = recv(new_fd, buf, MAXSIZE, 0);
            
            if (bytes_received == -1) {
                //fprintf(stderr, "ERROR: recv(): Could not receive data");
                exit(1);
            }
						            
            // Close socket
            close(new_fd);
			
			// Store cars in linked list
			branch = strtok(buf, "\n");	

			for (j = 0; j < 5; j++) {
				car = strtok(NULL, "\n");
				storeCar(car, atoi(branch));
			}
      
			char branchname[9];
			strcpy(branchname, "<Branch");
			strcat(branchname, branch);
			strcat(branchname, ">");
			
			printf("Received the car list from %s\n", branchname);
	
            exit(0);
        }
   
		// Close child socket in parent
		close(new_fd);
    }
	
	int status1, status2, status3;
	wait(&status1);
	wait(&status2);
	wait(&status3);
	
	printf("End of Phase 1 for the database\n");

	
	// Begin phase 2, UDP
	
	int udpsockfd;
	struct addrinfo udphints, *udpservinfo;
	struct sockaddr_storage user_their_addr; // connector's address information
	
	memset(&udphints, 0, sizeof udphints);
    udphints.ai_family = AF_INET; // set to AF_INET to force IPv4
    udphints.ai_socktype = SOCK_DGRAM;
    udphints.ai_flags = AI_PASSIVE; // use my IP
	
	printf("The central database has UDP port %s and IP address %s\n", UDP_PORT, inet_ntoa(*(struct in_addr *)hostip->h_addr_list[0]));
	
    if ((rv = getaddrinfo(NULL, UDP_PORT, &udphints, &udpservinfo)) != 0) {
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
		
	if ((udpsockfd = socket(udpservinfo->ai_family, udpservinfo->ai_socktype, udpservinfo->ai_protocol)) == -1) {
		//perror("listener: socket");
		exit(1);
	}
		
	if (bind(udpsockfd, udpservinfo->ai_addr, udpservinfo->ai_addrlen) == -1) {
		close(udpsockfd);
		//perror("listener: bind");
		exit(1);
	}
	
		
	freeaddrinfo(udpservinfo);
	
	for (num_connections = 0; num_connections < 2; num_connections++) {
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
        
		childpid = fork();
		
        // Fork child process
        if (childpid == 0) {
			int numbytes;
			socklen_t addr_len;
			char udp_buf[MAXSIZE];
			char *index;
			char *user;
			char *car;
			char carname_brackets[1024];
			
			memset(&carname_brackets, 0, sizeof carname_brackets);
			memset(&udp_buf, 0, sizeof udp_buf);
			
			addr_len = sizeof user_their_addr;
								
			// Receive and confirm cars for User1
			int x;
			for(x=0; x<3; x++) {
				if ((numbytes = recvfrom(udpsockfd, udp_buf, MAXSIZE , 0,(struct sockaddr *)&user_their_addr, &addr_len)) == -1) {
					// perror("recvfrom");
					exit(1);
				}
				
				user = strtok(udp_buf, "#");
				car = strtok(NULL, "");

				// Create carname with brackets
				strcat(carname_brackets, "<");
				strcat(carname_brackets, car);
				strcat(carname_brackets, ">");
				
				index = lookupCar(udp_buf);
				sendto(udpsockfd, index, strlen(index), 0, (struct sockaddr *)&user_their_addr, addr_len);
				printf("Sent branch info about %s to %s\n", carname_brackets, user);
				
				memset(&carname_brackets, 0, sizeof carname_brackets);
				memset(&udp_buf, 0, sizeof udp_buf);

			}
		
			exit(0);
		}		
	}
	
	int udpstatus1, udpstatus2;
	wait(&udpstatus1);
	wait(&udpstatus2);
	
	printf("End of Phase 2 for the database\n");
	
	// Close Sockeet
	close(udpsockfd);
	
	return 0;
}
