/* tad.h */
#ifndef TAD_H
#define TAD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Definições do problema
#define NUM_DISCOS 4
#define NUM_PINOS 3
#define NUM_ESTADOS 81 // 3^4 = 81 estados possíveis

// Estrutura do Grafo usando Matriz de Adjacência
typedef struct {
    int matriz[NUM_ESTADOS][NUM_ESTADOS];
    int numVertices;
} Grafo;

// Estrutura para representar uma configuração (vetor de discos)
typedef struct {
    int posicoes[NUM_DISCOS]; // Índice 0 = menor disco, Índice 3 = maior disco
} Estado;

// Funções de gerenciamento do Grafo e Estados
void inicializarGrafo(Grafo *g);
int estadoParaId(Estado e);
Estado idParaEstado(int id);
void imprimirEstado(Estado e);

// Funções de lógica da Torre de Hanói
int movimentoValido(Estado atual, Estado proximo);
void construirGrafoHanoi(Grafo *g);

// Algoritmo de Dijkstra
void dijkstra(Grafo *g, int inicioId, int fimId);

#endif