/* main.c */
#include "tad.h"
#include <time.h> // <--- BIBLIOTECA NECESSÁRIA

int main() {
    Grafo g;
    Estado inicio, fim;
    clock_t tempo_inicio, tempo_fim; // Variáveis para armazenar os ticks do clock
    double tempo_total; // Variável para o tempo em segundos
    
    printf("=== TORRE DE HANOI (4 DISCOS) - GRAFO & DIJKSTRA ===\n");
    printf("Legenda do vetor: [Disco Pequeno, Disco Medio, ..., Disco Grande]\n");
    printf("Valores permitidos para os pinos: 1, 2 ou 3\n\n");

    // Construindo o grafo
    printf("Construindo o grafo de estados...\n");
    construirGrafoHanoi(&g);
    printf("Grafo construido com sucesso! (%d vertices)\n\n", NUM_ESTADOS);

// Leitura da configuração inicial
    printf("Digite a configuracao INICIAL dos 4 discos:\n");
    for(int i = 0; i < NUM_DISCOS; i++) {
        do {
            printf("Pino do Disco %d (1-3): ", i+1);
            scanf("%d", &inicio.posicoes[i]);
            
            // Adicionando aviso de erro
            if (inicio.posicoes[i] < 1 || inicio.posicoes[i] > 3) {
                printf("Entrada invalida! O pino deve ser 1, 2 ou 3.\n");
            }
        } while(inicio.posicoes[i] < 1 || inicio.posicoes[i] > 3);
    }

    // Leitura da configuração final
    printf("\nDigite a configuracao FINAL desejada:\n");
     for(int i = 0; i < NUM_DISCOS; i++) {
        do {
            printf("Pino do Disco %d (1-3): ", i+1);
            scanf("%d", &fim.posicoes[i]);
            
            // Adicionando aviso de erro
            if (fim.posicoes[i] < 1 || fim.posicoes[i] > 3) {
                printf(">> Entrada invalida! O pino deve ser 1, 2 ou 3.\n");
            }
        } while(fim.posicoes[i] < 1 || fim.posicoes[i] > 3);
    }

    int idInicio = estadoParaId(inicio);
    int idFim = estadoParaId(fim);

    printf("\nIniciando busca pelo menor caminho...\n");

    // --- INÍCIO DA CONTAGEM DO TEMPO ---
    tempo_inicio = clock();

    dijkstra(&g, idInicio, idFim);

    tempo_fim = clock();
    // --- FIM DA CONTAGEM DO TEMPO ---

    // Cálculo em segundos
    tempo_total = ((double)(tempo_fim - tempo_inicio)) / CLOCKS_PER_SEC;

    printf("\n------------------------------------------------\n");
    printf("Tempo gasto para encontrar a solucao: %.6f segundos\n", tempo_total);
    printf("------------------------------------------------\n");

    return 0;
}