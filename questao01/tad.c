/* tad.c */
#include "tad.h"

// Inicializa a matriz com 0 (sem arestas)
void inicializarGrafo(Grafo *g) {
    g->numVertices = NUM_ESTADOS;
    for (int i = 0; i < NUM_ESTADOS; i++) {
        for (int j = 0; j < NUM_ESTADOS; j++) {
            g->matriz[i][j] = 0;
        }
    }
}

// Converte vetor de estado (base 3) para um ID inteiro (0 a 80)
// Ex: Discos [0, 0, 0, 0] -> ID 0. Discos [2, 2, 2, 2] -> ID 80.
int estadoParaId(Estado e) {
    int id = 0;
    int potencia = 1;
    for (int i = 0; i < NUM_DISCOS; i++) {
        id += (e.posicoes[i] - 1) * potencia; // Ajusta para 0-2 (internamente)
        potencia *= 3;
    }
    return id;
}

// Converte ID inteiro de volta para vetor de estado
Estado idParaEstado(int id) {
    Estado e;
    for (int i = 0; i < NUM_DISCOS; i++) {
        e.posicoes[i] = (id % 3) + 1; // Ajusta para 1-3 (apresentação)
        id /= 3;
    }
    return e;
}

void imprimirEstado(Estado e) {
    printf("[ ");
    for (int i = 0; i < NUM_DISCOS; i++) {
        printf("%d ", e.posicoes[i]);
    }
    printf("] (D1...D4)");
}

// Verifica qual disco está no topo de um pino específico
// Retorna o índice do disco (0 a 3) ou -1 se o pino estiver vazio
int topoDoPino(Estado e, int pino) {
    // Procura do menor disco (índice 0) para o maior
    for (int i = 0; i < NUM_DISCOS; i++) {
        if (e.posicoes[i] == pino) {
            return i; // Encontrou o menor disco neste pino (é o topo)
        }
    }
    return -1; // Pino vazio
}

// Preenche a matriz de adjacência com todos os movimentos legais
void construirGrafoHanoi(Grafo *g) {
    inicializarGrafo(g);
    
    // Para cada um dos 81 estados possíveis
    for (int i = 0; i < NUM_ESTADOS; i++) {
        Estado atual = idParaEstado(i);
        
        // Tentar mover o topo de cada pino 'origem' para cada pino 'destino'
        for (int pinoOrigem = 1; pinoOrigem <= 3; pinoOrigem++) {
            int discoMovido = topoDoPino(atual, pinoOrigem);
            
            if (discoMovido == -1) continue; // Pino vazio, nada a mover
            
            for (int pinoDestino = 1; pinoDestino <= 3; pinoDestino++) {
                if (pinoOrigem == pinoDestino) continue;
                
                int discoNoDestino = topoDoPino(atual, pinoDestino);
                
                // Regra de Hanói: Só pode mover se o destino estiver vazio 
                // OU se o disco no destino for MAIOR que o disco movido.
                if (discoNoDestino == -1 || discoNoDestino > discoMovido) {
                    // Movimento Válido! Criar estado vizinho
                    Estado vizinho = atual;
                    vizinho.posicoes[discoMovido] = pinoDestino;
                    
                    int j = estadoParaId(vizinho);
                    g->matriz[i][j] = 1; // Aresta direcionada com peso 1
                }
            }
        }
    }
}

// Algoritmo de Dijkstra para encontrar o menor caminho
void dijkstra(Grafo *g, int inicioId, int fimId) {
    int dist[NUM_ESTADOS];
    int visitado[NUM_ESTADOS];
    int pai[NUM_ESTADOS]; // Para reconstruir o caminho
    
    // Inicialização
    for (int i = 0; i < NUM_ESTADOS; i++) {
        dist[i] = INT_MAX;
        visitado[i] = 0;
        pai[i] = -1;
    }
    
    dist[inicioId] = 0;
    
    for (int i = 0; i < NUM_ESTADOS; i++) {
        // Encontrar o vértice com menor distância não visitado
        int u = -1;
        int minVal = INT_MAX;
        
        for (int v = 0; v < NUM_ESTADOS; v++) {
            if (!visitado[v] && dist[v] < minVal) {
                minVal = dist[v];
                u = v;
            }
        }
        
        if (u == -1 || dist[u] == INT_MAX) break; // Fim dos alcançáveis
        if (u == fimId) break; // Chegamos ao destino
        
        visitado[u] = 1;
        
        // Relaxamento das arestas vizinhas
        for (int v = 0; v < NUM_ESTADOS; v++) {
            if (g->matriz[u][v] == 1 && !visitado[v]) {
                if (dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    pai[v] = u;
                }
            }
        }
    }
    
    // Exibição do Resultado
    if (dist[fimId] == INT_MAX) {
        printf("\nNao ha caminho possivel entre as configuracoes.\n");
    } else {
        printf("\n--- Menor Caminho Encontrado (Dijkstra) ---\n");
        printf("Numero de movimentos: %d\n\n", dist[fimId]);
        
        // Reconstruir caminho usando pilha (array simples)
        int caminho[NUM_ESTADOS];
        int tam = 0;
        int atual = fimId;
        
        while (atual != -1) {
            caminho[tam++] = atual;
            atual = pai[atual];
        }
        
        // Imprimir na ordem correta (inicio -> fim)
        printf("Passo a passo:\n");
        for (int i = tam - 1; i >= 0; i--) {
            printf("%02d: ", tam - 1 - i); // Contador de passos
            imprimirEstado(idParaEstado(caminho[i]));
            printf("\n");
        }
    }
}