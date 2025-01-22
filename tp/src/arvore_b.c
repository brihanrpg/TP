#include "../include/arvore_b.h"

void Inicializa(TipoApontador *Arvore){
    *Arvore = NULL;
}

void InsereAB(tipoitem Reg,TipoApontador *Ap,int *comparacoes){
    short Cresceu;
    tipoitem RegRetorno;
    TipoPagina *ApRetorno,*ApTemp;
    Ins(Reg,*Ap,&Cresceu,&RegRetorno,&ApRetorno,comparacoes);
    if(Cresceu){//se precisa de nova raiz
        ApTemp=malloc(sizeof(TipoPagina));
        ApTemp->n=1;
        ApTemp->r[0]=RegRetorno;
        ApTemp->p[1]=ApRetorno;
        ApTemp->p[0]=*Ap;
        *Ap=ApTemp;
    }
}

void Ins(tipoitem Reg, TipoApontador Ap, short *Cresceu, tipoitem *RegRetorno, TipoApontador *ApRetorno, int *comparacoes){
    long i = 1; 

    TipoApontador ApTemp;
    if (Ap == NULL) {  // Ou não existe a árvore ou chegou nas folhas
        *Cresceu = 1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }

    while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave) {  // Verifica os índices
        (*comparacoes)++;
        i++;
    }

    if (i > 0 && Reg.chave == Ap->r[i - 1].chave) {  // Verifique se o índice não é inválido
        (*comparacoes)++;
        *Cresceu = 0;
        return;
    }

    if (i > 0 && Reg.chave < Ap->r[i - 1].chave) {  // Verifique se o índice não é inválido
        (*comparacoes)++;
        i--;
    }

    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, comparacoes);  // Faz recursão até encontrar local de inserção
    if (!(*Cresceu)) return;

    if (Ap->n < MM) {  // Caso página caiba item
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, comparacoes);
        *Cresceu = 0;
        return;
    }

    ApTemp = malloc(sizeof(TipoPagina));  // Criação de nova página pois a que deveria ser inserida está lotada
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < (M + 1)) {  // Saber se o item ficará na nova página ou na página existente
        InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM], comparacoes);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, comparacoes);
    } else {
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, comparacoes);
    }

    for (int j = M + 2; j <= MM; j++) {  // Move os itens para a nova página
        InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j], comparacoes);
    }

    Ap->n = M;  // Atualiza o tamanho da nova página
    ApTemp->p[0] = Ap->p[M + 1];  // Primeiro filho da nova página é o último da página que foi dividida
    *RegRetorno = Ap->r[M];  // Registro que deve subir
    *ApRetorno = ApTemp;  // Filho à direita do que vai subir
}


void InsereNaPagina(TipoApontador Ap,tipoitem Reg,TipoApontador Apdir,int *comparacoes){

    int k;
    k=Ap->n;
    short NaoAchouPosicao=0;
    if(k>0)//verifica se existe elementos
        NaoAchouPosicao=1;
    while(NaoAchouPosicao){//se existe elementos procura a posicao correta e move os elementos necessarios e seus ponteiros
        (*comparacoes)++;
        if(Reg.chave >= Ap->r[k-1].chave){

            NaoAchouPosicao=0;
            break;
        }
        Ap->r[k]=Ap->r[k-1];
        Ap->p[k+1]=Ap->p[k];
        k--;
        if(k<1)
            NaoAchouPosicao=0;
    }
    Ap->r[k]=Reg;
    Ap->p[k+1]=Apdir;
    Ap->n++;
}

void limpar(TipoApontador arvore){
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        limpar(arvore->p[i]);
        i++;
    }
    free(arvore);
}

int PesquisaAB(int chave,TipoApontador Ap,int *comparacoes){
    long i=1;
    if(Ap == NULL)
        return 0;
    while(i<Ap->n && chave > Ap->r[i-1].chave){//vai ate a possivel posicao do item ou do seu pai
        (*comparacoes)++;
        i++;
    }

    if(chave == Ap->r[i-1].chave){//verifica se achou o item
        (*comparacoes)++;
        return 1;
    }
        //caso contrario continua a busca pelo lado correto ate encontrar item ou ate apontar para NULL(nao achou)
    else if(chave < Ap->r[i-1].chave){
        (*comparacoes)++;
        return PesquisaAB(chave,Ap->p[i-1],comparacoes);
    }

    else    {
        (*comparacoes)++;
        return PesquisaAB(chave,Ap->p[i],comparacoes);
    }

}
