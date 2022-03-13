#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FOLDER_PATH "FOLDER_PATH"
#define INDEX "index.html"

// TODO: pass parsed request
void handleRequest(char *request, int *connection)
{
    char *folderpath = getenv(FOLDER_PATH);

    char requestPath[100] = {0};

    char response[1024] =
        "HTTP/1.1 200 OK\n"
        "Server: MY_SERVER\n"
        "Content-Type: text/html\n"
        "Connection: Closed\n"
        "\n";

    FILE *fp;

    char buff[256] = {0};

    strcat(requestPath, folderpath);
    strcat(requestPath, INDEX); // TODO: append path from request

    fp = fopen(requestPath, "r"); // TODO: send default error if not found

    while (fgets(buff, sizeof(buff), (FILE *)fp) != NULL) strcat(response, buff);
    
    fclose(fp);

    send(*connection, response, strlen(response), 0);

    printf("Response sent\n------\n%s\n------\n", response);
}