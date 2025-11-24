#include "planilha.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>

// Inicializa vértices e matriz de adjacência
void inicializarPlanilha(GrafoPlanilha *g) {
    for (int i = 0; i < MAX_CELULAS; i++) {
        g->celulas[i].valorCalculado = 0.0;
        strcpy(g->celulas[i].formula, "");
        g->celulas[i].visitado = 0;
        for (int j = 0; j < MAX_CELULAS; j++) {
            g->matrizAdj[i][j] = 0;
        }
    }
}

// Converte "A1" -> Índice (0 a 159)
// Retorna -1 se inválido
int obterIndice(char *coord) {
    if (strlen(coord) < 2) return -1;
    
    char colChar = toupper(coord[0]);
    if (colChar < 'A' || colChar > 'H') return -1;
    
    int col = colChar - 'A';
    int linha = atoi(&coord[1]) - 1; // "1" vira 0z
    
    if (linha < 0 || linha >= NUM_LINHAS) return -1;
    
    return (linha * NUM_COLUNAS) + col;
}

// Função auxiliar recursiva para calcular valor de uma célula
double avaliarCelula(GrafoPlanilha *g, int idx) {
    // Detecção de ciclo simples
    if (g->celulas[idx].visitado) {
        printf("ERRO: Referencia circular detectada em %s!\n", g->celulas[idx].formula);
        return 0.0;
    }

    g->celulas[idx].visitado = 1;
    char *fmt = g->celulas[idx].formula;
    double resultado = 0.0;

    // CASO 1: Vazio
    if (strlen(fmt) == 0) {
        resultado = 0.0;
    }
    // CASO 2: Referência Direta ou Soma Simples (Ex: "+A1" ou "=A1")
    else if (fmt[0] == '+' || fmt[0] == '=') {
        int idxRef = obterIndice(&fmt[1]);
        if (idxRef != -1) {
            resultado = avaliarCelula(g, idxRef);
        }
    }
    // CASO 3: Função (Ex: "@soma(A1..B2)")
    else if (fmt[0] == '@') {
        char tipoFunc[20];
        char rangeStr[20];
        
        // Parse rudimentar: pega nome da funcao e o conteudo dentro dos parenteses
        sscanf(fmt, "@%[^(](%[^)])", tipoFunc, rangeStr);
        
        // Separa A1..B2
        char *p = strstr(rangeStr, "..");
        if (p) {
            *p = '\0'; // Corta a string
            char *inicioStr = rangeStr;
            char *fimStr = p + 2;
            
            int idxInicio = obterIndice(inicioStr);
            int idxFim = obterIndice(fimStr);

            if (idxInicio != -1 && idxFim != -1) {
                int l1 = idxInicio / NUM_COLUNAS;
                int c1 = idxInicio % NUM_COLUNAS;
                int l2 = idxFim / NUM_COLUNAS;
                int c2 = idxFim % NUM_COLUNAS;
                
                double soma = 0, min = DBL_MAX, max = -DBL_MAX;
                int count = 0;

                // Itera sobre o retângulo de células
                for (int l = l1; l <= l2; l++) {
                    for (int c = c1; c <= c2; c++) {
                        int currIdx = l * NUM_COLUNAS + c;
                        double val = avaliarCelula(g, currIdx); // Recursão
                        
                        soma += val;
                        if (val < min) min = val;
                        if (val > max) max = val;
                        count++;
                    }
                }
                
                if (strcmp(tipoFunc, "soma") == 0) resultado = soma;
                else if (strcmp(tipoFunc, "media") == 0) resultado = (count > 0) ? soma/count : 0;
                else if (strcmp(tipoFunc, "max") == 0) resultado = max;
                else if (strcmp(tipoFunc, "min") == 0) resultado = min;
            }
        }
    }
    // CASO 4: Número direto
    else {
        resultado = atof(fmt);
    }

    g->celulas[idx].visitado = 0; // Backtrack
    return resultado;
}

// Atualiza arestas do grafo baseadas na nova fórmula inserida
void atualizarDependencias(GrafoPlanilha *g, int idxAlvo, char *conteudo) {
    // 1. Limpar dependências antigas (zera a linha do alvo na matriz)
    for (int i = 0; i < MAX_CELULAS; i++) {
        g->matrizAdj[idxAlvo][i] = 0;
    }

    // 2. Analisar nova dependência
    if (conteudo[0] == '=' || conteudo[0] == '+') {
        int idxRef = obterIndice(&conteudo[1]);
        if (idxRef != -1) {
            g->matrizAdj[idxAlvo][idxRef] = 1; // Alvo depende de Ref
        }
    }
    else if (conteudo[0] == '@') {
        // Lógica similar ao eval para extrair range e marcar dependências
        char temp[TAM_FORMULA];
        strcpy(temp, conteudo);
        char *ptrIni = strchr(temp, '(');
        char *ptrFim = strchr(temp, ')');
        char *ptrSep = strstr(temp, "..");
        
        if (ptrIni && ptrFim && ptrSep) {
            *ptrFim = '\0';
            *ptrSep = '\0';
            int idxInicio = obterIndice(ptrIni + 1);
            int idxFinal = obterIndice(ptrSep + 2);
            
            if (idxInicio != -1 && idxFinal != -1) {
                 int l1 = idxInicio / NUM_COLUNAS;
                 int c1 = idxInicio % NUM_COLUNAS;
                 int l2 = idxFinal / NUM_COLUNAS;
                 int c2 = idxFinal % NUM_COLUNAS;

                 for (int l = l1; l <= l2; l++) {
                    for (int c = c1; c <= c2; c++) {
                        int depIdx = l * NUM_COLUNAS + c;
                        g->matrizAdj[idxAlvo][depIdx] = 1; // Cria aresta
                    }
                 }
            }
        }
    }
}

void processarEntrada(GrafoPlanilha *g, char *coordStr, char *conteudo) {
    int idx = obterIndice(coordStr);
    if (idx == -1) {
        printf("Célula inválida!\n");
        return;
    }

    // Armazena a fórmula
    strcpy(g->celulas[idx].formula, conteudo);

    // Atualiza o Grafo (Arestas)
    atualizarDependencias(g, idx, conteudo);

    // Recalcula valores
    atualizarPlanilha(g);
}

void atualizarPlanilha(GrafoPlanilha *g) {
    // Na prática, em um grafo complexo, faríamos uma ordenação topológica.
    // Como é "rudimentar" e temos recursão com flag de visita no avaliarCelula,
    // podemos iterar e reavaliar.
    for (int i = 0; i < MAX_CELULAS; i++) {
        // Se a célula tem conteúdo, avalia
        if (strlen(g->celulas[i].formula) > 0) {
            g->celulas[i].valorCalculado = avaliarCelula(g, i);
        } else {
            g->celulas[i].valorCalculado = 0;
        }
    }
}

void exibirPlanilha(GrafoPlanilha *g) {
    printf("\n    ");
    for (int c = 0; c < NUM_COLUNAS; c++) printf("%-8c", 'A' + c);
    printf("\n");
    
    for (int l = 0; l < NUM_LINHAS; l++) {
        printf("%2d |", l + 1);
        for (int c = 0; c < NUM_COLUNAS; c++) {
            int idx = l * NUM_COLUNAS + c;
            // Exibe o valor calculado, mas se for 0 e sem formula, mostra vazio para ficar limpo
            if (g->celulas[idx].valorCalculado == 0 && strlen(g->celulas[idx].formula) == 0)
                printf("%-8s", ".");
            else
                printf("%-8.1f", g->celulas[idx].valorCalculado);
        }
        printf("\n");
    }
    printf("\n");
}

void obterNomeCoord(int indice, char *buffer) {
    int l = indice / NUM_COLUNAS;
    int c = indice % NUM_COLUNAS;
    sprintf(buffer, "%c%d", 'A' + c, l + 1);
}

void exibirGrafo(GrafoPlanilha *g) {
    printf("\n--- Estrutura do Grafo (Dependencias) ---\n");
    int encontrou = 0;
    char origem[10], destino[10];

    for (int i = 0; i < MAX_CELULAS; i++) {
        for (int j = 0; j < MAX_CELULAS; j++) {
            if (g->matrizAdj[i][j]) {
                obterNomeCoord(i, destino);
                obterNomeCoord(j, origem);
                printf("Aresta: [%s] depende de [%s]\n", destino, origem);
                encontrou = 1;
            }
        }
    }
    if (!encontrou) printf("Nenhuma dependencia registrada (Grafo sem arestas).\n");
    printf("-----------------------------------------\n");
}