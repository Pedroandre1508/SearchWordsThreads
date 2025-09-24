/**
 * Caça Palavras
 * @authors: Pedro André e Rafael
 * @course: Sistemas Operacionais
 * @compile: gcc -pthread -o caca_palavras main.c src/core/busca_algoritmos.c src/threading/thread_manager.c src/io/arquivo_manager.c src/utils/formatador.c -Iinclude
 */

#include "include/caca_palavras.h"

int main(int argc, char *argv[]) {
    // ═══ VALIDAÇÃO DE ARGUMENTOS ═══
    if (argc != 3) {
        printf("╔════════════════════════════════════════╗\n");
        printf("║          CAÇA-PALAVRAS PARALELO        ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║  Uso: %s <entrada> <saida>   ║\n", argv[0]);
        printf("║                                        ║\n");
        printf("║  Exemplo:                              ║\n");
        printf("║  ./caca_palavras entrada.txt saida.txt║\n");
        printf("╚════════════════════════════════════════╝\n");
        return 1;
    }
    
    printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    Iniciando processamento...                         ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════╝\n\n");
    
    // ═══ INICIALIZAÇÃO ═══
    DadosCompartilhados dados;
    memset(&dados, 0, sizeof(DadosCompartilhados));
    
    inicializar_threading();
    
    // ═══ CARREGAMENTO DE DADOS ═══
    printf("┌─ CARREGAMENTO DE DADOS ────────────────────────────────────────┐\n");
    if (!ler_arquivo_entrada(argv[1], &dados)) {
        printf("└─ ERRO: Falha no carregamento dos dados ───────────────────────────────┘\n");
        return 1;
    }
    printf("└─ Dados carregados com sucesso ─────────────────────────────────────────┘\n\n");
    
    // Verificar se há palavras para buscar
    if (dados.num_palavras == 0) {
        printf("[ERRO] Nenhuma palavra para buscar encontrada no arquivo\n");
        liberar_memoria(&dados);
        return 1;
    }
    
    // ═══ BUSCA PARALELA ═══
    printf("┌─ BUSCA PARALELA ───────────────────────────────────────────────┐\n");
    printf("│ Criando %d threads...\n", dados.num_palavras);
    
    pthread_t threads[MAX_WORDS];
    DadosThread dados_threads[MAX_WORDS];
    
    // Criar threads
    for (int i = 0; i < dados.num_palavras; i++) {
        dados_threads[i].dados = &dados;
        dados_threads[i].indice_palavra = i;
        
        if (pthread_create(&threads[i], NULL, buscar_palavra_thread, &dados_threads[i]) != 0) {
            printf("│ [ERRO] Falha ao criar thread %d\n", i);
            liberar_memoria(&dados);
            finalizar_threading();
            return 1;
        }
    }
    
    // Aguardar conclusão de todas as threads
    for (int i = 0; i < dados.num_palavras; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("└─ Busca paralela concluída ─────────────────────────────────────────────┘\n\n");
    
    // ═══ GERAÇÃO DE RESULTADOS ═══
    printf("┌─ GERAÇÃO DE RESULTADOS ────────────────────────────────────────┐\n");
    escrever_arquivo_saida(argv[2], &dados);
    printf("└─ Arquivo de saída gerado ──────────────────────────────────────────────┘\n");
    
    // ═══ EXIBIÇÃO DOS RESULTADOS ═══
    exibir_resultados(&dados);
    
    // ═══ FINAL ═══
    printf("\n┌─ FINALIZANDO ───────────────────────────────────────────────────────────┐\n");
    liberar_memoria(&dados);
    finalizar_threading();
    printf("    Pedro André e Rafael \n");

    printf("└─ Programa finalizado com sucesso ──────────────────────────────────────┘\n");

    
    return 0;
}