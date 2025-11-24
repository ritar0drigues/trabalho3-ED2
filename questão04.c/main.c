#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planilha.h"

int main() {
    GrafoPlanilha planilha;
    inicializarPlanilha(&planilha);

    char entrada[150];
    char coord[10];
    char valor[100];

    printf("=== Planilha (Implementacao com Listas de Adjacencia) ===\n");
    printf("Comandos:\n");
    printf(" - Atribuir valor: A1 10\n");
    printf(" - Referencia:     B1 =A1 (ou +A1)\n");
    printf(" - Funcao:         C1 @soma(A1..A5)\n");
    printf(" - Ver Grafo:      grafo\n");
    printf(" - Exemplo Auto:   demo\n");
    printf(" - Sair:           sair\n\n");

    exibirPlanilha(&planilha);

    while (1) {
        printf("Digite o comando: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) break;

        entrada[strcspn(entrada, "\n")] = 0;
        if (strcmp(entrada, "sair") == 0) break;

        if (strcmp(entrada, "grafo") == 0) {
            exibirGrafo(&planilha);
            continue;
        }

        if (strcmp(entrada, "demo") == 0) {
            printf("\n--- Demo: Listas de Adjacencia ---\n");
            processarEntrada(&planilha, "A1", "10");
            processarEntrada(&planilha, "B1", "20");
            processarEntrada(&planilha, "A2", "30");
            printf(">> D1 recebe +A1\n");
            processarEntrada(&planilha, "D1", "+A1");
            exibirGrafo(&planilha); 
            
            printf(">> C1 recebe @soma(A1..B2)\n");
            processarEntrada(&planilha, "C1", "@soma(A1..B2)");
            
            printf(">> D1 atualizado para +C1 (Limpa lista antiga, cria nova)\n");
            processarEntrada(&planilha, "D1", "+C1");
            
            exibirPlanilha(&planilha);
            exibirGrafo(&planilha);
            continue;
        }

        char *token = strtok(entrada, " ");
        if (token != NULL) {
            strcpy(coord, token);
            token = strtok(NULL, ""); 
            
            if (token != NULL) {
                while(*token == ' ') token++;
                strcpy(valor, token);

                processarEntrada(&planilha, coord, valor);
                exibirPlanilha(&planilha);
            }
        }
    }

    return 0;
}