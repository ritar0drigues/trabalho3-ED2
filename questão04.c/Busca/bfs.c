#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "graph_common.h"

void bfs_list(int start, int n, struct Node* adj[]) {
    int *visited = calloc(n, sizeof(int));
    int *q = malloc(n * sizeof(int));
    int front = 0, back = 0;
    if (!visited || !q) { free(visited); free(q); return; }

    visited[start] = 1;
    q[back++] = start;
    printf("BFS (lista) a partir de %d: ", start);
    while (front < back) {
        int u = q[front++];
        printf("%d ", u);
        struct Node *p = adj[u];
        while (p) {
            if (!visited[p->v]) {
                visited[p->v] = 1;
                q[back++] = p->v;
            }
            p = p->next;
        }
    }
    printf("\n");
    free(q);
    free(visited);
}

/* nova: BFS que marca visited[] (lista) sem prints */
void bfs_mark_list(int start, int n, struct Node* adj[], int visited[]) {
    if (!visited) return;
    for (int i = 0; i < n; ++i) visited[i] = 0;
    int *q = malloc(n * sizeof(int));
    if (!q) return;
    int front = 0, back = 0;
    visited[start] = 1; q[back++] = start;
    while (front < back) {
        int u = q[front++];
        struct Node *p = adj[u];
        while (p) {
            if (!visited[p->v]) {
                visited[p->v] = 1;
                q[back++] = p->v;
            }
            p = p->next;
        }
    }
    free(q);
}
