#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include "graph_common.h"

// internal recursive helper (com prints já existente)
void dfs_matrix_rec(int u, int n, int graph[n][n], int visited[]) {
    visited[u] = 1;
    printf("%d ", u);

    for (int v = 0; v < n; v++)
        if (graph[u][v] && !visited[v])
            dfs_matrix_rec(v, n, graph, visited);
}

void dfs_matrix(int start, int n, int graph[n][n]) {
    int *visited = calloc(n, sizeof(int));
    printf("DFS (matriz) a partir de %d: ", start);
    dfs_matrix_rec(start, n, graph, visited);
    printf("\n");
    free(visited);
}

/* nova implementação: marca visited[] usando DFS recursiva, sem prints */
static void dfs_mark_rec(int u, int n, int graph[n][n], int visited[]) {
    if (visited[u]) return;
    visited[u] = 1;
    for (int v = 0; v < n; ++v) {
        if (graph[u][v] && !visited[v]) dfs_mark_rec(v, n, graph, visited);
    }
}

void dfs_mark_matrix(int start, int n, int graph[n][n], int visited[]) {
    if (!visited) return;
    for (int i = 0; i < n; ++i) visited[i] = 0;
    dfs_mark_rec(start, n, graph, visited);
}
