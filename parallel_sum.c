#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    const int n = 50;
    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    // Número de threads por argumento, ou padrão 2
    int n_threads = 2;
    if (argc > 1) {
        n_threads = atoi(argv[1]);
        if (n_threads < 1) n_threads = 1;
        if (n_threads > n) n_threads = n;
    }
    omp_set_num_threads(n_threads);

    // Gera lista aleatória
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }

    long *part_sum = calloc(n_threads, sizeof(long));
    if (!part_sum) {
        perror("calloc");
        free(arr);
        return 1;
    }

    printf("Threads: %d  |  Tamanho do vetor: %d\n", n_threads, n);

    // --- Soma paralela ---
    double t0 = omp_get_wtime();
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int chunk = n / n_threads;
        int start = id * chunk;
        int end = (id == n_threads - 1) ? n : start + chunk;

        long local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += arr[i];
        }
        part_sum[id] = local_sum;
    }
    long parallel_sum = 0;
    for (int i = 0; i < n_threads; i++) {
        parallel_sum += part_sum[i];
    }
    double t1 = omp_get_wtime();

    // --- Soma serial ---
    double t2 = omp_get_wtime();
    long serial_sum = 0;
    for (int i = 0; i < n; i++) {
        serial_sum += arr[i];
    }
    double t3 = omp_get_wtime();

    // --- Resultados ---
    printf("\nResultados:\n");
    printf("  Soma paralela: %ld  (%.6f s)\n", parallel_sum, t1 - t0);
    printf("  Soma serial:  %ld  (%.6f s)\n", serial_sum,  t3 - t2);
    printf("  Comparação:   %s\n",
           (parallel_sum == serial_sum) ? "iguais" : "diferentes");

    free(arr);
    free(part_sum);
    return 0;
}