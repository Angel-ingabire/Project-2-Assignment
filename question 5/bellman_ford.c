#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Nodes: A B C D E F G H I J => 0..9
#define NODES 10

typedef struct
{
    int u, v, w;
} Edge;

const char *names = "ABCDEFGHIJ";

void print_path(int pred[], int v)
{
    if (v < 0)
        return;
    int stack[NODES];
    int top = 0;
    while (v != -1)
    {
        stack[top++] = v;
        v = pred[v];
    }
    for (int i = top - 1; i >= 0; --i)
    {
        printf("%c", names[stack[i]]);
        if (i)
            printf(" -> ");
    }
}

int main(void)
{
    // Undirected edges (stored as directed pairs)
    Edge edges[] = {
        {0, 1, 4}, {1, 0, 4}, // A-B
        {1, 2, 6},
        {2, 1, 6}, // B-C
        {2, 6, 9},
        {6, 2, 9}, // C-G
        {6, 7, 13},
        {7, 6, 13}, // G-H
        {0, 3, 16},
        {3, 0, 16}, // A-D
        {1, 3, 6},
        {3, 1, 6}, // B-D
        {1, 9, 7},
        {9, 1, 7}, // B-J
        {3, 4, 7},
        {4, 3, 7}, // D-E
        {9, 5, 3},
        {5, 9, 3}, // J-F
        {5, 6, 3},
        {6, 5, 3}, // F-G
        {4, 5, 10},
        {5, 4, 10}, // E-F
        {4, 8, 2},
        {8, 4, 2}, // E-I
        {5, 8, 10},
        {8, 5, 10} // F-I
    };
    int m = sizeof(edges) / sizeof(edges[0]);

    // Print graph summary
    printf("Graph edges (u -> v : w):\n");
    for (int i = 0; i < m; ++i)
        printf(" %c -> %c : %d\n", names[edges[i].u], names[edges[i].v], edges[i].w);

    // Bellman-Ford from source A (0)
    int src = 0;
    int dist[NODES];
    int pred[NODES];
    for (int i = 0; i < NODES; ++i)
    {
        dist[i] = INT_MAX / 4;
        pred[i] = -1;
    }
    dist[src] = 0;

    for (int iter = 0; iter < NODES - 1; ++iter)
    {
        int changed = 0;
        for (int e = 0; e < m; ++e)
        {
            int u = edges[e].u, v = edges[e].v, w = edges[e].w;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pred[v] = u;
                changed = 1;
            }
        }
        if (!changed)
            break;
    }

    // Check for negative-weight cycles
    int negcycle = 0;
    for (int e = 0; e < m; ++e)
    {
        int u = edges[e].u, v = edges[e].v, w = edges[e].w;
        if (dist[u] + w < dist[v])
        {
            negcycle = 1;
            break;
        }
    }

    if (negcycle)
    {
        printf("\nNegative-weight cycle detected in graph. Shortest paths undefined.\n");
        return 0;
    }

    // Print shortest paths and costs
    printf("\nShortest paths from Branch A:\n");
    for (int v = 0; v < NODES; ++v)
    {
        printf("To %c: ", names[v]);
        if (dist[v] >= INT_MAX / 8)
        {
            printf("unreachable\n");
            continue;
        }
        print_path(pred, v);
        printf("  |  Cost = %d\n", dist[v]);
    }

    return 0;
}
