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
        printf("binding address to the server socket has failed\n");
        return -1;
    }

    printf("http-server has closed\n");
    return 0;
}