#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include "graph_common.h"


void dfs_list_rec(int u, int n, struct Node* adj[], int visited[]) {
    visited[u] = 1;
    printf("%d ", u);

    struct Node *p = adj[u];
    while (p) {
        if (!visited[p->v])
            dfs_list_rec(p->v, n, adj, visited);
        p = p->next;
    }
}

void dfs_list(int start, int n, struct Node* adj[]) {
    int *visited = calloc(n, sizeof(int));
    printf("DFS (lista) a partir de %d: ", start);
    dfs_list_rec(start, n, adj, visited);
    printf("\n");
    free(visited);
}

/* nova implementação: marca visited[] usando DFS recursiva, sem prints */
static void dfs_mark_rec(int u, int n, struct Node* adj[], int visited[]) {
    if (visited[u]) return;
    visited[u] = 1;
    struct Node *p = adj[u];
    while (p) {
        if (!visited[p->v]) dfs_mark_rec(p->v, n, adj, visited);
        p = p->next;
    }
}

void dfs_mark_list(int start, int n, struct Node* adj[], int visited[]) {
    if (!visited) return;
    for (int i = 0; i < n; ++i) visited[i] = 0;
    dfs_mark_rec(start, n, adj, visited);
}
