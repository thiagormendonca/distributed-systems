#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

int accumulator = 0;
char *vector;

struct lock
{
    int held;
};

void acquire(struct lock lock)
{
    while (__sync_lock_test_and_set(&lock.held, 1))
        ;
}

void release(struct lock *lock)
{
    lock->held = 0;
}

struct arg_struct
{
    char *arg1;
    struct lock arg2;
    int arg3;
};

void *sum(void *arguments)
{
    // pega os argumentos
    struct arg_struct *args = arguments;
    int n = args->arg3;
    int soma = 0;

    // soma os elementos a variável soma
    for (int i = 0; i < n; i++)
    {
        soma += (int)(args->arg1[i]);
    }

    // usa o lock pra escrever no acumulador
    acquire(args->arg2);
    accumulator += soma;
    release(&args->arg2);

    return NULL;
}

void gen_random_numbers(char *numbers, int n)
{
    for (int i = 0; i < n; i++)
    {
        // gera números aleatórios entre -100 e 100
        numbers[i] = (char)((rand() % 200) - 100);
    }
}

void gen_threads(char *numbers, int n, int k)
{
    struct lock lock;
    lock.held = 0;
    int part_size = n / k;
    pthread_t thread_id[k];
    struct arg_struct args[k];

    // para cada thread até k - 1
    for (int i = 0; i < k - 1; i++)
    {
        // pega o endereço do primeiro elemento do vetor dessa thread
        args[i].arg1 = &numbers[i * part_size];
        args[i].arg2 = lock;
        args[i].arg3 = part_size;
        pthread_create(&thread_id[i], NULL, &sum, (void *)&args[i]);
    }

    // para a última thread pega o que restou do vetor
    int left_part = part_size + n % k;
    args[k - 1].arg1 = &numbers[(k - 1) * left_part];
    args[k - 1].arg2 = lock;
    args[k - 1].arg3 = left_part;
    pthread_create(&thread_id[k - 1], NULL, &sum, (void *)&args[k - 1]);

    // espera as threads executarem
    for (int i = 0; i < k; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    vector = malloc(n * sizeof(char));

    srand(time(NULL));

    clock_t before, after;
    double time;

    // executa 10 vezes para obter o tempo médio
    for (int i = 0; i < 10; i++)
    {
        gen_random_numbers(vector, n);
        before = clock();
        gen_threads(vector, n, k);
        after = clock();
        time += ((double)(after - before)) / CLOCKS_PER_SEC;
        accumulator = 0;
    }

    printf("%f\n", time / 10);

    return 0;
}