/**
 * CAMADA THREADING - Gerenciamento de Threads
 * Responsável por paralelização e sincronização
 */

#include "../../include/caca_palavras.h"

// Direções de busca: 8 direções possíveis
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
char *direcoes[] = {"esquerda/cima", "cima", "direita/cima", "esquerda", "direita", "esquerda/baixo", "baixo", "direita/baixo"};

// Mutex para sincronização
pthread_mutex_t mutex_resultados = PTHREAD_MUTEX_INITIALIZER;

// Função executada por cada thread para buscar uma palavra específica
void* buscar_palavra_thread(void* arg) {
    DadosThread *dados_thread = (DadosThread*)arg;
    DadosCompartilhados *dados = dados_thread->dados;
    int idx = dados_thread->indice_palavra;
    char *palavra = dados->palavras[idx];
    
    printf("[THREAD-%d] Iniciada para buscar palavra: %s\n", idx, palavra);
    
    // Alocar coordenadas temporárias dinamicamente para evitar stack overflow
    int **coordenadas_temp = malloc(MAX_WORD_LEN * sizeof(int*));
    if (!coordenadas_temp) {
        printf("[ERRO] Falha ao alocar memória para coordenadas\n");
        pthread_exit(NULL);
    }
    
    for (int i = 0; i < MAX_WORD_LEN; i++) {
        coordenadas_temp[i] = malloc(2 * sizeof(int));
        if (!coordenadas_temp[i]) {
            printf("[ERRO] Falha ao alocar coordenada %d\n", i);
            // Liberar memória já alocada
            for (int j = 0; j < i; j++) {
                free(coordenadas_temp[j]);
            }
            free(coordenadas_temp);
            pthread_exit(NULL);
        }
    }
    
    // Buscar em todas as posições e direções
    for (int i = 0; i < dados->linhas; i++) {
        for (int j = 0; j < dados->colunas; j++) {
            for (int dir = 0; dir < 8; dir++) {
                
                if (buscar_palavra_direcao(dados->matriz_original, dados->linhas, dados->colunas, 
                                         palavra, i, j, dir, coordenadas_temp)) {
                    
                    // Usa mutex para atualizar resultado
                    pthread_mutex_lock(&mutex_resultados);
                    
                    if (!dados->resultados[idx].encontrada) {
                        dados->resultados[idx].linha = i;
                        dados->resultados[idx].coluna = j;
                        strcpy(dados->resultados[idx].direcao, direcoes[dir]);
                        dados->resultados[idx].encontrada = 1;
                        dados->tamanhos_palavras[idx] = strlen(palavra);
                        
                        // Armazenar coordenadas para marcação posterior
                        int len_palavra = strlen(palavra);
                        for (int k = 0; k < len_palavra; k++) {
                            dados->coordenadas_encontradas[idx][k][0] = coordenadas_temp[k][0];
                            dados->coordenadas_encontradas[idx][k][1] = coordenadas_temp[k][1];
                        }
                        
                        printf("[SUCESSO] '%s' encontrada em (%d,%d) direção %s\n", 
                               palavra, i+1, j+1, direcoes[dir]);
                    }
                    
                    pthread_mutex_unlock(&mutex_resultados);
                    
                    // Liberar memória antes de sair
                    for (int k = 0; k < MAX_WORD_LEN; k++) {
                        free(coordenadas_temp[k]);
                    }
                    free(coordenadas_temp);
                    
                    pthread_exit(NULL); // Thread finalizada após encontrar palavra
                }
            }
        }
    }
    
    // Liberar memória se não encontrou
    for (int k = 0; k < MAX_WORD_LEN; k++) {
        free(coordenadas_temp[k]);
    }
    free(coordenadas_temp);
    
    printf("[THREAD-%d] Palavra '%s' não encontrada\n", idx, palavra);
    pthread_exit(NULL);
}

// Inicializar sistema de threading
void inicializar_threading() {
    pthread_mutex_init(&mutex_resultados, NULL);
}

// Finalizar sistema de threading
void finalizar_threading() {
    pthread_mutex_destroy(&mutex_resultados);
}