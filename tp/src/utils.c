#include "../include/registro.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Função para gerar um arquivo binário com registros ordenados crescentemente
void gerarCrescente(int quantidade) {
    const char *nomeArquivo = "./data/crescente.bin";

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    tipoitem registro;
    for (int i = 1; i <= quantidade; i++) {  // Inicia em 1
        registro.chave = i;
        registro.dado1 = (long int)(rand() % 10000);
        snprintf(registro.dado2, sizeof(registro.dado2), "Registro %d", i);
        fwrite(&registro, sizeof(tipoitem), 1, arquivo);
    }

    fclose(arquivo);
    printf("Arquivo gerado em ordem crescente: %s\n", nomeArquivo);
}

// Função para gerar um arquivo binário com registros ordenados decrescentemente
void gerarDecrescente(int quantidade) {
    const char *nomeArquivo = "./data/decrescente.bin";

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    tipoitem registro;
    for (int i = quantidade; i >= 1; i--) {  // Inicia em `quantidade` e vai até 1
        registro.chave = i;
        registro.dado1 = (long int)(rand() % 10000);
        snprintf(registro.dado2, sizeof(registro.dado2), "Registro %d", i);
        fwrite(&registro, sizeof(tipoitem), 1, arquivo);
    }

    fclose(arquivo);
    printf("Arquivo gerado em ordem decrescente: %s\n", nomeArquivo);
}

// Função para gerar um arquivo binário com registros em ordem aleatória
void gerarAleatorio(int quantidade) {
    const char *nomeArquivo = "./data/aleatorio.bin";

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    tipoitem *registros = malloc(quantidade * sizeof(tipoitem));
    if (!registros) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    // Preenche os registros com valores de 1 até `quantidade`
    for (int i = 1; i <= quantidade; i++) {
        registros[i - 1].chave = i;
        registros[i - 1].dado1 = (long int)(rand() % 10000);
        snprintf(registros[i - 1].dado2, sizeof(registros[i - 1].dado2), "Registro %d", i);
    }

    // Embaralhar registros
    for (int i = 0; i < quantidade; i++) {
        int j = rand() % quantidade;
        tipoitem temp = registros[i];
        registros[i] = registros[j];
        registros[j] = temp;
    }

    fwrite(registros, sizeof(tipoitem), quantidade, arquivo);

    free(registros);
    fclose(arquivo);
    printf("Arquivo gerado em ordem aleatória: %s\n", nomeArquivo);
}
