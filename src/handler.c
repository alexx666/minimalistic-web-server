#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

// TODO: define as file in FS
static const char *response =
    "HTTP/1.1 200 OK\n"
    "Server: MY_SERVER\n"
    "Content-Type: text/html\n"
    "Connection: Closed\n"
    "\n"
    "Hello madafaka!\n";

// TODO: define struct for request
void handleRequest(char *request, int *connection)
{
    send(*connection, response, strlen(response), 0);
    printf("Response sent\n------\n%s\n------\n", response);
}