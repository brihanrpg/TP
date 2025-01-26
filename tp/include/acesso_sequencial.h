#ifndef ACESSO_SEQUENCIAL_H
#define ACESSO_SEQUENCIAL_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
int acesso_sequencial_indexado(int qtd, int chave, dados *cont, FILE *arq);
#endif // ACESSO_SEQUENCIAL_H
