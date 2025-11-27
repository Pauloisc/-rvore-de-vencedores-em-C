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
    //Criar folhas
    TNo **vetorFolhas = (TNo**)malloc(num_p * sizeof(TNo*)); // Aloca um vetor de ponteiros dinamicamente, guardará as primeiras folhas com o primeiro registro 
    Nomes *atual = nome_particoes;
    for (int i = 0; i < num_p; i++){
        vetorFolhas[i] = (TNo*)malloc(sizeof(TNo));
        vetorFolhas[i]->f = fopen(atual->nome,"rb");
        TCliente *c = le_cliente(vetorFolhas[i] -> f);
        if (c == NULL){
            c = cliente (INT_MAX, " ");
            vetorFolhas[i]->vencedor = c;
        }
        else{
            vetorFolhas[i]->vencedor = c;
        }
        vetorFolhas[i]->esq = NULL;
        vetorFolhas[i]->dir = NULL;
        vetorFolhas[i]->endVencedor = NULL;
        vetorFolhas[i]->pai = NULL;
        atual = atual->prox;
    }
    //Construir arvore  
    TNo **nivelAtual = VetorFolhas;
    int cont = 0;

    while (num_p != 1){
        if (num_p%2==0){
            TNo **nivelArvore = (TNo**)malloc((num_p/2) * sizeof(TNo*)); // Aloca um vetor de ponteiros dinamicamente, servirá como cada nível da arvore
            for (int i = 0; i < num_p; i+=2){
                TNo *noPai = (TNo*)malloc(sizeof(TNo));
                noPai -> f = NULL;
                noPai -> pai = NULL;
                noPai->esq = vetorFolhas[i];
                noPai->dir = vetorFolhas[i+1];
                vetorFolhas[i]->pai = noPai;
                vetorFolhas[i+1]->pai = noPai;
                if (vetorFolhas[i]->vencedor->cod < vetorFolhas[i+1]->vencedor->cod){ // Aqui há a definição do vencedor durante a conexão das folhas
                    noPai->vencedor = vetorFolhas[i]->vencedor;
                    noPai->endVencedor = vetorFolhas[i];
                }
                else{
                    noPai->vencedor = vetorFolhas[i+1]->vencedor;
                    noPai->endVencedor = vetorFolhas[i+1];
                }
                nivelArvore[cont] = noPai
                cont++;
            }
            num_p = num_p/2;
        }
        else{
            TNo **nivelArvore = (TNo**)malloc(((num_p + 1)/2) * sizeof(TNo*)); // Aloca um vetor de ponteiros dinamicamente, servirá como cada nível da arvore
            for (int i = 0; i < num_p-1; i+=2){
                TNo *noPai = (TNo*)malloc(sizeof(TNo));
                noPai -> f = NULL;
                noPai -> pai = NULL;
                noPai->esq = vetorFolhas[i];
                noPai->dir = vetorFolhas[i+1];
                vetorFolhas[i]->pai = noPai;
                vetorFolhas[i+1]->pai = noPai;
                if (vetorFolhas[i]->vencedor->cod < vetorFolhas[i+1]->vencedor->cod){ // Aqui há a definição do vencedor durante a conexão das folhas
                    noPai->vencedor = vetorFolhas[i]->vencedor;
                    noPai->endVencedor = vetorFolhas[i];
                }
                else{
                    noPai->vencedor = vetorFolhas[i+1]->vencedor;
                    noPai->endVencedor = vetorFolhas[i+1];
                }
                nivelArvore[cont] = noPai
                cont++;
            }
            nivelArvore[cont] = vetorFolhas[num_p - 1];
            num_p = num_p/2+1;
        }
        cont = 0
        vetorFolhas = nivelArvore;
    }

    TNo *raiz = vetorFolhas[0];  // Cria a raiz que pega justamente a ultima folha criada, que é a raiz
    //intercalçao
    File *saida = fopen(nome_arquivo_saida, "wb");//Precisa abrir o arquivo que recebrá tudo
    if(saida==NULL){
        printf("Erro");
        return;    
            }
    
    while (raiz->vencedor->cod != INT_MAX) {
        salva_cliente(raiz->vencedor, out); //salva vencedor
        TNo *folhaVencedora = noPai -> endVencedor; //mais facil de encontrar o endvencedor
        free(folhaVencedora -> vencedora;
        TCliente *proximo = le_cliente(folhaVencedora -> f);
        if(proximo == NULL){
            folhaVencedora -> vencedor = Cliente(INT_MAX, "");
        }
        else{
            folhaVencedora -> vencedor
        
    }
}

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Nomes *nome_particoes, int f) {
    //TODO: Implementar essa função
}
