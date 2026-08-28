#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10

typedef struct sArgs {
    int idThread, nThreads;
} tArgs;

void *printHello(void * arg) {
    tArgs *args = (tArgs *) arg;
    printf("Hello World da Thread %d/%d\n", args->idThread, (args->nThreads - 1));
    free(arg);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t tid_sistema[NTHREADS];
    tArgs *args;
    for (int i = 0; i < NTHREADS; i++) {
        printf("--Cria a Thread %d\n", i);
        args = malloc(sizeof(tArgs));
        args->idThread = i;
        args->nThreads = NTHREADS;
        if (pthread_create(&tid_sistema[i], NULL, printHello, (void *) args)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    for (int i = 0; i < NTHREADS; i++) {
        printf("--Termina a Thread %d\n", i);
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
}