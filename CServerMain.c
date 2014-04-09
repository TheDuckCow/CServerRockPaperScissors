/*
EC440 HW6
Patrick Crawford 7 Connor McEwen.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_LINE 20

int clientCount;	// counts the number of clients so far
int clientWins;		// number of times client "won"
int serverWins;		// number of times server "won"

int runServer;		// whether to continue running the server
int sockfd;			// the primary, main server port
int newsockfd;		// the child or newly connected thing
int globalPpid;		// the global PID, needs to be global for signal handling...
char buffer[256];	// idk, trying to make signal handling work..
struct sockaddr_in serv_addr, cli_addr;


// after fork, the actual handler for the client to be cleaner
void clientHandle(int newsockfd){
	
	
	// valid choices for either client or server
	const char *gameChoices[] = {"ROCK","PAPER","SCISSORS"};
	// write initial message
	char tmp[50]={0x0};
	sprintf(tmp,"Enter one of {ROCK,PAPER,SCISSORS}: ");
	n = write(newsockfd,tmp,sizeof(tmp));
	
	// to read from the client....
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	
	
	// handle the input
    if (userIn[0] !='\0'){
    	
    }
    else{
    	// skip? / general handling for not valid input
    }
    
    // after finished everything..
    close(newsockfd);
	
}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


// signal handling, to properly close the port connection
void sigintHandler(int sig_num)
{
    //fprintf(stderr,"Handling signal break\n");
    signal(SIGINT, sigintHandler);
    
    // technically should make this a try statement...
    close(sockfd);
    close(newsockfd);
    
    // kill ALL children processes, which aren't the parent.
    if (getpid()!=globalPpid){
    	exit(0);
    }
    
    
}

int main(int argc, char *argv[])
{
	// set the global parent ID:
	globalPpid = getpid();
	
	// signal handling
	signal(SIGINT, sigintHandler);
	
	// game choices
	
	// and the rest of the things...
	int portno;
	socklen_t clilen;
	clientCount = 0;
	clientWins = 0;
	serverWins = 0;
	runServer = 1;
	int n;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	
	
	//setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	///  HERE MIGHT NEED TO UNLINK THE SOCKET for it to work..
	//unlink(sockfd);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	
	printf("\nStarting server...\n");
	while (runServer){
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
	        (struct sockaddr *) &cli_addr, 
	        &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		
		
		// ########### SHOULD FORK HERE ############
		// run clientHandle(newsockfd);
		
		
		// only now has the client been confirmed
		clientCount++;
		
		//write on the server side that we connected to a client, (before forking?)
		printf("Servicing client %i",clientCount);
		fflush(stdout);
		
		
		
		
		// printing stuff to our client
		char tmp[50]={0x0};
		sprintf(tmp,"Client connected, servicing %11d\n", clientCount);
		n = write(newsockfd,tmp,sizeof(tmp));
		
		
		
		if (n < 0) error("ERROR writing to socket");
		close(newsockfd);
		
		// test, see if ends properly such that we can restart server with same port in temrinal
		if (clientCount == 3){
			//runServer=0;
		}
	}
	
	close(sockfd);
	return 0; 
}