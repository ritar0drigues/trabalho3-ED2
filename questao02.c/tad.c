/* tad.c */
#include "tad.h"

// Inicializa todas as listas como NULL
void inicializarGrafo(Grafo *g) {
    g->numVertices = NUM_ESTADOS;
    for (int i = 0; i < NUM_ESTADOS; i++) {
        g->listaAdj[i] = NULL;
    }
}

// Função auxiliar para adicionar aresta na lista
void adicionarAresta(Grafo *g, int origem, int destino) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->destino = destino;
    novoNo->peso = 1;
    // Inserção no início da lista (mais rápido: O(1))
    novoNo->prox = g->listaAdj[origem];
    g->listaAdj[origem] = novoNo;
}

// Libera a memória alocada pelos nós
void liberarGrafo(Grafo *g) {
    for (int i = 0; i < NUM_ESTADOS; i++) {
        No* atual = g->listaAdj[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        g->listaAdj[i] = NULL;
    }
}

int estadoParaId(Estado e) {
    int id = 0;
    int potencia = 1;
    for (int i = 0; i < NUM_DISCOS; i++) {
        id += (e.posicoes[i] - 1) * potencia;
        potencia *= 3;
    }
    return id;
}

Estado idParaEstado(int id) {
    Estado e;
    for (int i = 0; i < NUM_DISCOS; i++) {
        e.posicoes[i] = (id % 3) + 1;
        id /= 3;
    }
    return e;
}

void imprimirEstado(Estado e) {
    printf("[ ");
    for (int i = 0; i < NUM_DISCOS; i++) {
        printf("%d ", e.posicoes[i]);
    }
    printf("] ");
}

int topoDoPino(Estado e, int pino) {
    for (int i = 0; i < NUM_DISCOS; i++) {
        if (e.posicoes[i] == pino) return i;
    }
    return -1;
}

void construirGrafoHanoi(Grafo *g) {
    inicializarGrafo(g);
    
    for (int i = 0; i < NUM_ESTADOS; i++) {
        Estado atual = idParaEstado(i);
        
        for (int pinoOrigem = 1; pinoOrigem <= 3; pinoOrigem++) {
            int discoMovido = topoDoPino(atual, pinoOrigem);
            if (discoMovido == -1) continue;
            
            for (int pinoDestino = 1; pinoDestino <= 3; pinoDestino++) {
                if (pinoOrigem == pinoDestino) continue;
                
                int discoNoDestino = topoDoPino(atual, pinoDestino);
                
                if (discoNoDestino == -1 || discoNoDestino > discoMovido) {
                    Estado vizinho = atual;
                    vizinho.posicoes[discoMovido] = pinoDestino;
                    int j = estadoParaId(vizinho);
                    
                    // --- MUDANÇA AQUI ---
                    // Em vez de matriz[i][j] = 1, chamamos a função
                    adicionarAresta(g, i, j);
                }
            }
        }
    }
}

void dijkstra(Grafo *g, int inicioId, int fimId) {
    int dist[NUM_ESTADOS];
    int visitado[NUM_ESTADOS];
    int pai[NUM_ESTADOS];
    
    for (int i = 0; i < NUM_ESTADOS; i++) {
        dist[i] = INT_MAX;
        visitado[i] = 0;
        pai[i] = -1;
    }
    
    dist[inicioId] = 0;
    
    // Loop principal (para V vértices)
    for (int count = 0; count < NUM_ESTADOS - 1; count++) {
        // 1. Escolher o vértice de menor distância não visitado
        int u = -1;
        int minVal = INT_MAX;
        
        for (int v = 0; v < NUM_ESTADOS; v++) {
            if (!visitado[v] && dist[v] < minVal) {
                minVal = dist[v];
                u = v;
            }
        }
        
        if (u == -1 || dist[u] == INT_MAX) break;
        if (u == fimId) break;
        
        visitado[u] = 1;
        
        // --- MUDANÇA CRÍTICA AQUI (LISTA DE ADJACÊNCIA) ---
        // Em vez de for(v=0...81), percorremos a lista encadeada
        No* adj = g->listaAdj[u];
        while (adj != NULL) {
            int v = adj->destino;
            
            if (!visitado[v] && dist[u] + adj->peso < dist[v]) {
                dist[v] = dist[u] + adj->peso;
                pai[v] = u;
            }
            adj = adj->prox; // Avança para o próximo vizinho na lista
        }
    }
    
    if (dist[fimId] == INT_MAX) {
        printf("\nNao ha caminho possivel.\n");
    } else {
        printf("\n--- Menor Caminho Encontrado ---\n");
        printf("Numero de movimentos: %d\n\n", dist[fimId]);
        
        int caminho[NUM_ESTADOS];
        int tam = 0;
        int atual = fimId;
        while (atual != -1) {
            caminho[tam++] = atual;
            atual = pai[atual];
        }
        
        printf("Passo a passo:\n");
        for (int i = tam - 1; i >= 0; i--) {
            printf("%02d: ", tam - 1 - i);
            imprimirEstado(idParaEstado(caminho[i]));
            printf("\n");
        }
    }
}