/* tad.h */
#ifndef TAD_H
#define TAD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_DISCOS 4
#define NUM_PINOS 3
#define NUM_ESTADOS 81 // 3^4

// Estrutura para um nó da lista encadeada (Representa uma aresta)
typedef struct No {
    int destino;        // ID do vértice vizinho
    int peso;           // Peso da aresta (será sempre 1 neste caso)
    struct No* prox;    // Ponteiro para o próximo vizinho
} No;

// Estrutura do Grafo usando Lista de Adjacência
typedef struct {
    No* listaAdj[NUM_ESTADOS]; // Array de ponteiros (cabeças das listas)
    int numVertices;
} Grafo;

// Estrutura para representar a configuração
typedef struct {
    int posicoes[NUM_DISCOS]; 
} Estado;

// Funções
void inicializarGrafo(Grafo *g);
void liberarGrafo(Grafo *g); // Importante para limpar memória de listas
int estadoParaId(Estado e);
Estado idParaEstado(int id);
void imprimirEstado(Estado e);

// Lógica
void construirGrafoHanoi(Grafo *g);

// Dijkstra
void dijkstra(Grafo *g, int inicioId, int fimId);

#endif