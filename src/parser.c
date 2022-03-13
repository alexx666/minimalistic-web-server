#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// TODO: read connection and return parsed request as struct
char parseRequest(int *connection)
{
    char request[10*1024] = {0};
    read(*connection, &request, sizeof(request));
    printf("Request received\n------\n%s\n------\n", request);
    return *request;
}