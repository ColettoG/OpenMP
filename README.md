# OpenMP

## Descrição

Este arquivo explica, de forma simples, como compilar e executar o programa `parallel_sum.c`, que faz a soma paralela e serial de uma lista e compara tempos de execução.

## Pré-requisitos

* GCC com suporte ao OpenMP (libgomp).

## Compilação

Abra um terminal na pasta onde está o arquivo e execute:

```bash
gcc -fopenmp parallel_sum.c -o parallel_sum
```

## Execução

Após compilar, execute:

```bash
./parallel_sum
```

O programa exibirá:

* Número de processadores disponíveis
* Tempo e resultado das somas paralela e serial

