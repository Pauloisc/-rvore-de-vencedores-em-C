//
// Created by Vanessa Braganholo.
// Updated by Raphael Oliveira.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "intercalacao.h"
#include "nomes.h"
#include "cliente.h"
#include <limits.h>
#include <string.h>

typedef struct vetor{
    TCliente *cli;
    FILE *f;
}TVet;

struct Arvore {
   int info;
   struct Arvore *esq;
   struct Arvore *dir;
}

/*
 * Definicao de tipo para no de arvore de vencedores
 */
typedef struct No {
    TCliente *vencedor;
    struct No *endVencedor;
    FILE *f;
    struct No *pai;
    struct No *dir;
    struct No *esq;
} TNo;

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Nomes *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de saida\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro cliente do arquivo no campo cli do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            if (v[i].f != NULL) {
                TCliente *c = le_cliente(v[i].f);
                if (c == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posicao do vetor
                    v[i].cli = cliente(INT_MAX, "");
                }
                else {
                    //conseguiu ler cliente, coloca na posicao atual do vetor
                    v[i].cli = c;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o cliente com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].cli->cod_cliente < menor){
                    menor = v[i].cli->cod_cliente;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            } else {
                //salva cliente no arquivo de saida
                salva_cliente(v[pos_menor].cli, out);
                //atualiza posicao pos_menor do vetor com proximo cliente do arquivo
                TCliente *c = le_cliente(v[pos_menor].f);
                if (c == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posicao do vetor
                    v[pos_menor].cli = cliente(INT_MAX, "");
                }
                else {
                    v[pos_menor].cli = c;
                }

            }
        }

        //fecha arquivos das particoes de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        }
        //fecha arquivo de saida
        fclose(out);
    }
}

void intercalacao_arv_vencedores(char *nome_arquivo_saida, int num_p, Nomes *nome_particoes){
    Arvore *vetorFolhas [num_p];
    for (int i = 0, i < num_p; i++){
        vetorFolhas[i] = (Arvore*)malloc(sizeof(Arvore));
    }
}

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Nomes *nome_particoes, int f) {
    //TODO: Implementar essa função
}
