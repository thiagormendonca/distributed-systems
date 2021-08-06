#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#define MAX 80

int fd[2];

int numberGenerator(int n)
{
    return n + random() % 100 + 1;
}

int isPrime(int n)
{
    if (n <= 1)
        return 0;

    for (int i = 2; i <= sqrt((double)n); i++)
        if (n % i == 0)
            return 0;

    return 1;
}

void producer(int max)
{
    char buff[MAX];
    int n = 1;
    int generatedNumbers = 0;
    while (n != 0)
    {
        if (generatedNumbers == max)
            n = 0;
        else
        {
            n = numberGenerator(n);
            generatedNumbers++;
        }
        bzero(buff, MAX);
        sprintf(buff, "%d", n);
        write(fd[1], buff, sizeof(buff));
    }
}

void consumer()
{
    char buff[MAX];
    int prime;

    for (;;)
    {
        bzero(buff, MAX);

        read(fd[0], buff, sizeof(buff));

        if (atoi(buff) == 0)
            break;

        prime = isPrime(atoi(buff));

        if (prime)
        {
            printf("É primo\n");
        }
        else
        {
            printf("Não é primo\n");
        }
    }
}

int main(int argc, char *argv[])
{
    if (pipe(fd) == -1)
    {
        printf("Ocorreu um erro ao abrir o pipe\n");
        return 1;
    }

    int id = fork();

    if (id < 0)
    {
        printf("Erro ao fazer fork\n");
        return 1;
    }
    else if (id > 0)
    {
        producer(atoi(argv[1]));
    }
    else
    {
        consumer();
    }

    return 0;
}