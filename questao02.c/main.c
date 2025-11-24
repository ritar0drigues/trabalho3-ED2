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
    
    printf("Digite a configuracao INICIAL (ex: 1 1 1 1):\n");
    for(int i = 0; i < NUM_DISCOS; i++) {
        scanf("%d", &inicio.posicoes[i]);
    }

    printf("Digite a configuracao FINAL (ex: 3 3 3 3):\n");
     for(int i = 0; i < NUM_DISCOS; i++) {
        scanf("%d", &fim.posicoes[i]);
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