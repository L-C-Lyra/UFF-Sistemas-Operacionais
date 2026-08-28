#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10

void *printHello(void * arg) {
    printf("Hello World\n");
    pthread_exit(NULL);
}

int main(void) {
    pthread_t tid_sistema[NTHREADS];
    for (int i = 0; i < NTHREADS; i++) {
        printf("--Cria a Thread %d\n", i);
        if (pthread_create(&tid_sistema[i], NULL, printHello, NULL)) {
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