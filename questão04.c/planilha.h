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

// Estrutura de cada Célula (Dados do Vértice)
typedef struct {
    char formula[TAM_FORMULA]; 
    double valorCalculado;     
    int visitado;              
} Celula;

// Estrutura para o Nó da Lista de Adjacências
typedef struct NoAdj {
    int idVertice;        // Índice da célula dependente
    struct NoAdj *prox;   // Próximo nó na lista
} NoAdj;

// Estrutura do Grafo (Planilha)
typedef struct {
    Celula celulas[MAX_CELULAS];    // Vetor de dados dos vértices
    NoAdj *adj[MAX_CELULAS];        // Vetor de listas de adjacências
    // adj[i] aponta para o primeiro nó da lista de células das quais 'i' depende
} GrafoPlanilha;

// --- Protótipos das Funções ---

void inicializarPlanilha(GrafoPlanilha *g);
void processarEntrada(GrafoPlanilha *g, char *coordStr, char *conteudo);
void atualizarPlanilha(GrafoPlanilha *g);
void exibirPlanilha(GrafoPlanilha *g);
void exibirGrafo(GrafoPlanilha *g);

// Helpers
int obterIndice(char *coord);
void obterNomeCoord(int indice, char *buffer);

#endif