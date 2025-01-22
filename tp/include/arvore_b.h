#ifndef ARVORE_B_H
#define ARVORE_B_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TipoPagina *TipoApontador;

typedef struct TipoPagina {
  short n;
  tipoitem r[MM];
  TipoApontador p[MM + 1];
} TipoPagina;

void Inicializa(TipoApontador Arvore);

void InsereAB(tipoitem Reg, TipoApontador *Ap, int *comparacoes);

int PesquisaAB(int chave, TipoApontador Ap, int *comparacoes);

void Ins(tipoitem Reg, TipoApontador Ap, short *Cresceu, tipoitem *RegRetorno,
         TipoApontador *ApRetorno, int *comparacoes);

void InsereNaPagina(TipoApontador Ap, tipoitem Reg, TipoApontador Apdir,
                    int *comparacoes);

void limpar(TipoApontador arvore);

#endif // ARVORE_B_H
