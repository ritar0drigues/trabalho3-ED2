#ifndef PLANILHA_H
#define PLANILHA_H

#include <stdio.h>

// Definições de tamanho
#define NUM_LINHAS 20
#define NUM_COLUNAS 8   // A até H
#define MAX_CELULAS (NUM_LINHAS * NUM_COLUNAS)
#define TAM_FORMULA 100

// Enum para facilitar leitura
enum Funcoes { NENHUMA, SOMA, MAX, MIN, MEDIA };

// Estrutura de cada Célula (Vértice com dados)
typedef struct {
    char formula[TAM_FORMULA]; // O que o usuário digitou (ex: "@soma(A1..A3)" ou "10")
    double valorCalculado;     // O resultado numérico
    int visitado;              // Para detecção de ciclos durante recursão
} Celula;

// Estrutura do Grafo (Planilha)
typedef struct {
    Celula celulas[MAX_CELULAS];        // Vetor de vértices
    int matrizAdj[MAX_CELULAS][MAX_CELULAS]; // Matriz de Adjacência: [Destino][Origem]
    // Se matrizAdj[i][j] == 1, significa que a célula i depende da célula j
} GrafoPlanilha;

// --- Protótipos das Funções ---

// Inicializa a planilha zerada
void inicializarPlanilha(GrafoPlanilha *g);

// Processa a entrada do usuário (Ex: "A1 10" ou "C1 @soma(...)")
void processarEntrada(GrafoPlanilha *g, char *coordStr, char *conteudo);

// Recalcula toda a planilha baseada nas dependências
void atualizarPlanilha(GrafoPlanilha *g);

// Exibe a planilha na tela
void exibirPlanilha(GrafoPlanilha *g);

// Exibe as arestas do grafo (dependências)
void exibirGrafo(GrafoPlanilha *g);

// Helpers auxiliares
int obterIndice(char *coord);
void obterNomeCoord(int indice, char *buffer);

#endif