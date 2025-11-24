#ifndef DFS_H
#define DFS_H
#include "graph_common.h"

void dfs_matrix(int start, int n, int graph[n][n]);
void dfs_matrix_rec(int u, int n, int graph[n][n], int visited[]);

/* nova: marca visited[] usando DFS (matriz), sem prints */
void dfs_mark_matrix(int start, int n, int graph[n][n], int visited[]);

#endif
