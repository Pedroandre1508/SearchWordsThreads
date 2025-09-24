/**
 * CAMADA CORE - Lógica de Negócio
 * Contém algoritmos fundamentais de busca
 */

#include "../include/caca_palavras.h"

// Função para verificar se uma posição está dentro dos limites da matriz
int posicao_valida(int x, int y, int linhas, int colunas) {
    return x >= 0 && x < linhas && y >= 0 && y < colunas;
}

// Função para buscar uma palavra em uma direção específica
int buscar_palavra_direcao(char **matriz, int linhas, int colunas, char *palavra, 
                          int start_x, int start_y, int dir_idx, int **coordenadas) {
    int len = strlen(palavra);
    int x = start_x, y = start_y;
    
    // Verificar se a palavra cabe na direção escolhida
    for (int i = 0; i < len; i++) {
        if (!posicao_valida(x, y, linhas, colunas)) {
            return 0;
        }
        
        // Comparar ignorando case (usar matriz original)
        if (tolower(matriz[x][y]) != tolower(palavra[i])) {
            return 0;
        }
        
        // Armazenar coordenadas
        coordenadas[i][0] = x;
        coordenadas[i][1] = y;
        
        // Mover para próxima posição
        x += dx[dir_idx];
        y += dy[dir_idx];
    }
    
    return 1; // Palavra encontrada
}