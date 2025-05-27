#include <stdio.h>
#include <omp.h>

int main() {
    // Exemplo de lista
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int n   = sizeof(arr) / sizeof(arr[0]);
    int mid = n / 2;

    // Buffer para as duas somas parciais
    long part_sum[2] = { 0, 0 };

    // Imprime número de processadores disponíveis
    printf("Número de processadores disponíveis: %d\n", omp_get_num_procs());

    // --- Somatório paralelo ---
    double t0 = omp_get_wtime();

    #pragma omp parallel num_threads(2)
    {
        int id    = omp_get_thread_num();
        int start = id * mid;
        int end   = (id == 0 ? mid : n);

        long local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += arr[i];
        }
        part_sum[id] = local_sum;
    }

    long parallel_sum = part_sum[0] + part_sum[1];
    double t1 = omp_get_wtime();
    double parallel_time = t1 - t0;

    // --- Somatório serial ---
    double t2 = omp_get_wtime();

    long serial_sum = 0;
    for (int i = 0; i < n; i++) {
        serial_sum += arr[i];
    }

    double t3 = omp_get_wtime();
    double serial_time = t3 - t2;

    // --- Resultados ---
    printf("\nResultados:\n");
    printf("  Soma paralela: %ld  (%.6f s)\n", parallel_sum, parallel_time);
    printf("  Soma serial:  %ld  (%.6f s)\n", serial_sum,  serial_time);
    printf("  Comparação:   %s\n",
           (parallel_sum == serial_sum) ? "iguais" : "diferentes");

    return 0;
}