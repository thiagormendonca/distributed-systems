#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "time.h"
#include <semaphore.h>
#include <math.h>
#include <stdbool.h>


int memory_size;
int *memory;
int consumed;
int Np, Nc;

clock_t start, end;

//threads
pthread_t *consumer_threads;
pthread_t *producer_threads;


//semaphores
sem_t mutex;
sem_t empty_sem;
sem_t full;

//random number (1,10^7)
int randomNumber()
{
    return (rand() % 10000000) + 1;
}

//verify prime number
int isPrime(int n)
{
    if (n <= 1)
        return 0;

    for (int i = 2; i <= sqrt((double)n); i++)
        if (n % i == 0)
            return 0;

    return 1;
}

//insert number in shared memory
void insertNumber(int number)
{
    for (int i = 0; i < memory_size; i++)
    {
        if (memory[i] == 0)
        {
            memory[i] = number;
            break;
        }
    }
}

// consumes number in shared memory 
void consume()
{
    int number;
    for (int i = 0; i < memory_size; i++)
    {
        number = memory[i];
        if (number != 0)
        {
            //answers if number is prime
            if(isPrime(number) == 1){
                printf("Consumer: %ls Is prime.", &number);
            }
            else{
                printf("Consumer: %ls aint`t prime.", &number);
            }
            memory[i] = 0;
            //increments consumed count
            consumed++;
            break;
        }
    }
}

//producer threads
void producer()
{
    while (true)
    {
        int number = randomNumber();

        //awaits empty semaphore
        sem_wait(&empty_sem);
        sem_wait(&mutex);
        
        // add number to shared memory
        insertNumber(number);

        //clears mutex and increments semaphore
        sem_post(&mutex);
        sem_post(&full);
    }
}

//consumer threads
void consumer()
{
    // race condition
    while (true)
    {
        // awaits semaphore
        sem_wait(&full);
        sem_wait(&mutex);

        consume();

        //end task when hits 100000 cycles
        if (consumed == 100000)
        {
            //ends execution time
            end = clock();

            //execution time
            double runTime = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("%i / %i / %i / %.20lf", memory_size, Np, Nc, runTime );
            exit(0);
        }
        sem_post(&mutex);
        sem_post(&empty_sem);
    }
}

//creates pre-defined number of producer threads
void producerThreads(int num_threads)
{
    producer_threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t*));
    for (int i = 0; i < num_threads; i++)
    {
        //states producer threads as producers
        producer_threads[i] = pthread_create(&producer_threads[i], NULL, (void *) &consumer, NULL);
    }
}

//creates pre-defined number of consumer threads
void consumerThreads(int num_threads)
{
    consumer_threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t*));
    // create num_threads threads that will produce numbers
    for (int i = 0; i < num_threads; i++)
    {
        //states consumer threads as consumers
        pthread_create(&consumer_threads[i], NULL, (void *) &consumer, NULL);
    }
}


void joinProducerThreads(int Np)
{
    for (int i = 0; i < Np; i++)
    {
        pthread_join(producer_threads[i], NULL);
    }
}


void joinConsumerThreads(int Nc)
{
    for (int i = 0; i < Nc; i++)
    {
        pthread_join(consumer_threads[i], NULL);
    }
}


int main()
{
    srand(time(NULL));
    //Producer-consumer setup selection
    printf("Enter memory size:");
    scanf("%d",&memory_size);
    printf("Enter Number of producers:");
    scanf("%d",&Np);
    printf("Enter Number of consumers:");
    scanf("%d",&Nc);
    
    //memory array with desired size
    memory = (int*) malloc(memory_size * sizeof(int));//array of zeroes
    
    //sempahores init and setup
    sem_init(&mutex, 1, 1); //init mutex
    sem_init(&full, 1, memory_size); //semaphore max size
    sem_init(&empty_sem, 1, 0); //semaphore min size

    //start execution time
    start = clock();
    // open threads
    producerThreads(Nc);
    consumerThreads(Np);
    joinConsumerThreads(Nc);
    joinProducerThreads(Np);

    free(memory);
    free(consumer_threads);
    return 0;
}