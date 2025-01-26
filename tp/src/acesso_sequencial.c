#include <stdio.h>
#include <stdlib.h>
#include "../include/acesso_sequencial.h"

// Função auxiliar para gerar a tabela de índices
int gerar_tabela_indices(FILE *arq, int *tab, int qtd, dados *cont) {
    tipoitem item;
    for (int i = 0; i < qtd; i++) {
        if (fread(&item, sizeof(tipoitem), 1, arq) == 1) {
            cont->acessos++;
            tab[i] = item.chave; // Armazena a chave na tabela de índices
        } else {
            return 0; // Falha ao ler o registro
        }
    }
    return 1; // Sucesso
}

// Função principal de acesso sequencial indexado com busca binária
int acesso_sequencial_indexado(int qtd, int chave, dados *cont, const char *nome_arquivo) {
    tipoitem item;
    int *tab = malloc(qtd * sizeof(int)); // Alocação dinâmica para a tabela de índices
    if (tab == NULL) {
        printf("Erro ao alocar memória para a tabela de índices.\n");
        return 0;
    }

    // Abre o arquivo binário para leitura
    FILE *arq = fopen(nome_arquivo, "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        free(tab);
        return 0;
    }

    // Gera a tabela sequencial de índices
    if (!gerar_tabela_indices(arq, tab, qtd, cont)) {
        printf("Erro ao gerar a tabela de índices a partir do arquivo.\n");
        free(tab);
        fclose(arq);
        return 0;
    }

    // Realiza a busca binária na tabela de índices
    int esquerda = 0, direita = qtd - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2; // Evita overflow ao calcular o meio
        cont->comparacoes++;

        if (tab[meio] == chave) {
            // Chave encontrada no índice, busca o registro correspondente no arquivo
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

    // Caso a chave não seja encontrada
    printf("Chave não encontrada!\n");
    printf("Número de transferências (leitura): %d\n", cont->acessos);
    printf("Número de comparações: %d\n", cont->comparacoes);
    free(tab);
    fclose(arq);
    return 0;
}
