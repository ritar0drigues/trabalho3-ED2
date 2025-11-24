/* main.c */
#include "tad.h"
#include <time.h>

int main() {
    Grafo g;
    Estado inicio, fim;
    clock_t tempo_inicio, tempo_fim;
    double tempo_total;
    
    printf("=== TORRE DE HANOI (4 DISCOS) - LISTA DE ADJACENCIA ===\n");
    
    construirGrafoHanoi(&g);
    
    printf("Digite a configuracao INICIAL (1, 2 ou 3 para cada disco):\n");
    for(int i = 0; i < NUM_DISCOS; i++) {
        do {
            printf("Pino do Disco %d: ", i + 1);
            scanf("%d", &inicio.posicoes[i]);
            
            if (inicio.posicoes[i] < 1 || inicio.posicoes[i] > 3) {
                printf("Erro: Valor invalido! Digite apenas 1, 2 ou 3.\n");
            }
        } while (inicio.posicoes[i] < 1 || inicio.posicoes[i] > 3);
    }

    // --- BLINDAGEM DA ENTRADA FINAL ---
    printf("\nDigite a configuracao FINAL (1, 2 ou 3 para cada disco):\n");
    for(int i = 0; i < NUM_DISCOS; i++) {
        do {
            printf("Pino do Disco %d: ", i + 1);
            scanf("%d", &fim.posicoes[i]);

            if (fim.posicoes[i] < 1 || fim.posicoes[i] > 3) {
                printf("Erro: Valor invalido! Digite apenas 1, 2 ou 3.\n");
            }
        } while (fim.posicoes[i] < 1 || fim.posicoes[i] > 3);
    }
    
    int idInicio = estadoParaId(inicio);
    int idFim = estadoParaId(fim);

    // Medindo tempo
    tempo_inicio = clock();
    dijkstra(&g, idInicio, idFim);
    tempo_fim = clock();

    tempo_total = ((double)(tempo_fim - tempo_inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %.6f segundos\n", tempo_total);

    // Boa prática: limpar memória dinâmica das listas
    liberarGrafo(&g);

    return 0;
}