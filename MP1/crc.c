#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include <iostream>
#include <bits/stdc++.h> 
#include <errno.h>

#define BUFFER_LENGTH    250

using std::endl;
using std::cout;

int connect_to(const char *host, const int port);
struct Reply process_command(const int sockfd, char* command);
void process_chatmode(const char* host, const int port);

int main(int argc, char** argv) 
{
	if (argc != 3) {
		fprintf(stderr,
				"usage: enter host address and port number\n");
		exit(1);
	}

    display_title();
    
	while (1) {
	
		int sockfd = connect_to(argv[1], atoi(argv[2]));
    
		char command[MAX_DATA];
        get_command(command, MAX_DATA);

		struct Reply reply = process_command(sockfd, command);
		display_reply(command, reply);
		
		touppercase(command, strlen(command) - 1);
		if (strncmp(command, "JOIN", 4) == 0) {
			printf("Now you are in the chatmode\n");
			process_chatmode(argv[1], reply.port);
		}
	
		close(sockfd);
    }

    return 0;
}

/*
 * Connect to the server using given host and port information
 *
 * @parameter host    host address given by command line argument
 * @parameter port    port given by command line argument
 * 
 * @return socket fildescriptor
 */
int connect_to(const char *host, const int port)
{
	// ------------------------------------------------------------
	// GUIDE :
	// In this function, you are suppose to connect to the server.
	// After connection is established, you are ready to send or
	// receive the message to/from the server.
	// 
	// Finally, you should return the socket fildescriptor
	// so that other functions such as "process_command" can use it
	// ------------------------------------------------------------

    // below is just dummy code for compilation, remove it.
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("socket error\n"); exit(-1);
	}

	//here we populate server conn. data
	struct sockaddr_in serveraddr;

	//here we prepare to get host
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family      = AF_INET;
    serveraddr.sin_port        = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(server);

	//here we get host name
	if(serveraddr).sin_addr.s_addr == (unsigned long)INADDR_NONE) {
		hostp = gethostbyname(server);
		if(hostp == (struct hostent *)NULL){
			printf("there is a error with host\n"); 
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));
	}

	//here we establish a connection to the server
	int err = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(err<0) {
		printf("Error in connect_to func.\n");
		exit(-1);
	}

	return sockfd;
}

/* 
 * Send an input command to the server and return the result
 *
 * @parameter sockfd   socket file descriptor to commnunicate
 *                     with the server
 * @parameter command  command will be sent to the server
 *
 * @return    Reply    
 */



struct Reply process_command(const int sockfd, char* command)
{
	// ------------------------------------------------------------
	// GUIDE 1:
	// In this function, you are supposed to parse a given command
	// and create your own message in order to communicate with
	// the server. Surely, you can use the input command without
	// any changes if your server understand it. The given command
    // will be one of the followings:
	//
	// CREATE <name>
	// DELETE <name>
	// JOIN <name>
    // LIST
	//
	// -  "<name>" is a chatroom name that you want to create, delete,
	// or join.
	// 
	// - CREATE/DELETE/JOIN and "<name>" are separated by one space.
	// ------------------------------------------------------------


	// ------------------------------------------------------------
	// GUIDE 2:
	// After you create the message, you need to send it to the
	// server and receive a result from the server.
	// ------------------------------------------------------------


	// ------------------------------------------------------------
	// GUIDE 3:
	// Then, you should create a variable of Reply structure
	// provided by the interface and initialize it according to
	// the result.
	//
	// For example, if a given command is "JOIN room1"
	// and the server successfully created the chatroom,
	// the server will reply a message including information about
	// success/failure, the number of members and port number.
	// By using this information, you should set the Reply variable.
	// the variable will be set as following:
	//
	// Reply reply;
	// reply.status = SUCCESS;
	// reply.num_member = number;
	// reply.port = port;
	// 
	// "number" and "port" variables are just an integer variable
	// and can be initialized using the message fomr the server.
	//
	// For another example, if a given command is "CREATE room1"
	// and the server failed to create the chatroom becuase it
	// already exists, the Reply varible will be set as following:
	//
	// Reply reply;
	// reply.status = FAILURE_ALREADY_EXISTS;
    // 
    // For the "LIST" command,
    // You are suppose to copy the list of chatroom to the list_room
    // variable. Each room name should be seperated by comma ','.
    // For example, if given command is "LIST", the Reply variable
    // will be set as following.
    //
    // Reply reply;
    // reply.status = SUCCESS;
    // strcpy(reply.list_room, list);
    // 
    // "list" is a string that contains a list of chat rooms such 
    // as "r1,r2,r3,"
	// ------------------------------------------------------------

	// REMOVE below code and write your own Reply.
	//command info for the message
	char buffer[BUFFER_LENGTH];
	strcpy(buffer, command);

	//the send tp server and recieve result part
	int rc = send(sockfd, buffer, sizeof(buffer), 0);
	//testing the error
	if(rc < 0){
		printf("error in send\n");
		exit(-1);
	}

	//creating the reply struct
	struct Reply reply;

	// creating create, join, list, and delete below
	// in each of these ifs we willb have a get and set status part that is common to all

	//create
	if(strncmp(buffer, "CREATE ", 7) == 0){
		char charStatus[BUFFER_LENGTH];
        rc = recv(sockfd, charStatus, sizeof(charStatus), 0);
        if(rc < 0){
            printf("Failed to recieve in the socket");
			exit(-1);
        }
		//Get and set status part mentioned above
		string status(charStatus);
		if(status == "FAILURE_ALREADY_EXISTS"){
			reply.status = FAILURE_ALREADY_EXISTS;
		}
		else if(status == "SUCCESS"){
			reply.status = SUCCESS;
		}
		else if(status == "FAILURE_INVALID"){
			reply.status = FAILURE_INVALID;
		}
		else {
			reply.status = FAILURE_UNKNOWN;
		}
	}

	//join
	else if(strncmp(buffer, "JOIN ", 5) == 0) {
		char charStatus[BUFFER_LENGTH];
		rc = recv(sockfd, charStatus, sizeof(charStatus), 0);
		string replyInfo(charStatus);
		vector <std::string> tokens; 
		stringstream check1(replyInfo); 
		string intermediate;
		while(getline(check1, intermediate, ',')){ 
			tokens.push_back(intermediate); 
		}
		
		//Get and set status as mentioned above
		string status(tokens.front());
		if(status == "FAILURE_NOT_EXISTS"){
			reply.status = FAILURE_ALREADY_EXISTS;
			reply.port = -1;
			reply.num_member = -1;
		}
		else if(status == "SUCCESS"){
			reply.status = SUCCESS;
			reply.port = std::stoi(tokens.at(1));
			reply.num_member = std::stoi(tokens.at(2));
		}
		else{
			reply.status = FAILURE_UNKNOWN;
			reply.port = -1;
			reply.num_member = -1;
		}
		
    }

	//list
	else if(strncmp(buffer, "LIST", 4) == 0){
		char list[BUFFER_LENGTH];
        rc = recv(sockfd, list, sizeof(list), 0);
        if(rc < 0){
            printf("Failed to recieve in the socket");
			exit(-1);
        }
		reply.status = SUCCESS;
		strcpy(reply.list_room, list);
    }

	//delete
	else if(strncmp(buffer, "DELETE ", 7) == 0){
		char charStatus[BUFFER_LENGTH];
        rc = recv(sockfd, charStatus, sizeof(charStatus), 0);
        if(rc < 0){
            printf("Failed to recieve in the socket");
			exit(-1);
        }
		//Get and set status as mentioned above
		string status(charStatus);
		if(status == "FAILURE_NOT_EXISTS"){
			reply.status = FAILURE_NOT_EXISTS;
		}
		else if(status == "SUCCESS"){
			reply.status = SUCCESS;
		}
		else{
			reply.status = FAILURE_UNKNOWN;
		}
    }

	close(sockfd);
	return reply;
}

/* 
 * Get into the chat mode
 * 
 * @parameter host     host address
 * @parameter port     port
 */

void* process_recv(void* arg){
    int sd;
    sd = *((int *)arg);
    char rbuffer[BUFFER_LENGTH];
    while(1){
	  ssize_t rcn = recv(sd, rbuffer,  BUFFER_LENGTH, 0); 
	  	if(rcn<=0){
		  	printf("Recieve error chatroom\n");
		  	break;
		}
    	else{
        	rbuffer[rcn]='\0';
        	if(rbuffer[0]>0){
            	//printf("receive %s %i from %i %i\n", rbuffer,rcn, sd, rbuffer[0]);
            	display_message(rbuffer);
	  		}
	   }
    }
}

void process_chatmode(const char* host, const int port)
{
	// ------------------------------------------------------------
	// GUIDE 1:
	// In order to join the chatroom, you are supposed to connect
	// to the server using host and port.
	// You may re-use the function "connect_to".
	// ------------------------------------------------------------

	// ------------------------------------------------------------
	// GUIDE 2:
	// Once the client have been connected to the server, we need
	// to get a message from the user and send it to server.
	// At the same time, the client should wait for a message from
	// the server.
	// ------------------------------------------------------------
	
    // ------------------------------------------------------------
    // IMPORTANT NOTICE:
    // 1. To get a message from a user, you should use a function
    // "void get_message(char*, int);" in the interface.h file
    // 
    // 2. To print the messages from other members, you should use
    // the function "void display_message(char*)" in the interface.h
    //
    // 3. Once a user entered to one of chatrooms, there is no way
    //    to command mode where the user  enter other commands
    //    such as CREATE,DELETE,LIST.
    //    Don't have to worry about this situation, and you can 
    //    terminate the client program by pressing CTRL-C (SIGINT)
	// ------------------------------------------------------------
    int sd = connect_to(host, port);
    pthread_t rworker;
    pthread_create(&rworker, NULL,process_recv, &sd );
	
    //loop untill the end of time
    while(1){
        //message from user recieved
        char sbuffer[BUFFER_LENGTH];
        char rbuffer[BUFFER_LENGTH];
        get_message(sbuffer,BUFFER_LENGTH);
		
        int rc = send(sd, sbuffer, sizeof(sbuffer), 0);
        if(rc<0){
			printf("Send error in chat\n"); 
			exit(1);
		}
	}
}

