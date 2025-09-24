/**
 * CAMADA IO - Entrada e Saída de Dados
 * Gerencia leitura e escrita de arquivos
 */

#include "../include/caca_palavras.h"

// Função para ler arquivo de entrada
int ler_arquivo_entrada(char *nome_arquivo, DadosCompartilhados *dados) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("[ERRO] Não foi possível abrir o arquivo: %s\n", nome_arquivo);
        return 0;
    }
    
    printf("[IO] Lendo arquivo: %s\n", nome_arquivo);
    
    // Ler dimensões
    if (fscanf(arquivo, "%d %d", &dados->colunas, &dados->linhas) != 2) {
        printf("[ERRO] Formato inválido - dimensões não encontradas\n");
        fclose(arquivo);
        return 0;
    }
    
    printf("[IO] Dimensões: %d colunas x %d linhas\n", dados->colunas, dados->linhas);
    
    // Validar dimensões
    if (dados->linhas <= 0 || dados->colunas <= 0 || 
        dados->linhas > MAX_LINES || dados->colunas > MAX_COLS) {
        printf("[ERRO] Dimensões inválidas (%d x %d)\n", dados->colunas, dados->linhas);
        fclose(arquivo);
        return 0;
    }
    
    // Alocar matrizes
    dados->matriz = malloc(dados->linhas * sizeof(char*));
    dados->matriz_original = malloc(dados->linhas * sizeof(char*));
    
    if (!dados->matriz || !dados->matriz_original) {
        printf("[ERRO] Falha na alocação de memória para matrizes\n");
        fclose(arquivo);
        return 0;
    }
    
    // Alocar linhas das matrizes
    for (int i = 0; i < dados->linhas; i++) {
        dados->matriz[i] = malloc((dados->colunas + 1) * sizeof(char));
        dados->matriz_original[i] = malloc((dados->colunas + 1) * sizeof(char));
        
        if (!dados->matriz[i] || !dados->matriz_original[i]) {
            printf("[ERRO] Falha na alocação da linha %d\n", i);
            fclose(arquivo);
            return 0;
        }
    }
    
    // Ler matriz de caracteres
    for (int i = 0; i < dados->linhas; i++) {
        if (fscanf(arquivo, "%s", dados->matriz[i]) != 1) {
            printf("[ERRO] Falha ao ler linha %d da matriz\n", i);
            fclose(arquivo);
            return 0;
        }
        
        strcpy(dados->matriz_original[i], dados->matriz[i]); // Backup original
        printf("[IO] Linha %d: %ld caracteres\n", i+1, strlen(dados->matriz[i]));
    }
    
    // Ler palavras a serem buscadas
    dados->num_palavras = 0;
    while (fscanf(arquivo, "%s", dados->palavras[dados->num_palavras]) == 1) {
        dados->resultados[dados->num_palavras].encontrada = 0;
        dados->num_palavras++;
        if (dados->num_palavras >= MAX_WORDS) break;
    }
    
    printf("[IO] Total de palavras para buscar: %d\n", dados->num_palavras);
    
    fclose(arquivo);
    return 1;
}

// Função para escrever arquivo de saída
void escrever_arquivo_saida(char *nome_arquivo, DadosCompartilhados *dados) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("[ERRO] Não foi possível criar arquivo de saída: %s\n", nome_arquivo);
        return;
    }
    
    printf("[IO] Gerando arquivo de saída: %s\n", nome_arquivo);
    
    // Marcar palavras encontradas
    marcar_palavras_encontradas(dados);
    
    // Escrever matriz com palavras marcadas
    for (int i = 0; i < dados->linhas; i++) {
        fprintf(arquivo, "%s\n", dados->matriz[i]);
    }
    
    // Escrever resultados das palavras
    fprintf(arquivo, "\n=== RESULTADOS DA BUSCA ===\n");
    for (int i = 0; i < dados->num_palavras; i++) {
        if (dados->resultados[i].encontrada) {
            fprintf(arquivo, "%s – (%d,%d):%s\n", 
                   dados->palavras[i],
                   dados->resultados[i].linha+1,
                   dados->resultados[i].coluna+1,
                   dados->resultados[i].direcao);
        } else {
            fprintf(arquivo, "%s: não encontrada\n", dados->palavras[i]);
        }
    }
    
    fclose(arquivo);
    printf("[IO] Arquivo de saída criado com sucesso!\n");
}