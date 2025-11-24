#ifndef BFS_H
#define BFS_H
#include "graph_common.h"


void bfs_list(int start, int n, struct Node* adj[]);

/* novas funções: marcam o array visited[] com 1 para vértices alcançados */
void bfs_mark_list(int start, int n, struct Node* adj[], int visited[]);

#endif