#include <stdio.h>
#include <stdlib.h>
#include "../include/acesso_sequencial.h"

int acesso_sequencial_indexado(int qtd, int chave, dados *cont, FILE *arq) {
    tipoitem item;
    int *tab = malloc(qtd * sizeof(int)); // Alocação dinâmica para a tabela de índices
    if (tab == NULL) {
        printf("Erro ao alocar memória para a tabela de índices.\n");
        return 0;
    }

    // Abre o arquivo binário para leitura
    arq = fopen("./data/crescente.bin", "r+");
    if (arq == NULL) {
        printf("Erro na abertura do arquivo!\n");
        free(tab);
        return 0;
    }

    // Gera a tabela sequencial de índices
    rewind(arq); // Garante que a leitura comece no início do arquivo
    for (int i = 0; i < qtd; i++) {
        if (fread(&item, sizeof(tipoitem), 1, arq) == 1) {
            cont->acessos++;
            tab[i] = item.chave; // Armazena a chave na tabela de índices
        } else {
            break;
        }
    }

    // Realiza a busca com divisão e conquista
    int esquerda = 0, direita = qtd - 1;
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        cont->comparacoes++;

        if (tab[meio] == chave) {
            // Chave encontrada no índice, busca o registro correspondente
            fseek(arq, meio * sizeof(tipoitem), SEEK_SET);
            fread(&item, sizeof(tipoitem), 1, arq);
            cont->acessos++;
            printf("\nChave encontrada: %d\nDado1: %ld\nDado2: %s\n", item.chave, item.dado1, item.dado2);
            printf("Número de transferências (leitura): %d\n", cont->acessos);
            printf("Número de comparações: %d\n", cont->comparacoes);
            free(tab);
            fclose(arq);
            return 1;
        }

        if (tab[meio] < chave) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    printf("Chave não encontrada!\n");
    printf("Número de transferências (leitura): %d\n", cont->acessos);
    printf("Número de comparações: %d\n", cont->comparacoes);
    free(tab);
    fclose(arq);
    return 0;
}