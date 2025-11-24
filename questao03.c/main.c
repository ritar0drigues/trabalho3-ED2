#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planilha.h"
#include "Busca/bfs.h"
#include "Busca/dfs.h"
#include <time.h>

int main() {
    GrafoPlanilha planilha;
    inicializarPlanilha(&planilha);

    char entrada[150];
    char coord[10];
    char valor[100];

    printf("=== Planilha Rudimentar com Grafo ===\n");
    printf("Comandos:\n");
    printf(" - Atribuir valor: A1 10\n");
    printf(" - Referencia:     B1 =A1 (ou +A1)\n");
    printf(" - Funcao:         C1 @soma(A1..A5)\n");
    printf(" - Ver Grafo:      grafo\n");
    printf(" - Buscar:         buscar A1   (usa busca selecionada)\n");
    printf(" - Exemplo Auto:   demo\n");
    printf(" - Sair:           sair\n\n");

    /* escolha do tipo de busca no inicio */
    int buscaModo = 3; /* 1=bfs,2=dfs,3=nenhuma */
    char buf[16];
    printf("Escolha tipo de busca (1=BFS, 2=DFS, 3=Nenhuma) [3]: ");
    if (fgets(buf, sizeof(buf), stdin)) {
        int opt = atoi(buf);
        if (opt == 1 || opt == 2 || opt == 3) buscaModo = opt;
    }
    if (buscaModo == 1) printf("BFS selecionada.\n");
    else if (buscaModo == 2) printf("DFS selecionada.\n");
    else printf("Nenhuma busca selecionada.\n");

    exibirPlanilha(&planilha);

    while (1) {
        printf("Digite o comando (celula valor): ");
        if (!fgets(entrada, sizeof(entrada), stdin)) break;

        // Remove newline
        entrada[strcspn(entrada, "\n")] = 0;

        if (strcmp(entrada, "sair") == 0) break;

        if (strcmp(entrada, "grafo") == 0) {
            exibirGrafo(&planilha);
            continue;
        }

        if (strcmp(entrada, "demo") == 0) {
            printf("\n--- Executando Exemplo do Enunciado ---\n");
            processarEntrada(&planilha, "A1", "10");
            processarEntrada(&planilha, "B1", "20");
            processarEntrada(&planilha, "A2", "30");
            printf(">> D1 recebe +A1\n");
            processarEntrada(&planilha, "D1", "+A1");
            exibirGrafo(&planilha); // Mostra grafo intermediário
            
            printf(">> C1 recebe @soma(A1..B2)\n");
            processarEntrada(&planilha, "C1", "@soma(A1..B2)");
            
            printf(">> D1 atualizado para +C1 (Remove aresta D1->A1, adiciona D1->C1)\n");
            processarEntrada(&planilha, "D1", "+C1");
            
            exibirPlanilha(&planilha);
            exibirGrafo(&planilha);
            continue;
        }

        /* comando de busca: "buscar A1" */
        if (strncmp(entrada, "buscar", 6) == 0) {
            char *arg = entrada + 6;
            while (*arg == ' ') arg++;
            if (*arg == '\0') {
                printf("Uso: buscar <coord>\n");
                continue;
            }
            int idxStart = obterIndice(arg);
            if (idxStart == -1) {
                printf("Coordenada invalida.\n");
                continue;
            }
            if (buscaModo == 3) {
                printf("Nenhuma busca selecionada no inicio. Reexecute e escolha BFS ou DFS.\n");
                continue;
            }

            int visited[MAX_CELULAS];
            struct timespec tstart, tend;
            clock_gettime(CLOCK_MONOTONIC, &tstart);

            if (buscaModo == 1) {
                bfs_mark_matrix(idxStart, MAX_CELULAS, planilha.matrizAdj, visited);
            } else {
                dfs_mark_matrix(idxStart, MAX_CELULAS, planilha.matrizAdj, visited);
            }

            clock_gettime(CLOCK_MONOTONIC, &tend);
            double elapsed_ms = (tend.tv_sec - tstart.tv_sec) * 1000.0
                                + (tend.tv_nsec - tstart.tv_nsec) / 1e6;

            printf("Tempo da busca: %.8f ms\n", elapsed_ms);

            printf("A partir de %s alcancaveis:\n", arg);
            int found = 0;
            char nome[10];
            for (int i = 0; i < MAX_CELULAS; ++i) {
                if (visited[i]) {
                    obterNomeCoord(i, nome);
                    printf(" - %s\n", nome);
                    found = 1;
                }
            }
            if (!found) printf(" Nenhum vertice alcancavel.\n");

            continue;
        }

        /* Parse simples da string de entrada para edição */
        char *token = strtok(entrada, " ");
        if (token != NULL) {
            strcpy(coord, token);
            token = strtok(NULL, ""); // Pega o resto da linha
            
            if (token != NULL) {
                // Remove espaços extras do inicio do valor se houver
                while(*token == ' ') token++;
                strcpy(valor, token);

                processarEntrada(&planilha, coord, valor);
                exibirPlanilha(&planilha);

                /* medir e mostrar tempo da busca a partir da célula editada, se ativada */
                if (buscaModo != 3) {
                    int idxEdited = obterIndice(coord);
                    if (idxEdited != -1) {
                        int visited[MAX_CELULAS];
                        struct timespec tstart, tend;
                        clock_gettime(CLOCK_MONOTONIC, &tstart);
                        if (buscaModo == 1)
                            bfs_mark_matrix(idxEdited, MAX_CELULAS, planilha.matrizAdj, visited);
                        else
                            dfs_mark_matrix(idxEdited, MAX_CELULAS, planilha.matrizAdj, visited);
                        clock_gettime(CLOCK_MONOTONIC, &tend);
                        double elapsed_ms = (tend.tv_sec - tstart.tv_sec) * 1000.0
                                            + (tend.tv_nsec - tstart.tv_nsec) / 1e6;
                        int count = 0;
                        for (int i = 0; i < MAX_CELULAS; ++i) if (visited[i]) ++count;
                        printf("Tempo da busca apos alteracao em %s: %.8f ms — alcancaveis: %d\n", coord, elapsed_ms, count);
                    }
                }

            } else {
                printf("Formato inválido. Use: CELULA VALOR\n");
            }
        }
    }

    return 0;
}