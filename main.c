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

// Handle all communication between the client and server
void* handleComm(void* arg)
{
	int received;

	// Endless loop for servet/client communication
	while (1)
	{
		received = recv(newsock, buffer, BUFFER_SIZE - 1, 0);
	}







     while ( 1 )
     {
      /* can also use read() and write() */
      n = recv( newsock, buffer, BUFFER_SIZE - 1, 0 ); // BLOCK
      if ( n < 0 )
      {
        perror( "recv()" );
        break;
      }
      else if ( n == 0 )
      {
        printf( "Client closed its socket\n" );
        break;
      }
      else {
        buffer[n] = '\0';
        printf( "Received message from %s: %s\n",
                inet_ntoa( (struct in_addr)client.sin_addr ),
                buffer );
      }
      n = send( newsock, msg, strlen( msg ), 0 );
      if ( n < strlen( msg ) ) {
        perror( "Write()" );
      }
     }

      close( newsock );
      exit( 0 );
      /* child process becomes a zombie... */









	printf("Thread %ld\n", (long int)pthread_self());
	wait(10000);

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

		// Spawn thead for client communication
		rc = pthread_create(&tid, NULL, handleComm, 0);
	}




//   while ( 1 )
//   {
//     printf( "Blocked on accept()\n" );
//     newsock = accept( sock, (struct sockaddr *)&client, &fromlen );
//     /* newsock is another file (socket) descriptor */


//     printf( "Accepted client connection\n" );

//     printf( "Client is %s\n", inet_ntoa( client.sin_addr ) );


//     /* handle socket in child process */
//     pid = fork();

//     if ( pid == 0 )   /* CHILD */
//     {
//      while ( 1 )
//      {
//       /* can also use read() and write() */
//       n = recv( newsock, buffer, BUFFER_SIZE - 1, 0 ); // BLOCK
//       if ( n < 0 )
//       {
//         perror( "recv()" );
//         break;
//       }
//       else if ( n == 0 )
//       {
//         printf( "Client closed its socket\n" );
//         break;
//       }
//       else {
//         buffer[n] = '\0';
//         printf( "Received message from %s: %s\n",
//                 inet_ntoa( (struct in_addr)client.sin_addr ),
//                 buffer );
//       }
//       n = send( newsock, msg, strlen( msg ), 0 );
//       if ( n < strlen( msg ) ) {
//         perror( "Write()" );
//       }
//      }

//       close( newsock );
//       exit( 0 );
//       /* child process becomes a zombie... */
//     }

//     /* parent simply closes the socket */
//     close( newsock );
//   }

// close( newsock );
// exit( 0 );
// /* child process becomes a zombie... */
// }

// /* parent simply closes the socket */
// close( newsock );
// }













	// long maxPath;
	// int i;
	// int index = 0;
	// char* backupPaths[99999];
	// unsigned long long totalSize = 0;
	// int totalFiles = 0;
	// int totalDirectories = 0;
	// bool restoreFlag = false;
	// bool unknownFlag = false;

	// // Determine the max pathname allowed on this PC
	// if ((maxPath = pathconf("/", _PC_PATH_MAX)) == -1)
	// 	maxPath = 4096;

	// // Increase space for string delimeter
	// maxPath++;

	// char cwd[maxPath];

	// if (getcwd(cwd, sizeof(cwd)) == NULL)
	// 	printf("Error obtaining working directory!");
		

	// for (i = 0; i < 99999; i++)
	// 	backupPaths[i] = malloc((maxPath + 1) * sizeof(char));

	// // Check for init error
	// if (backupPaths[i-1] == NULL)
	// 	return 1;

	// // Parse command line arguments
	// for (i = 1; i < argc; i++) 
	// {
	// 	if (strcmp(argv[i], "-r") == 0) 
	// 		restoreFlag = true; 
	// 	else  
	// 		unknownFlag = true;
	// }

	// // Show a warning if unknown command line arguments were provided
	// if (unknownFlag) 
	// {
	// 	printf("An unknown command line argument has been provided and was\n");
	// 	printf("ignored. Only -r for restoring a backup is supported\n");
	// }		

	// // Make the backup directory if it does not exist
	// mkdir(".mybackup", S_IRWXU);

	// if (restoreFlag)
	// {
	// 	// Get all paths to restore
	// 	int final = getPaths("./.mybackup", maxPath, backupPaths, index);
	// 	backupPaths[final] = NULL;

	// 	// Spawn threads for each restore function
	// 	int i, rc;
	// 	totalSize = 0;
	// 	totalFiles = 0;
	// 	totalDirectories = 0;
	// 	pthread_t tid[final - 1];

	// 	for ( i = 0 ; i < final - 1 ; i++ )
	// 	{
	// 		if (backupPaths[i] == NULL)
	// 			break;

	// 		struct stat s;
	// 		stat(backupPaths[i], &s);

	// 		backupStruct *args = (backupStruct*)malloc(sizeof *args);
	// 		args->pathname = malloc(maxPath * sizeof(char));
	// 		strcpy(args->pathname, backupPaths[i]);
	// 		args->maxPath = maxPath;

	// 		if(S_ISDIR(s.st_mode)) 
	// 		{
 //    			// Create the directory if needed
 //       			rc = pthread_create(&tid[i], NULL, restoreBackupDir, args);
 //       			totalDirectories++;
	// 			pthread_join(tid[i], NULL);
	// 		} else {
	// 		    if(S_ISREG(s.st_mode)) {
	// 		    	// Skip the executable name
	// 		    	if (strcmp(basename(args->pathname), "mybackup") == 0)
	// 		    		continue;

	// 		        // Directory exists
	// 				FILE* source = fopen(args->pathname, "r");

	// 				// Get the filesize
	// 				fseek(source, 0L, SEEK_END);
	// 				int size = ftell(source);
	// 				totalSize += size;
	// 				totalFiles++;
	// 				args->size = size;

	// 				fclose(source);

	// 				// Spawn the restoration thread
	// 				rc = pthread_create(&tid[i], NULL, restore, args);
	// 		    } 
	// 		}

	// 		if (rc != 0) 
	// 			perror("Could not create thread");
	// 	}

	// 	for ( i = 0 ; i < final - 1 ; i++ )
	// 		pthread_join(tid[i], NULL);

	// 	printf("Successfully restored %d files (%llu bytes) in %d subdirectories\n", totalFiles, totalSize, totalDirectories);
	// }
	// else
	// {
	// 	// Get all paths to back up
	// 	int final = getPaths(".", maxPath, backupPaths, index);
	// 	backupPaths[final] = NULL;

	// 	// Spawn threads for each backup function
	// 	int i, rc;
	// 	totalSize = 0;
	// 	totalFiles = 0;
	// 	totalDirectories = 0;
	// 	pthread_t tid[final - 1];

	// 	for ( i = 0 ; i < final - 1 ; i++ )
	// 	{
	// 		if (backupPaths[i] == NULL)
	// 			break;

	// 		struct stat s;
	// 		stat(backupPaths[i], &s);

	// 		backupStruct *args = (backupStruct*)malloc(sizeof *args);
	// 		args->pathname = malloc(maxPath * sizeof(char));
	// 		strcpy(args->pathname, backupPaths[i]);
	// 		args->maxPath = maxPath;

	// 		if(S_ISDIR(s.st_mode)) 
	// 		{
 //    			// Create the directory if needed
 //       			rc = pthread_create(&tid[i], NULL, makeBackupDir, args);
 //       			totalDirectories++;
 //       			pthread_join(tid[i], NULL);
	// 		} else {
	// 		    if(S_ISREG(s.st_mode)) {
	// 				// Skip the executable name
	// 		    	if (strcmp(basename(args->pathname), "mybackup") == 0)
	// 		    		continue;

	// 		        // Directory exists
	// 				FILE* source = fopen(args->pathname, "r");

	// 				// Get the filesize
	// 				fseek(source, 0L, SEEK_END);
	// 				int size = ftell(source);
	// 				totalFiles++;
	// 				args->size = size;

	// 				fclose(source);

	// 				// Spawn the backup thread
	// 				rc = pthread_create(&tid[i], NULL, backup, args);
	// 		    } 
	// 		}

	// 		if (rc != 0) 
	// 			perror("Could not create thread");\
	// 	}

	// 	// Join the threads and count the total bytes
	// 	for ( i = 0 ; i < final - 1 ; i++ )
	// 	{
	// 		void* status;

	// 		pthread_join(tid[i], &status);

	// 		if ((intptr_t)status > 0)
	// 			totalSize += (intptr_t)status;
	// 	}

	// 	printf("Successfully backed up %d files (%llu bytes) in %d subdirectories\n", totalFiles, totalSize, totalDirectories);
	// }

	return 0;
}