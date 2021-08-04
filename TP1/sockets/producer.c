#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int numberGenerator(int n)
{
    return n + random() % 100 + 1;
}

void sendNumber(int sockfd, int max)
{
    char buff[MAX];
    int n = 1;
    int generatedNumbers = 0;
    while (n != 0)
    {
        bzero(buff, sizeof(buff));
        if (generatedNumbers == max)
            n = 0;
        else
        {
            n = numberGenerator(n);
            generatedNumbers++;
        }
        sprintf(buff, "%d", n);
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
    }
}

int main(int argc, char *argv[])
{
    srandom(time(NULL));
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Erro ao criar socket\n");
        exit(0);
    }
    else
        printf("Socket criado\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Erro ao conectar ao servidor\n");
        exit(0);
    }
    else
        printf("Conectado ao servidor\n");

    sendNumber(sockfd, atoi(argv[1]));

    close(sockfd);
}