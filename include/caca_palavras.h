#ifndef CACA_PALAVRAS_H
#define CACA_PALAVRAS_H

// === INCLUDES PADRÃO ===
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

// === CONSTANTES GLOBAIS ===
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_LINES 1000
#define MAX_COLS 1000

// === ESTRUTURAS DE DADOS ===
typedef struct {
    int linha;
    int coluna;
    char direcao[20];
    int encontrada;
} Resultado;

typedef struct {
    char **matriz;
    int linhas;
    int colunas;
    char palavras[MAX_WORDS][MAX_WORD_LEN];
    int num_palavras;
    Resultado resultados[MAX_WORDS];
    char **matriz_original;
    int coordenadas_encontradas[MAX_WORDS][MAX_WORD_LEN][2];
    int tamanhos_palavras[MAX_WORDS];
} DadosCompartilhados;

typedef struct {
    DadosCompartilhados *dados;
    int indice_palavra;
} DadosThread;

// === VARIÁVEIS GLOBAIS ===
extern int dx[8];
extern int dy[8];
extern char *direcoes[8];
extern pthread_mutex_t mutex_resultados;

// === logica de busca ===
int posicao_valida(int x, int y, int linhas, int colunas);
int buscar_palavra_direcao(char **matriz, int linhas, int colunas, char *palavra, 
                          int start_x, int start_y, int dir_idx, int **coordenadas);

// === Threads ===
void* buscar_palavra_thread(void* arg);
void inicializar_threading();
void finalizar_threading();

// Manipulação de Arquivos ===
int ler_arquivo_entrada(char *nome_arquivo, DadosCompartilhados *dados);
void escrever_arquivo_saida(char *nome_arquivo, DadosCompartilhados *dados);

// === Utilitários ===
void marcar_palavras_encontradas(DadosCompartilhados *dados);
void liberar_memoria(DadosCompartilhados *dados);
void exibir_resultados(DadosCompartilhados *dados);

#endif // CACA_PALAVRAS_H