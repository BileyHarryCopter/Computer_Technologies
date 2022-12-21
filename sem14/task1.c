#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7
#define MAXLEN 1000

int main(char argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Too few arguments\n");
        exit(-1);
    }

    struct sockaddr_in cliaddr;
    bzero(&cliaddr, sizeof(cliaddr));

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sockd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockd < 0)
    {
        perror(NULL);
        exit(-1);
    }

    if (bind(sockd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
    {
        perror(NULL);
        close(sockd);
        exit(-1);
    }

    struct sockaddr_in servaddr;
    bzero(&cliaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        perror(NULL);
        close(sockd);
        exit(-1);
    }

    printf("Type a string => ");
    char sendline[MAXLEN] = {0};
    fgets(sendline, MAXLEN, stdin);

    if (sendto(sockd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockd);
        exit(-1);
    }

    char recvline[MAXLEN] = {0};
    if (recvfrom(sockd, recvline, MAXLEN, 0, NULL, NULL) < 0)
    {
        perror(NULL);
        close(sockd);
        exit(-1);
    }

    printf("Recieved message: %s\n", recvline);
    close(sockd);

    return 0;
}
