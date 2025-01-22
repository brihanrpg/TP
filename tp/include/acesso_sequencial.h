#ifndef ACESSO_SEQUENCIAL_H
#define ACESSO_SEQUENCIAL_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>

int pesquisaPSI(int tab[], int tam, int chave, int qtd, FILE *arq, int *acessos,
                int *comparacoes);

int PesquisaBinaria(tipoitem *pagina, int esq, int dir, int x, int *acessos,
                    int *comparacoes);

#endif // ACESSO_SEQUENCIAL_H
