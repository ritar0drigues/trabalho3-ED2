#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "graph_common.h"

/* BFS usando matriz de adjacência (versão com print já existente) */
void bfs_matrix(int start, int n, int graph[n][n]) {
    int *visited = calloc(n, sizeof(int));
    int *q = malloc(n * sizeof(int));
    int front = 0, back = 0;
    if (!visited || !q) { free(visited); free(q); return; }

    visited[start] = 1;
    q[back++] = start;
    printf("BFS (matriz) a partir de %d: ", start);
    while (front < back) {
        int u = q[front++];
        printf("%d ", u);
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = 1;
                q[back++] = v;
            }
        }
    }
    printf("\n");
    free(q);
    free(visited);
}

/* nova: BFS que marca visited[] (matriz) sem prints */
void bfs_mark_matrix(int start, int n, int graph[n][n], int visited[]) {
    if (!visited) return;
    for (int i = 0; i < n; ++i) visited[i] = 0;
    int *q = malloc(n * sizeof(int));
    if (!q) return;
    int front = 0, back = 0;
    visited[start] = 1; q[back++] = start;
    while (front < back) {
        int u = q[front++];
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = 1;
                q[back++] = v;
            }
        }
    }
    free(q);
}
