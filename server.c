#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct connectedSocket
{
    struct sockaddr_in *clientAddress;
    int clientSocketFileDesc;
};

void *printIncomingMessage(void *acceptedSocket);
struct connectedSocket createSocket(int serverSocketFd);

int main()
{
    int serverSocketFileDesc = socket(AF_INET, SOCK_STREAM, 0);
    char *ip = "";
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3000);

    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int res = bind(serverSocketFileDesc, (struct sockaddr *)&serverAddress, sizeof serverAddress);
    if (res == 0)
    {
        printf("Socket bound to port 3000\n");
    }
    int listenRes = listen(serverSocketFileDesc, 15);

    while (1)
    {
        struct connectedSocket acceptedSocket = createSocket(serverSocketFileDesc);
        pthread_t t1;
        pthread_create(&t1, NULL, printIncomingMessage, (void *)&acceptedSocket);
        // pthread_join(t1, NULL);
    }
    shutdown(serverSocketFileDesc, SHUT_RDWR);
    return 0;
}

struct connectedSocket createSocket(int serverSocketFd)
{
    struct connectedSocket cs1;
    cs1.clientAddress = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    int clientAddrSize = sizeof(struct sockaddr_in);
    int clientSocketFileDesc = accept(serverSocketFd, (struct sockaddr *)cs1.clientAddress, &clientAddrSize);
    cs1.clientSocketFileDesc = clientSocketFileDesc;
    return cs1;
}

void *printIncomingMessage(void *acceptedSocket)
{
    struct connectedSocket *clientSocketFileDesc = (struct connectedSocket *)acceptedSocket;
    char buffer[1024];
    while (1)
    {
        ssize_t charRecieved = recv(clientSocketFileDesc->clientSocketFileDesc, buffer, 1024, 0);
        printf("recieved\n");
        if (charRecieved <= 0)
            break;
        buffer[charRecieved] = '\0';
        printf("Client says : %s\n", buffer);
    }

    close(clientSocketFileDesc->clientSocketFileDesc);
    free(clientSocketFileDesc->clientAddress);
    // pthread_exit(NULL);
}