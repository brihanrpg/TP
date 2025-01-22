#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/acesso_sequencial.h"
#include "../include/arvore_binaria.h"
#include "../include/arvore_b.h"
#include "../include/arvore_b_star.h"
#include "../include/registro.h"
#include "../include/utils.h"

// Arquivo decrescente vai de 1999999 até 0

// Definindo limites para a quantidade de registros
#define MAX_REGISTROS 2000000
#define MIN_REGISTROS 1

int main(int argc, char *argv[]) {
    clock_t start, end;
    double elapsed;
    start = clock();

    // Verificando se o número correto de argumentos foi passado
    if (argc < 5 || argc > 6) {
        printf("Uso incorreto. Sintaxe esperada: pesquisa <metodo> <quantidade> <situacao> <chave> [-P]\n");
        return 0;
    }

    int metodo = atoi(argv[1]);
    int qtd = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);

    // Validando as entradas fornecidas pelo usuário
    if ((metodo < 1 || metodo > 4) || (qtd < MIN_REGISTROS || qtd > MAX_REGISTROS) || (situacao < 1 || situacao > 3)) {
        printf("Entrada inválida!\n\n");
        printf("Métodos:\n1: Pesquisa sequencialmente indexada (Apenas ordem crescente)\n");
        printf("2: Árvore binária externa\n3: Árvore B\n4: Árvore B*\n\n");
        printf("Quantidade permitida: Entre %d e %d itens\n", MIN_REGISTROS, MAX_REGISTROS);
        printf("Situações:\n1: Ordenado crescentemente\n2: Ordenado decrescentemente\n3: Ordenado aleatoriamente\n");
        return 0;
    }

    FILE *arq;
    tipoitem aux;
    int acessos = 0, comparacoes = 0, contarq = 0;

    // Abrindo o arquivo de acordo com a situação especificada
    if (situacao == 1) {
        arq = fopen("./data/crescente.bin", "rb");
        gerarCrescente(qtd);

    } else if (situacao == 2) {
        arq = fopen("./data/decrescente.bin", "rb");
        gerarDecrescente(qtd);
    } else {
        arq = fopen("./data/aleatorio.bin", "rb");
        gerarAleatorio(qtd);
    }

    if (arq == NULL) {
        printf("Erro na abertura do arquivo. Verifique o caminho e tente novamente.\n");
        return 0;
    }

    // Se o argumento -P for passado, imprime as chaves do arquivo
    if (argc == 6 && strcmp(argv[5], "-P") == 0) {
        while (fread(&aux, sizeof(tipoitem), 1, arq) > 0 && contarq < qtd) {
            printf("%d ", aux.chave);
            contarq++;
        }
        printf("\n");
        rewind(arq);  // Volta ao início do arquivo após a impressão
    }

    // Executando o método de pesquisa conforme a escolha do usuário
    switch (metodo) {
    case 1:  // Pesquisa Sequencialmente Indexada
    {
        int tabela[MAXTABELA];
        tipoitem x[ITENSPAGINA];
        int pos = 0, cont = 0;

        while (fread(&x, sizeof(tipoitem), ITENSPAGINA, arq) > 0 && cont < qtd) {
            tabela[pos] = x[0].chave;
            pos++;
            cont += ITENSPAGINA;
            acessos++;
        }
        fseek(arq, 0, SEEK_SET);  // Reposiciona o ponteiro de leitura

        if (pesquisaPSI(tabela, pos, chave, qtd, arq, &acessos, &comparacoes)) {
            printf("Registro de código %d foi localizado\n", chave);
        } else {
            printf("Registro de código %d não foi localizado\n", chave);
        }
        fclose(arq);
        break;
    }
    case 2:  // Árvore Binária 
    {
        FILE *arquivo2 = fopen("../output/arvorebin.bin", "wb+");
        tipoitem temp;
        tipoitemarvore itemcompleto;
        int pos = 0;

        while (fread(&temp, sizeof(tipoitem), 1, arq) == 1 && pos < qtd) {
            itemcompleto.chave = temp.chave;
            itemcompleto.dado1 = temp.dado1;
            strcpy(itemcompleto.dado2, temp.dado2);  // Cuidado com o uso de strcpy
            itemcompleto.ant = -1;
            itemcompleto.prox = -1;
            itemcompleto.posicao = pos;
            itemcompleto.atualizado = 0;
            fwrite(&itemcompleto, sizeof(tipoitemarvore), 1, arquivo2);
            pos++;
            acessos++;
        }

        rewind(arquivo2);
        for (int i = 1; i < pos; i++) {
            atualizaPonteiroABP(i, arquivo2, &acessos, &comparacoes);
        }

        int retorno = procuraArvoreABP(chave, arquivo2, &acessos, &comparacoes);
        if (retorno != -1) {
            printf("Registro de código %d foi localizado\n", chave);
        } else {
            printf("Registro de código %d não foi localizado\n", chave);
        }
        fclose(arquivo2);
        break;
    }
    case 3:  // Árvore B
    {
        tipoitem temp;
        TipoApontador Ap = NULL;
        Inicializa(&Ap);
        int cont = 0;

        while (fread(&temp, sizeof(tipoitem), 1, arq) == 1 && cont < qtd) {
            InsereAB(temp, &Ap, &comparacoes);
            cont++;
            acessos++;
        }
        fclose(arq);

        int retorno = PesquisaAB(chave, Ap, &comparacoes);
        if (retorno > 0) {
            printf("Registro de código %d foi localizado\n", chave);
        } else {
            printf("Registro de código %d não foi localizado\n", chave);
        }
        limpar(Ap);
        break;
    }
    case 4:  // Árvore B*
    {
        tipoitem temp, pesquisado;
        pesquisado.chave = chave;
        TipoApontador2 Ap = NULL;
        int cont = 0;

        while (fread(&temp, sizeof(tipoitem), 1, arq) == 1 && cont < qtd) {
            InsereEstrela(temp, &Ap, &comparacoes);
            cont++;
            acessos++;
        }
        rewind(arq);
        fclose(arq);

        int retorno = PesquisaEstrela(&pesquisado, &Ap, &comparacoes);
        if (retorno) {
            printf("Registro de código %d foi localizado\n", chave);
        } else {
            printf("Registro de código %d não foi localizado\n", chave);
        }
        break;
    }
    }

    // Calculando o tempo de execução
    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: %f segundos\nTransferências: %d\nComparações: %d\n", elapsed, acessos, comparacoes);

    return 0;
}
