# Search Words Threads

## Descrição
Este programa implementa um caça-palavras que utiliza threads para buscar palavras em paralelo em uma matriz de caracteres.

## Estratégia de Paralelização

### Threads por Palavra
- **Cada palavra é buscada por uma thread independente**
- **Cada thread percorre toda a matriz em todas as 8 direções**
- **Primeira ocorrência encontrada é registrada**

### Sincronização
- **Mutex `mutex_resultados`**: Protege o acesso aos resultados compartilhados
- **Preservação da matriz original**: Cópia separada para evitar interferência entre threads
- **Marcação posterior**: Coordenadas são armazenadas e marcação em maiúsculas ocorre ao final

### Direções de Busca
1. esquerda/cima (diagonal)
2. cima (vertical)
3. direita/cima (diagonal)
4. esquerda (horizontal)
5. direita (horizontal)
6. esquerda/baixo (diagonal)
7. baixo (vertical)
8. direita/baixo (diagonal)

## Compilação e Execução

### Execução
Faz a criação do arquivo a ser executado
```bash
gcc -pthread -Wall -Wextra -std=c99 -O2 -Iinclude -o caca_palavras main.c src/core/busca_algoritmos.c src/threading/thread_manager.c src/io/arquivo_manager.c src/utils/formatador.c
```
Executa o programa informando qual o arquivo de entrada txt e onde deve armazenar o de saida.
```bash
./caca_palavras data/entrada.txt data/saida.txt
```

## Formato do Arquivo de Entrada
```
<colunas> <linhas>
<linha1_da_matriz>
<linha2_da_matriz>
...
<palavra1>
<palavra2>
...
```

## Formato do Arquivo de Saída
```
<matriz_com_palavras_em_maiusculas>
<palavra1> – (<linha>,<coluna>):<direção>
<palavra2>: não encontrada (Caso não tenha sido encontrada)
...
```

## Exemplo de Uso

### Arquivo de Entrada (entrada_exemplo.txt)
```
60 16
runkffxscslvqwvtvfolwymmxprocessoslkqoifheurjzqbdlpybntwiblv
xjhjguvykdfrjzltbbweoldpyiymizzdyinjqjfoukvluuypvancozhgzfuk
...
threads
arquivos
sinais
pipe
processos
mutex
```

### Saída no Terminal
```
Dimensões lidas: 60 colunas x 16 linhas
Lidas 6 palavras para buscar
Iniciando busca with 6 threads...
Thread iniciada para buscar palavra: threads
Palavra 'threads' encontrada na posição (3,5) direção baixo
...
Busca concluída!
```

## Arquitetura do Código

### Estruturas Principais
- **`DadosCompartilhados`**: Dados compartilhados entre todas as threads
- **`DadosThread`**: Dados específicos de cada thread
- **`Resultado`**: Informações sobre palavras encontradas

### Funções Principais
- **`buscar_palavra_thread()`**: Função executada por cada thread
- **`buscar_palavra_direcao()`**: Busca palavra em direção específica
- **`ler_arquivo_entrada()`**: Processa arquivo de entrada
- **`escrever_arquivo_saida()`**: Gera arquivo de saída
- **`marcar_palavras_encontradas()`**: Converte letras para maiúsculas

## Considerações Técnicas

### Tratamento de Maiúsculas/Minúsculas
- **Busca insensível a caso**: Usa `tolower()` para comparação
- **Preservação original**: Matriz original separada durante busca
- **Marcação final**: Conversão para maiúsculas apenas no resultado final

### Evitar Interferência entre Threads
- **Cópia da matriz**: `matriz_original` preserva estado inicial
- **Coordenadas armazenadas**: Posições salvas para marcação posterior
- **Primeira ocorrência**: Thread para quando encontra primeira vez

## Autores
- Pedro André e Rafael
- Desenvolvido projeto para disciplina de Sistemas Operacionais.
