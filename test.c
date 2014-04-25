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

int main()
{
	FILE *addFile = fopen("/Users/sideslapd/Documents/Dropbox/School/Operating Systems/Homework/Homework 4/File Server/.storage/test.txt", "rb");

	// Return an error if the file already exists
	if (addFile)
	{
		printf("exists!\n");
	}

	return 0;
}