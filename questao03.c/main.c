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

    printf("=== Planilha Rudimentar com Grafo ===\n");
    printf("Comandos:\n");
    printf(" - Atribuir valor: A1 10\n");
    printf(" - Referencia:     B1 =A1 (ou +A1)\n");
    printf(" - Funcao:         C1 @soma(A1..A5)\n");
    printf(" - Ver Grafo:      grafo\n");
    printf(" - Exemplo Auto:   demo\n");
    printf(" - Sair:           sair\n\n");

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

        // Parse simples da string de entrada
        // Separa o primeiro token (coord) do resto (valor/formula)
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
            } else {
                printf("Formato inválido. Use: CELULA VALOR\n");
            }
        }
    }

    return 0;
}