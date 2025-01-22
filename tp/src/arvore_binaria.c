#include "../include/arvore_binaria.h"

void atualizaPonteiroABP(int posicaoFilho, FILE *arquivo,int *acessos,int* comparacoes) {
  int posicaoPai = 0;
  tipoitemarvore filho, pai;
  fseek(arquivo, posicaoFilho * sizeof(tipoitemarvore), SEEK_SET);//resgata registro do filho
  fread(&filho, sizeof(tipoitemarvore), 1, arquivo);
  (*acessos)++;

  // Obtém a raiz da árvore
  fseek(arquivo, 0, SEEK_SET);//inicializa na raiz 
  fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
  (*acessos)++;
  while (1) {
      if (filho.chave < pai.chave) {
          (*comparacoes)++;
          if (pai.ant == -1) {//encontrou a posicao
              break;
          }
          posicaoPai = pai.ant;//caso contrario tem que procurar fazendo o caminhamento pela esquerda
          fseek(arquivo, pai.ant * sizeof(tipoitemarvore), SEEK_SET);
          fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
          (*acessos)++;
      }
      else if (filho.chave > pai.chave) {
          (*comparacoes)++;
          if (pai.prox == -1) {//encontrou a posicao 
              break;
          }
          posicaoPai = pai.prox;//caso contrario tem que procurar fazendo o caminhamento pela direita
          fseek(arquivo, pai.prox * sizeof(tipoitemarvore), SEEK_SET);
          fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
          (*acessos)++;
      }
  }

  // Atualiza o ponteiro anterior ou próximo do pai com a posição do filho
  if (filho.chave < pai.chave) {
      (*comparacoes)++;
      pai.ant = posicaoFilho;
  }
  else {
      (*comparacoes)++;
      pai.prox = posicaoFilho;
  }
  fseek(arquivo, posicaoPai * sizeof(tipoitemarvore), SEEK_SET);//vai ate a posicao do pai e sobrescreve com o ponteiro do filho atualizado
  fwrite(&pai, sizeof(tipoitemarvore), 1, arquivo);
  (*acessos)++;
}

int procuraArvoreABP(int chave, FILE *arquivo,int *acessos,int* comparacoes) {//funçao faz o caminhamento para encontrar um determinado registro

  tipoitemarvore pai;
  fseek(arquivo, 0, SEEK_SET);

  while (fread(&pai, sizeof(tipoitemarvore), 1, arquivo) == 1) {
      (*acessos)++;
      (*comparacoes)++;
      if (chave == pai.chave)//encontrou 
         return chave;

      else if (chave < pai.chave) {
          (*comparacoes)++;
          if (pai.ant == -1) {//item nao existe
              return -1;
          }
          fseek(arquivo, pai.ant * sizeof(tipoitemarvore), SEEK_SET);//continua caminhamento pela esquerda
      }
      else if (chave > pai.chave) {
          (*comparacoes)++;
          if (pai.prox == -1) {//item nao existe
              return -1;
          }
          fseek(arquivo, pai.prox * sizeof(tipoitemarvore), SEEK_SET);//continua caminhamento pela direita
      }
  }
  return -1;
}