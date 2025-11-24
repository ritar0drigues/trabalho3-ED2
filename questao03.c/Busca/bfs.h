#ifndef BFS_H
#define BFS_H
#include "graph_common.h"


/* funções que já tens (mantive para compatibilidade) */
void bfs_matrix(int start, int n, int graph[n][n]);

/* novas funções: marcam o array visited[] com 1 para vértices alcançados */
void bfs_mark_matrix(int start, int n, int graph[n][n], int visited[]);

#endif