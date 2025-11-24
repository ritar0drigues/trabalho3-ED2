#ifndef DFS_H
#define DFS_H
#include "graph_common.h"

void dfs_list_rec(int u, int n, struct Node* adj[], int visited[]);
void dfs_list(int start, int n, struct Node* adj[]);

/* nova: marca visited[] com os vértices alcançados via DFS (sem prints) */
void dfs_mark_list(int start, int n, struct Node* adj[], int visited[]);

#endif
