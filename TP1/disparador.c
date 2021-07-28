#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    if (kill(atoi(argv[1]), atoi(argv[2])) == -1)
    {
        if (errno == ESRCH)
            printf("Processo não existe!\n");
    }
    else
    {
        printf("Sinal enviado.\n");
    }

    return 0;
}