#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "timer.h"

int nThreads, tamVet, *vet, *somaVet;

void *somaVetor(void *arg) {
    int soma = 0;
    unsigned int nop = 0;
    int *idThread = (int *) arg;
    for (int i = *idThread; i < tamVet; i += nThreads) {
        soma += vet[i];
        for (unsigned int j = 0; j < 1000000; j++) nop++;
    }
    somaVet[*idThread] = soma;
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid_sistema;
    int *arg;
    double inicio, fim, delta1, delta2, delta3;
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int numCPU = sysinfo.dwNumberOfProcessors;
#else
    int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    printf("Numero de Processadores: %d\n", numCPU);
    printf("Digite o Tamanho do Vetor:");
    scanf("%d", &tamVet);
    printf("Digite o Numero de Threads:");
    scanf("%d", &nThreads);
    GET_TIME(inicio);
    tid_sistema = (pthread_t *) malloc(nThreads * sizeof(pthread_t));
    somaVet = malloc(nThreads * sizeof(int));
    vet = malloc(tamVet * sizeof(int));
    for (int t = 0; t < tamVet; t++) vet[t] = 1;
    GET_TIME(fim);
    delta1 = fim - inicio;
    GET_TIME(inicio);
    for (int t = 0; t < nThreads; t++) {
        arg = malloc(sizeof(int));
        *arg = t;
        if (pthread_create(&tid_sistema[t], NULL, somaVetor, (void *) arg)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    for (int t = 0; t < nThreads; t++) {
        if (pthread_join(tid_sistema[t], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }
    GET_TIME(fim);
    delta2 = fim - inicio;
    GET_TIME(inicio);
    int S = 0;
    for (int t = 0; t < nThreads; t++) {
        printf("%d ", somaVet[t]);
        S += somaVet[t];
    }
    printf("\nSOMA = %d\n", S);
    free(vet);
    free(somaVet);
    free(tid_sistema);
    GET_TIME(fim);
    delta3 = fim - inicio;
    printf("Tempo Inicializacoes: %.8lf\n", delta1);
    printf("   Tempo Concorrente: %.8lf\n", delta2);
    printf("  Tempo Finalizacoes: %.8lf\n", delta3);
    pthread_exit(NULL);
}