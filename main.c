// Kevin Sullivan

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <libgen.h>

#define BUFFER_SIZE 1024

// Struct to maintain socket information
typedef struct {
	int newSock;
} sockStruct;

// Handle all communication between the client and server
void* handleComm(void* arg)
{
	int received, sent;
	char *response = NULL;
	char buffer[BUFFER_SIZE];
	long int threadID = (long int)pthread_self();
	sockStruct *data = arg;

	// Endless loop for servet/client communication
	while (1)
	{
		// Block until something is received from the client
		received = recv(data->newSock, buffer, BUFFER_SIZE - 1, 0);

		// Check for receiving error
		if (received < 0)
		{
			printf("Error: Unable to parse received data\n");
			continue;
		}

		// See if the client closed its socket
		else if (received == 0)
		{
			printf("[thead %ld] Client closed its socket....terminating\n");
			break;
		}

		// Receive the data if all is well
		else
		{
			// Print receipt message
			printf("[thead %ld] Rcvd: %s", threadID, buffer);

			// Check for ADD command
			if (strncmp(buffer, "ADD", 3) == 0)
			{
				printf("IT WORKS\n");
			}

			// Check for DELETE command
			else if (strncmp(buffer, "DELETE", 6) == 0)
			{

			}

			// Check for the READ command
			else if (strncmp(buffer, "READ", 4) == 0)
			{

			}

			// Otherwise, this is an ERROR
			else
			{
				sent = send(newSock, response, strlen(response), 0);
			}








			// sent = send(newSock, response, strlen(response), 0);




			// n = send( newsock, msg, strlen( msg ), 0 );
   //    if ( n < strlen( msg ) ) {
   //      perror( "Write()" );
   //    }
		}
	}

	close(data->newSock);
	
	return NULL;
}


int main(int argc, char* argv[])
{
	// Local vars
	int sock, newSock, portNumber, len, rc;
	unsigned int fromLen;
	pthread_t tid;

	// Make sure one argument is included
	if (argc != 2)
	{
		printf("Error: Too many arguments\nUsage: %s portNumber", argv[0]);
		return 1;
	}

	// Convert argument to integer
	portNumber = atoi(argv[1]);

	// Make sure the argument is an integer
	if (portNumber == 0)
	{
		printf("Error: Port argument must be an integer\nUsage: %s portNumber", argv[0]);
		return 1;
	}

	// Make sure the port is between 8000-9000
	if (portNumber < 8000 || portNumber > 9000)
	{
		printf("Error: Port must be in range 8000-9000\nUsage: %s portNumber", argv[0]);
		return 1;
	}

	// Set up socket structures
	struct sockaddr_in server;
	struct sockaddr_in client;

	// Create a TCP listener socket
	sock = socket(PF_INET, SOCK_STREAM, 0); // Socket Descriptor

	// Check for socket creation failure
	if (sock < 0)
	{
		printf("Error: Could not create socket\n");
		return 1;
	}

	server.sin_family = PF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portNumber);

	len = sizeof(server);

	// Check for binding failure
	if (bind(sock, (struct sockaddr*)&server, len) < 0)
	{
		printf("Error: Could not bind to socket\n");
		return 1;
	}

	// Start listening for clients
	fromLen = sizeof(client);
	listen(sock, 10);

	// Initialization message
	printf("Started file-server\nListening on port %d\n", portNumber);

	// Server listen loop
	while (true)
	{
		// Block main thread while waiting for next client
		newSock = accept(sock, (struct sockaddr*)&client, &fromLen);

		// Notify when a client connection comes in
		printf("Recieved incoming connection from %s\n", inet_ntoa(client.sin_addr));

		// Create a struct to pass information to the thread
		sockStruct *data = (sockStruct*)malloc(sizeof *data);
		data->newSock = newSock;

		// Spawn thead for client communication
		rc = pthread_create(&tid, NULL, handleComm, data);
	}

	return 0;
}