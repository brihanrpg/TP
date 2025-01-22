#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int chave;
  long int dado1;
  char dado2[5000];
  int prox, ant;
  int posicao, atualizado;
} tipoitemarvore;

void atualizaPonteiroABP(int posicaoFilho, FILE *arquivo, int *acessos, int *comparacoes);
int procuraArvoreABP(int chave, FILE *arquivo,int *acessos,int* comparacoes);

#endif // ARVORE_BINARIA_H
