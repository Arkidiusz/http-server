#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    // mark socket to be accepting incoming connections
    int listenResult = listen(serverSocket, 2137);
    if (listenResult == -1)
    {
        printf("listen has failed\n");
    }

    // accept connections
    while (1==1)
    {
        struct sockaddr clientSocketAddress;
        socklen_t clientSocketAddressSize = sizeof(clientSocketAddress);
        int clientSocket = accept(serverSocket, &clientSocketAddress, &clientSocketAddressSize);
        if (clientSocket == -1)
        {
            printf("accept has failed\n");
        }
    }

    printf("http-server has closed\n");
    return 0;
}