#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

// TODO: define modules
int main() {
    int socketfd;

    const char *response =
        "HTTP/1.1 200 OK\n"
        "Server: MY_SERVER\n"
        "Content-Type: text/html\n"
        "Connection: Closed\n"
        "\n"
        "Hello madafaka!\n";

    char buffer[1024] = {0};

    int backlog = 3;

    int socktype = SOCK_STREAM;
    int domain = AF_INET;
    int port = 8080;

    int opt_value = 1;
    int opt_level = SOL_SOCKET;
    int opt_name = SO_REUSEADDR;
    int optlen = sizeof(opt_value);

    struct sockaddr_in address;

    address.sin_family = domain;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int addrlen = sizeof(address);

    if ((socketfd = socket(domain, socktype, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socketfd, opt_level, opt_name, &opt_value, optlen) < 0)
    {
        perror("Unable to set socket configurations");
        exit(EXIT_FAILURE);
    }

    if (bind(socketfd, (struct sockaddr *) &address, addrlen) < 0) {
        perror("Unable to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(socketfd, backlog) < 0) {
        perror("Unable to listent to connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port: %d\n", port);

    // accept
    while (1)
    {
        int conn_socket = accept(socketfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    
        if (conn_socket < 0)
        {
            perror("Unable to accept connections");
            exit(EXIT_FAILURE);
        }

        int request = read(conn_socket, &buffer, 1024);

        // TODO: request parsing

        printf("Request received\n------\n%s\n------\n", buffer);

        // TODO: response formatting
        send(conn_socket, response, strlen(response), 0);

        printf("Response sent\n------\n%s\n------\n", response);

        close(conn_socket);
    }

    // TODO: graceful shutdown
    
    return 0;
}