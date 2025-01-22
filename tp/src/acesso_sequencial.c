#include "../include/acesso_sequencial.h"

int pesquisaPSI(int tab[],int tam,int chave,int qtd,FILE *arq,int *acessos,int *comparacoes){
    rewind(arq);
    tipoitem pagina[ITENSPAGINA];
    int i,quantitens;
    long desloc;

    i=0;

    while(i < tam && tab[i] <= chave)//i vai ate a possivel pagina do registro
       i++;
    (*comparacoes)+=i;
    if(i == 0)//nao tem nenhuma pagina
        return 0;
    else {
        if(i<tam)
            quantitens = ITENSPAGINA;
        else{
            //fseek(arq , 0, SEEK_END);
            quantitens=qtd%ITENSPAGINA;//se qtd de itens nao for multiplo da qtd por pagina a ultima pagina esta "incompleta"
            if(quantitens == 0)
                quantitens=ITENSPAGINA;
        }
        if (i - 1 < 0) {
          printf("Erro: deslocamento negativo no arquivo.\n");
          return 0;
        }

        desloc = (i-1) * ITENSPAGINA *sizeof(tipoitem);
        fseek (arq, desloc,SEEK_SET);
        if (fread(&pagina, sizeof(tipoitem), quantitens, arq) != quantitens) {
            printf("Erro ao ler os dados do arquivo.\n");
            return 0;
        }
        (*acessos)++;
        int retorno=PesquisaBinaria(pagina,0,quantitens-1,chave,acessos,comparacoes);//pesquisa pelo item dentro da pagina
        if(retorno == -1)
            return 0;
        else 
            return 1;

    }
}

int PesquisaBinaria(tipoitem *pagina,int esq,int dir,int x,int *acessos,int *comparacoes){
  
  int meio=(int)(esq+dir)/2;
  if (meio < 0 || meio >= ITENSPAGINA) {
    printf("Erro: índice fora dos limites do array.\n");
    return -1;
}
  if(esq > dir)
    return -1;

  else if(x < pagina[meio].chave){
    (*comparacoes)++;
    return PesquisaBinaria(pagina, esq,meio-1, x,acessos,comparacoes);
  }

  else if(x > pagina[meio].chave){
    (*comparacoes)++;
    return PesquisaBinaria(pagina, meio+1,dir, x,acessos,comparacoes);
  }

  else 
    return meio;
}