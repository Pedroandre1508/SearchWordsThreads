/**
 * CAMADA UTILS - Utilitários e Formatação
 * Funções auxiliares, formatação e gerenciamento de memória
 */

#include "../include/caca_palavras.h"

// Função para marcar palavras encontradas com formatação especial
void marcar_palavras_encontradas(DadosCompartilhados *dados) {
    printf("[UTILS] Aplicando formatação às palavras encontradas...\n");
    
    // Criar nova matriz expandida para asteriscos
    char **nova_matriz = malloc(dados->linhas * sizeof(char*));
    if (!nova_matriz) {
        printf("[ERRO] Falha ao alocar memória para formatação\n");
        return;
    }
    
    // Calcular expansão necessária
    int max_expansao = dados->num_palavras * 4; // Margem de segurança
    
    for (int i = 0; i < dados->linhas; i++) {
        nova_matriz[i] = malloc((dados->colunas + max_expansao + 1) * sizeof(char));
        if (!nova_matriz[i]) {
            printf("[ERRO] Falha ao alocar linha %d para formatação\n", i);
            return;
        }
        strcpy(nova_matriz[i], dados->matriz[i]);
    }
    
    // Aplicar formatação por tipo de direção
    for (int palavra_idx = 0; palavra_idx < dados->num_palavras; palavra_idx++) {
        if (!dados->resultados[palavra_idx].encontrada) continue;
        
        int tam_palavra = dados->tamanhos_palavras[palavra_idx];
        int primeira_linha = dados->coordenadas_encontradas[palavra_idx][0][0];
        int primeira_coluna = dados->coordenadas_encontradas[palavra_idx][0][1];
        int ultima_linha = dados->coordenadas_encontradas[palavra_idx][tam_palavra-1][0];
        int ultima_coluna = dados->coordenadas_encontradas[palavra_idx][tam_palavra-1][1];
        
        // HORIZONTAL
        if (primeira_linha == ultima_linha) {
            int linha = primeira_linha;
            int col_inicio = (primeira_coluna < ultima_coluna) ? primeira_coluna : ultima_coluna;
            int col_fim = (primeira_coluna > ultima_coluna) ? primeira_coluna : ultima_coluna;
            
            char nova_linha[MAX_COLS * 2];
            int pos = 0;
            
            // Antes da palavra
            for (int j = 0; j < col_inicio; j++) {
                nova_linha[pos++] = dados->matriz_original[linha][j];
            }
            
            // *PALAVRA*
            nova_linha[pos++] = '*';
            for (int j = col_inicio; j <= col_fim; j++) {
                nova_linha[pos++] = toupper(dados->matriz_original[linha][j]);
            }
            nova_linha[pos++] = '*';
            
            // Resto da linha
            for (int j = col_fim + 1; j < dados->colunas; j++) {
                nova_linha[pos++] = dados->matriz_original[linha][j];
            }
            
            nova_linha[pos] = '\0';
            strcpy(nova_matriz[linha], nova_linha);
        }
        // FORMATAÇÃO VERTICAL
        else if (primeira_coluna == ultima_coluna) {
            for (int j = 0; j < tam_palavra; j++) {
                int x = dados->coordenadas_encontradas[palavra_idx][j][0];
                int y = dados->coordenadas_encontradas[palavra_idx][j][1];
                if (x >= 0 && x < dados->linhas && y >= 0 && y < dados->colunas) {
                    nova_matriz[x][y] = toupper(dados->matriz_original[x][y]);
                }
            }
        }
        // DIAGONAL
        else {
            for (int j = 0; j < tam_palavra; j++) {
                int x = dados->coordenadas_encontradas[palavra_idx][j][0];
                int y = dados->coordenadas_encontradas[palavra_idx][j][1];
                
                if (x >= 0 && x < dados->linhas && y >= 0 && y < dados->colunas) {
                    char nova_linha[MAX_COLS * 2];
                    int pos = 0;
                    
                    // Antes da letra
                    for (int k = 0; k < y; k++) {
                        nova_linha[pos++] = nova_matriz[x][k];
                    }
                    
                    // *LETRA*
                    nova_linha[pos++] = '*';
                    nova_linha[pos++] = toupper(dados->matriz_original[x][y]);
                    nova_linha[pos++] = '*';
                    
                    // Resto da linha
                    int len_atual = strlen(nova_matriz[x]);
                    for (int k = y + 1; k < len_atual; k++) {
                        nova_linha[pos++] = nova_matriz[x][k];
                    }
                    
                    nova_linha[pos] = '\0';
                    
                    // Realocar se necessário
                    if (strlen(nova_linha) > strlen(nova_matriz[x])) {
                        free(nova_matriz[x]);
                        nova_matriz[x] = malloc((strlen(nova_linha) + 1) * sizeof(char));
                        if (!nova_matriz[x]) continue;
                    }
                    
                    strcpy(nova_matriz[x], nova_linha);
                }
            }
        }
    }
    
    // Atualizar matriz principal
    for (int i = 0; i < dados->linhas; i++) {
        free(dados->matriz[i]);
        dados->matriz[i] = nova_matriz[i];
    }
    free(nova_matriz);
    
    printf("[UTILS] Formatação aplicada com sucesso!\n");
}

// Função para exibir resultados no terminal
void exibir_resultados(DadosCompartilhados *dados) {
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║         RESULTADOS DA BUSCA          ║\n");
    printf("╠══════════════════════════════════════╣\n");
    
    int encontradas = 0;
    for (int i = 0; i < dados->num_palavras; i++) {
        if (dados->resultados[i].encontrada) {
            printf("║ ✓ %-15s (%2d,%2d):%-12s ║\n", 
                   dados->palavras[i],
                   dados->resultados[i].linha+1,
                   dados->resultados[i].coluna+1,
                   dados->resultados[i].direcao);
            encontradas++;
        } else {
            printf("║ ✗ %-15s NÃO ENCONTRADA      ║\n", dados->palavras[i]);
        }
    }
    
    printf("╠══════════════════════════════════════╣\n");
    printf("║ Total: %d/%d palavras encontradas     ║\n", encontradas, dados->num_palavras);
    printf("╚══════════════════════════════════════╝\n");
}

// Função para liberar toda a memória alocada
void liberar_memoria(DadosCompartilhados *dados) {
    printf("[UTILS] Liberando memória alocada...\n");
    
    if (dados->matriz) {
        for (int i = 0; i < dados->linhas; i++) {
            if (dados->matriz[i]) free(dados->matriz[i]);
        }
        free(dados->matriz);
    }
    
    if (dados->matriz_original) {
        for (int i = 0; i < dados->linhas; i++) {
            if (dados->matriz_original[i]) free(dados->matriz_original[i]);
        }
        free(dados->matriz_original);
    }
    
    printf("[UTILS] Memória liberada com sucesso!\n");
}