#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "handler.h"
#include "parser.h"

#define BACKLOG 3

static const int domain = AF_INET;

static struct sockaddr_in address;

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

        char request = parseRequest(&connection);

        handleRequest((char *)&request, &connection);

        close(connection);
    }
}