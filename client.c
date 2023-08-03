#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int socketFileDesc = socket(AF_INET, SOCK_STREAM, 0);
    char *ip = "127.0.0.1";
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_port = htons(3000);
    address->sin_family = AF_INET;

    inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    int res = connect(socketFileDesc, (struct sockaddr *)address, sizeof(struct sockaddr));

    if (res == 0)
        printf("Connected !!");

    char *line = NULL;
    size_t lineSize = 0;

    while (1)
    {
        size_t charCount = getline(&line, &lineSize, stdin);
        if (charCount > 0 && strcmp(line, "exit\n") == 0)
            break;

        send(socketFileDesc, line, charCount, 0);
    }

    close(socketFileDesc);
    shutdown(socketFileDesc, SHUT_RDWR);
}