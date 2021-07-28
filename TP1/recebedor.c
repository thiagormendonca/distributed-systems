#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int running = 1;

void signalHandler(int sig)
{
    if (sig == SIGTERM)
    {
        printf("Finalizando...\n");
        running = 0;
    }
    else
    {
        printf("Sinal %d recebido.\n", sig);
    }
}

int main(int argc, char *argv[])
{
    printf("%d\n", getpid());
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGTERM, signalHandler);

    if (atoi(argv[1]) == 0)
    {
        while (running)
            ;
    }
    else
    {
        while (running)
            pause();
    }

    return 0;
}