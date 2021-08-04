#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int isPrime(int n)
{
    if (n <= 1)
        return 0;

    for (int i = 2; i <= sqrt((double)n); i++)
        if (n % i == 0)
            return 0;

    return 1;
}

void checkNumber(int sockfd)
{
    char buff[MAX];
    int prime;

    for (;;)
    {
        bzero(buff, MAX);

        read(sockfd, buff, sizeof(buff));

        if (atoi(buff) == 0)
            break;

        prime = isPrime(atoi(buff));

        bzero(buff, MAX);

        if (prime)
        {
            strncpy(buff, "É primo\n", MAX);
        }
        else
        {
            strncpy(buff, "Não é primo\n", MAX);
        }

        write(sockfd, buff, sizeof(buff));
    }
}

int main(int argc, char *argv[])
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("Erro ao conectar socket\n");
        exit(0);
    }
    else
        printf("Socket conectado\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Erro ao escutar\n");
        exit(0);
    }
    else
        printf("Escutando\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA *)&cli, (unsigned int *)&len);
    if (connfd < 0)
    {
        printf("Erro ao aceitar o cliente\n");
        exit(0);
    }
    else
        printf("Cliente aceito\n");

    checkNumber(connfd);

    close(sockfd);
}