#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

struct clientNode
{
    int socket;
    struct clientNode *next;
};

int main(void)
{
    printf("http-server has started...\n");

    printf("creating a server socket and binding address to it...\n");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // IPv4 with TCP
    const struct sockaddr serverSocketAddress = {AF_INET, htons(80), INADDR_ANY};
    int bindResult = bind(serverSocket, &serverSocketAddress, sizeof(serverSocketAddress));
    if (bindResult == -1)
    {
        printf("bind has failed\n");
        return -1;
    }

    int fcntlResult = fcntl(serverSocket, F_SETFL, O_NONBLOCK); // set non-blocking io for accept()
    if (fcntlResult == -1)
    {
        printf("fcntl for serverSocket has failed\n");
        return -1;
    }

    // mark socket to be accepting incoming connections
    int listenResult = listen(serverSocket, 2137);
    if (listenResult == -1)
    {
        printf("listen has failed\n");
        return -1;
    }

    struct clientNode *clientList = NULL;
    int counter = 0;
    while (1 == 1)
    {
        printf("\ncounter index = %d\n", counter);
        printf("\nattempting to accept a client\n");
        struct sockaddr clientSocketAddress;
        socklen_t clientSocketAddressSize = sizeof(clientSocketAddress);
        int clientSocket = accept(serverSocket, &clientSocketAddress, &clientSocketAddressSize);
        if (clientSocket != -1)
        {
            printf("accepted socket: %d\n", clientSocket);

            int fcntlResult = fcntl(clientSocket, F_SETFL, O_NONBLOCK); // set non-blocking io for recv and send
            if (fcntlResult == -1)
            {
                printf("fcntl for clientSocket has failed\n");
            }
            else if (clientList == NULL)
            {
                printf("clist = NULL\n");
                struct clientNode *node = malloc(sizeof(struct clientNode));
                (*node).socket = clientSocket;
                (*node).next = NULL;
                clientList = node;
            }
            else
            {
                printf("clist = NOT NULL\n");
                struct clientNode *node = malloc(sizeof(struct clientNode));
                (*node).socket = clientSocket;
                (*node).next = clientList;
                clientList = node;
            }
        }

        printf("iterating over all clients...\n");
        struct clientNode *client = clientList;
        while (client != NULL)
        {
            printf("checking client %d...\n", (*client).socket);
            //recieve an https requestq
            char buffer[4096 + 1];
            size_t received = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (received != -1)
            {
                buffer[4096] = 0x00;
                printf("received: \n%s\n", buffer);
            }
            else
            {
                printf("havent received any data\n");
            }
            

            client = (*client).next;

                    // send an http response
            const char *http_response =
                "HTTP/1.1 200 OK\r\n"
                "Date: Mon, 23 May 2005 22:38:34 GMT\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: 155\r\n"
                "Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\r\n"
                "Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
                "ETag: \"3f80f-1b6-3e1cb03b\"\r\n"
                "Accept-Ranges: bytes\r\n"
                "Connection: close\r\n\r\n"
                "<html>\r\n"
                "  <head>\r\n"
                "    <title>An Example Page</title>\r\n"
                "  </head>\r\n"
                "  <body>\r\n"
                "    <p>Hello World, this is a very simple HTML document.</p>\r\n"
                "  </body>\r\n"
                "</html>";
            int sendResult = send(clientSocket, http_response, strlen(http_response), 0);
            if (sendResult == -1)
            {
                printf("havent sent any data\n");
            }
            else
            {
                printf("sent %d bytes to the client %d\n", sendResult, clientSocket);
            }
        }

        sleep(3);
        counter += 1;
    }

    printf("http-server has closed\n");
    return 0;
}
