#include <stdio.h>
#include <sys/socket.h>

int main(void)
{
    printf("http-server has started\n");

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);


    printf("http-server has closed\n");
    return 0;
}