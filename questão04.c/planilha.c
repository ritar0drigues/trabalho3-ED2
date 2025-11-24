#include "planilha.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>

// Inicializa vértices e listas de adjacência
void inicializarPlanilha(GrafoPlanilha *g) {
    for (int i = 0; i < MAX_CELULAS; i++) {
        g->celulas[i].valorCalculado = 0.0;
        strcpy(g->celulas[i].formula, "");
        g->celulas[i].visitado = 0;
        
        // Inicializa a lista de adjacência como vazia
        g->adj[i] = NULL;
    }
}

// Converte "A1" -> Índice
int obterIndice(char *coord) {
    if (strlen(coord) < 2) return -1;
    char colChar = toupper(coord[0]);
    if (colChar < 'A' || colChar > 'H') return -1;
    int col = colChar - 'A';
    int linha = atoi(&coord[1]) - 1;
    if (linha < 0 || linha >= NUM_LINHAS) return -1;
    return (linha * NUM_COLUNAS) + col;
}

// Converte Índice -> "A1"
void obterNomeCoord(int indice, char *buffer) {
    int l = indice / NUM_COLUNAS;
    int c = indice % NUM_COLUNAS;
    sprintf(buffer, "%c%d", 'A' + c, l + 1);
}

// Função auxiliar para adicionar aresta na lista
// Insere no início da lista para ser O(1)
void adicionarAresta(GrafoPlanilha *g, int origem, int destino) {
    NoAdj *novo = (NoAdj*)malloc(sizeof(NoAdj));
    novo->idVertice = destino;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}

// Função para limpar a lista de adjacências de uma célula (usada ao mudar a fórmula)
void limparArestas(GrafoPlanilha *g, int idx) {
    NoAdj *atual = g->adj[idx];
    while (atual != NULL) {
        NoAdj *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    g->adj[idx] = NULL;
}

// Função recursiva para calcular valor
double avaliarCelula(GrafoPlanilha *g, int idx) {
    if (g->celulas[idx].visitado) {
        printf("ERRO: Referencia circular detectada em %s!\n", g->celulas[idx].formula);
        return 0.0;
    }

    g->celulas[idx].visitado = 1;
    char *fmt = g->celulas[idx].formula;
    double resultado = 0.0;

    if (strlen(fmt) == 0) {
        resultado = 0.0;
    }
    else if (fmt[0] == '+' || fmt[0] == '=') {
        int idxRef = obterIndice(&fmt[1]);
        if (idxRef != -1) {
            resultado = avaliarCelula(g, idxRef);
        }
    }
    else if (fmt[0] == '@') {
        char tipoFunc[20], rangeStr[20];
        sscanf(fmt, "@%[^(](%[^)])", tipoFunc, rangeStr);
        char *p = strstr(rangeStr, "..");
        if (p) {
            *p = '\0';
            int idxInicio = obterIndice(rangeStr);
            int idxFim = obterIndice(p + 2);

            if (idxInicio != -1 && idxFim != -1) {
                int l1 = idxInicio / NUM_COLUNAS, c1 = idxInicio % NUM_COLUNAS;
                int l2 = idxFim / NUM_COLUNAS, c2 = idxFim % NUM_COLUNAS;
                
                double soma = 0, min = DBL_MAX, max = -DBL_MAX;
                int count = 0;

                for (int l = l1; l <= l2; l++) {
                    for (int c = c1; c <= c2; c++) {
                        int currIdx = l * NUM_COLUNAS + c;
                        double val = avaliarCelula(g, currIdx);
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
    else {
        resultado = atof(fmt);
    }

    g->celulas[idx].visitado = 0;
    return resultado;
}

// Atualiza arestas (Listas de Adjacência)
void atualizarDependencias(GrafoPlanilha *g, int idxAlvo, char *conteudo) {
    // 1. Limpar dependências antigas (libera memória da lista)
    limparArestas(g, idxAlvo);

    // 2. Analisar nova dependência e inserir nós na lista
    if (conteudo[0] == '=' || conteudo[0] == '+') {
        int idxRef = obterIndice(&conteudo[1]);
        if (idxRef != -1) {
            adicionarAresta(g, idxAlvo, idxRef);
        }
    }
    else if (conteudo[0] == '@') {
        char temp[TAM_FORMULA];
        strcpy(temp, conteudo);
        char *ptrIni = strchr(temp, '(');
        char *ptrFim = strchr(temp, ')');
        char *ptrSep = strstr(temp, "..");
        
        if (ptrIni && ptrFim && ptrSep) {
            *ptrFim = '\0'; *ptrSep = '\0';
            int idxInicio = obterIndice(ptrIni + 1);
            int idxFinal = obterIndice(ptrSep + 2);
            
            if (idxInicio != -1 && idxFinal != -1) {
                 int l1 = idxInicio / NUM_COLUNAS, c1 = idxInicio % NUM_COLUNAS;
                 int l2 = idxFinal / NUM_COLUNAS, c2 = idxFinal % NUM_COLUNAS;

                 for (int l = l1; l <= l2; l++) {
                    for (int c = c1; c <= c2; c++) {
                        int depIdx = l * NUM_COLUNAS + c;
                        adicionarAresta(g, idxAlvo, depIdx);
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
    strcpy(g->celulas[idx].formula, conteudo);
    atualizarDependencias(g, idx, conteudo);
    atualizarPlanilha(g);
}

void atualizarPlanilha(GrafoPlanilha *g) {
    for (int i = 0; i < MAX_CELULAS; i++) {
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
            if (g->celulas[idx].valorCalculado == 0 && strlen(g->celulas[idx].formula) == 0)
                printf("%-8s", ".");
            else
                printf("%-8.1f", g->celulas[idx].valorCalculado);
        }
        printf("\n");
    }
    printf("\n");
}

void exibirGrafo(GrafoPlanilha *g) {
    printf("\n--- Estrutura do Grafo (Listas de Adjacencia) ---\n");
    int encontrou = 0;
    char origem[10], destino[10];

    for (int i = 0; i < MAX_CELULAS; i++) {
        NoAdj *atual = g->adj[i]; // Cabeça da lista
        if (atual != NULL) {
            encontrou = 1;
            obterNomeCoord(i, destino);
            printf("[%s] depende de -> ", destino);
            
            while (atual != NULL) {
                obterNomeCoord(atual->idVertice, origem);
                printf("[%s] ", origem);
                atual = atual->prox;
            }
            printf("\n");
        }
    }
    if (!encontrou) printf("Nenhuma dependencia registrada.\n");
    printf("-------------------------------------------------\n");
}