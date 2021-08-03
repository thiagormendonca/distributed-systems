#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <time.h>
#include<sys/wait.h>


int main(){
    int fd[2];
    // int numbers = 100;
    // int n = 1;
    // int i;
    // for(i = 1; i < numbers; ++i){
    //     int randomNumber = rand() % 100;
    //     n = n + randomNumber;
    //     printf("N: %d  Random Number: %d \n", n, randomNumber );
    // }

    if(pipe(fd) == -1){
        printf("Ocorreu um erro ao abrir o pipe\n");
        return 1;
    }
    int id = fork();
    if(id == 0){
        // for(i = 1; i < numbers; ++i){
        //     int randomNumber = rand() % 100;
        //     n = n + randomNumber;
        //     write(fd[1], &n, sizeof(int));
        //     close(fd[0]);
        // }
        close(fd[0]);
        int x;
        printf("Input a number:");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(int));
        close(fd[1]);
    } else{
        close(fd[1]);
        int y;
        read(fd[0], &y, sizeof(int));
        close(fd[0]);
        printf("Recebido do processo filho: %d", y);

    }

    return 0;
}