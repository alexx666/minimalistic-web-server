#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#define BACKLOG 3

// TODO: define as file in FS
static const char *response =
    "HTTP/1.1 200 OK\n"
    "Server: MY_SERVER\n"
    "Content-Type: text/html\n"
    "Connection: Closed\n"
    "\n"
    "Hello madafaka!\n";

static const int domain = AF_INET;

static struct sockaddr_in address;

// TODO: define struct for request
void handleRequest(char *request, int *connection) {
    send(*connection, response, strlen(response), 0);
    printf("Response sent\n------\n%s\n------\n", response);
    close(*connection);
}

// TODO: read connection and return parsed request as struct
char readRequest(int *connection)
{
    char request[1024] = {0};
    read(*connection, &request, sizeof(request));
    printf("Request received\n------\n%s\n------\n", request);
    return *request;
}

int openAndConfigureSocket() {
    int sock, opt_value = 1;

    if ((sock = socket(domain, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value)) < 0)
    {
        perror("Unable to set socket configurations");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void start(int port)
{
    int sock = openAndConfigureSocket();

    address.sin_family = domain;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int addrlen = sizeof(address);

    if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        perror("Unable to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, BACKLOG) < 0)
    {
        perror("Unable to listent to connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port: %d\n", port);

    while (1)
    {
        int connection = accept(sock, (struct sockaddr *) &address, (socklen_t *) &addrlen);

        if (connection < 0)
        {
            perror("Unable to accept connections");
            exit(EXIT_FAILURE);
        }

        char request = readRequest(&connection);

        handleRequest((char *)&request, &connection);
    }
}