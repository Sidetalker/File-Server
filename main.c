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

#define BUFFER_SIZE 65536

// Struct to maintain socket information
typedef struct {
	int newSock;
	char *path;
} sockStruct;

// Handle all communication between the client and server
void* handleComm(void* arg)
{
	long int threadID = (long int)pthread_self();
	sockStruct *data = arg;
	char *tokenizer;
	bool inAdd = false;
	int received, sent, i;
	char *bufferData[5];
	char *buffer;
	char *response;
	char *fileLoc;
	char *fileSize;

	// Endless loop for servet/client communication
	while (1)
	{
		buffer = (char *)malloc(BUFFER_SIZE);
		response = (char *)malloc(BUFFER_SIZE);

		// Block until something is received from the client
		received = recv(data->newSock, buffer, BUFFER_SIZE - 1, 0);

		// Remove the incoming newline
		char *newline = strchr(buffer, '\n');

		if (newline != NULL)
			*newline = '\0';

		// Check for receiving error
		if (received < 0)
		{
			printf("Error: Unable to parse received data\n");
			continue;
		}

		// See if the client closed its socket
		else if (received == 0)
		{
			printf("[thread %ld] Client closed its socket....terminating\n", threadID);
			break;
		}

		// Receive the data if all is well
		else
		{
			if (!inAdd)
			{	
				// Print receipt message
				printf("[thread %ld] Rcvd: %s\n", threadID, buffer);

				for (i = 0; i < 5; i++)
					bufferData[i] = malloc(BUFFER_SIZE * sizeof(char));

				i = 0;
				tokenizer = strtok(buffer, " ");

				// Split the input by spaces
				while (tokenizer != NULL && i < 5)
				{
					strncpy(bufferData[i], tokenizer, strlen(tokenizer));
					tokenizer = strtok(NULL, " ");
					i++;
				}
			}

			// See if we're in a current ADD command
			if (inAdd)
			{
				// We've already recieved the file location and size, now let's write
				FILE *fp = fopen(fileLoc, "ab");

				if (fp != NULL)
				{
					fputs(buffer, fp);
					fclose(fp);
				}

				// Get the filesize
				fp = fopen(fileLoc, "r");
				fseek(fp, 0L, SEEK_END);
				int size = ftell(fp);
				fclose(fp);

				// Show user the response
				printf("[thread %ld] Transferred file (%d bytes)\n", threadID, size);

				// Send the ACK
				strcpy(response, "ACK\n");
				sent = send(data->newSock, response, strlen(response), 0);
				printf("[thread %ld] Sent: ACK\n", threadID);

				inAdd = false;
			}

			// Check for starter ADD command
			else if (strncmp(buffer, "ADD", 3) == 0)
			{
				// Flip the flag
				inAdd = true;

				// Return an error if there aren't exactly two arguments
				if (i != 3)
				{
					printf("[thread %ld] Sent: ERROR INVALID ARGUMENT TO ADD\n", threadID);
					strcpy(response, "ERROR INVALID ARGUMENT TO ADD\n");
					sent = send(data->newSock, response, strlen(response), 0);
					inAdd = false;
					continue;
				}

				// Save the file location and size
				fileLoc = (char *)malloc(BUFFER_SIZE);
				fileSize = (char *)malloc(BUFFER_SIZE);
				strcpy(fileLoc, data->path);
				strcat(fileLoc, "/");
				strcat(fileLoc, bufferData[1]);
				strcpy(fileSize, bufferData[2]);

				FILE *addFile = fopen(fileLoc, "rb");

				// Return an error if the file already exists
				if (addFile)
				{
					printf("[thread %ld] Sent: ERROR FILE EXISTS\n", threadID);
					strcpy(response, "ERROR FILE EXISTS\n");
					sent = send(data->newSock, response, strlen(response), 0);
					inAdd = false;
					fclose(addFile);
					continue;
				}
			}

			// Check for DELETE command
			else if (strncmp(buffer, "DELETE", 6) == 0)
			{
				// Return an error if there isn't exactly one argument
				if (i != 2)
				{
					printf("[thread %ld] Sent: ERROR INVALID ARGUMENT TO DELETE\n", threadID);
					strcpy(response, "ERROR INVALID ARGUMENT TO DELETE\n");
					sent = send(data->newSock, response, strlen(response), 0);
					continue;
				}

				// Save the file location
				fileLoc = (char *)malloc(BUFFER_SIZE);
				strcpy(fileLoc, data->path);
				strcat(fileLoc, "/");
				strcat(fileLoc, bufferData[1]);

				// if (strncmp(fileLoc, "/Users/sideslapd/Documents/Dropbox/School/Operating Systems/Homework/Homework 4/File Server/.storage/best.txt", 110) == 0)
				// 	printf("SOAEME\n");
				// else
				// {
				// 	printf("LenHave: %d\nLenWant: 109\n", strlen(fileLoc));
				// 	printf("CMP: %d\n", strncmp(fileLoc, "/Users/sideslapd/Documents/Dropbox/School/Operating Systems/Homework/Homework 4/File Server/.storage/best.txt", 110));
				// 	printf("Want: /Users/sideslapd/Documents/Dropbox/School/Operating Systems/Homework/Homework 4/File Server/.storage/best.txt\n");
				// 	printf("Have: %s\n", fileLoc);
				// }

				FILE *delFile = fopen(fileLoc, "rb");

				// Return an error if the file doesn't exist
				if (!delFile)
				{
					printf("[thread %ld] Sent: ERROR NO SUCH FILE\n", threadID);
					strcpy(response, "ERROR NO SUCH FILE\n");
					sent = send(data->newSock, response, strlen(response), 0);
					continue;
				}
				else
					fclose(delFile);

				// Delete the file
				remove(fileLoc);

				// Send the ACK
				printf("[thread %ld] Delete ""%s"" file\n", threadID, bufferData[1]);
				strcpy(response, "ACK\n");
				sent = send(data->newSock, response, strlen(response), 0);
				printf("[thread %ld] Sent: ACK\n", threadID);
			}

			// Check for the READ command
			else if (strncmp(buffer, "READ", 4) == 0)
			{
				// Return an error if there isn't exactly one argument
				if (i != 2)
				{
					printf("[thread %ld] Sent: ERROR INVALID ARGUMENT TO READ\n", threadID);
					strcpy(response, "ERROR INVALID ARGUMENT TO READ\n");
					sent = send(data->newSock, response, strlen(response), 0);
					continue;
				}

				// Save the file location
				fileLoc = (char *)malloc(BUFFER_SIZE);
				strcpy(fileLoc, data->path);
				strcat(fileLoc, "/");
				strcat(fileLoc, bufferData[1]);

				FILE *readFile = fopen(fileLoc, "rb");

				// Return an error if the file doesn't exist
				if (!readFile)
				{
					printf("[thread %ld] Sent: ERROR NO SUCH FILE\n", threadID);
					strcpy(response, "ERROR NO SUCH FILE\n");
					sent = send(data->newSock, response, strlen(response), 0);
					continue;
				}

				// Get the filesize
				fseek(readFile, 0L, SEEK_END);
				int size = ftell(readFile);
				fseek(readFile, 0L, SEEK_SET);

				// Get the content
				char *content = malloc(size + 1);
				size_t readSize = fread(content, 1, size, readFile);
				content[readSize] = '\0';

				// Send the response
				printf("[thread %ld] Transferred file (%d bytes)\n", threadID, size);
				sprintf(response, "%d\n%s\n", size, content);
				sent = send(data->newSock, response, strlen(response), 0);
				strcpy(response, "ACK\n");
				sent = send(data->newSock, response, strlen(response), 0);
				printf("[thread %ld] Sent: ACK\n", threadID);

			}

			// Otherwise, this is an ERROR
			else
			{
				printf("[thread %ld] Sent: ERROR UNSUPPORTED COMMAND\n", threadID);
				strcpy(response, "ERROR UNSUPPORTED COMMAND\n");
				sent = send(data->newSock, response, strlen(response), 0);
			}
		}

	}

	free(buffer);
	free(response);
	free(fileLoc);
	free(fileSize);

	close(data->newSock);
	
	return NULL;
}


int main(int argc, char* argv[])
{
	// Local vars
	int sock, newSock, portNumber, len, rc;
	long maxPath;
	unsigned int fromLen;
	pthread_t tid;

	// Make sure one argument is included
	if (argc != 2)
	{
		printf("Error: Too many arguments\nUsage: %s portNumber\n", argv[0]);
		return 1;
	}

	// Convert argument to integer
	portNumber = atoi(argv[1]);

	// Make sure the argument is an integer
	if (portNumber == 0)
	{
		printf("Error: Port argument must be an integer\nUsage: %s portNumber\n", argv[0]);
		return 1;
	}

	// Make sure the port is between 8000-9000
	if (portNumber < 8000 || portNumber > 9000)
	{
		printf("Error: Port must be in range 8000-9000\nUsage: %s portNumber\n", argv[0]);
		return 1;
	}

	// Determine the max pathname allowed on this PC
	if ((maxPath = pathconf("/", _PC_PATH_MAX)) == -1)
		maxPath = 4096;

	// Get the current working directory
	char cwd[maxPath];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("Error: Could not obtain working directory\n");
		return 1;
	}

	strcat(cwd, "/.storage");

	// Make our file directory if it doesn't already exist
	mkdir(cwd, S_IRWXU);

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
		data->path = malloc(maxPath * sizeof(char));
		strcpy(data->path, cwd);

		//Spawn thread for client communication
		rc = pthread_create(&tid, NULL, handleComm, data);
	}

	return 0;
}