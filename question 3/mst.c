#include <stdio.h>
#include <stdlib.h>

// Nodes: A B C D E F G => indices 0..6
#define NODES 7

typedef struct
{
    int u, v, w;
} Edge;

int parent[NODES];
int rankv[NODES];

int find_set(int x)
{
    if (parent[x] != x)
        parent[x] = find_set(parent[x]);
    return parent[x];
}

void union_set(int a, int b)
{
    a = find_set(a);
    b = find_set(b);
    if (a == b)
        return;
    if (rankv[a] < rankv[b])
        parent[a] = b;
    else if (rankv[b] < rankv[a])
        parent[b] = a;
    else
    {
        parent[b] = a;
        rankv[a]++;
    }
}

int cmp_edge(const void *p, const void *q)
{
    Edge *a = (Edge *)p;
    Edge *b = (Edge *)q;
    return a->w - b->w;
}

int main(void)
{
    const char *names = "ABCDEFG";

    // Adjacency matrix: 0 means no direct connection
    int adj[NODES][NODES] = {
        // A  B  C  D  E  F  G
        {0, 6, 0, 0, 0, 0, 0},    // A
        {6, 0, 11, 5, 0, 0, 0},   // B
        {0, 11, 0, 17, 0, 0, 25}, // C
        {0, 5, 17, 0, 22, 0, 0},  // D
        {0, 0, 0, 22, 0, 10, 0},  // E
        {0, 0, 0, 0, 10, 0, 22},  // F
        {0, 0, 25, 0, 0, 22, 0}   // G
    };

    printf("Adjacency matrix (0 = no direct connection):\n\n");
    printf("   ");
    for (int j = 0; j < NODES; ++j)
        printf(" %2c", names[j]);
    printf("\n");
    for (int i = 0; i < NODES; ++i)
    {
        printf(" %c ", names[i]);
        for (int j = 0; j < NODES; ++j)
            printf(" %2d", adj[i][j]);
        printf("\n");
    }

    // Build edge list (only once per undirected edge)
    Edge edges[] = {
        {0, 1, 6},  // A-B
        {1, 3, 5},  // B-D
        {1, 2, 11}, // B-C
        {2, 3, 17}, // C-D
        {2, 6, 25}, // C-G
        {5, 6, 22}, // F-G
        {3, 4, 22}, // D-E
        {4, 5, 10}  // E-F
    };
    int m = sizeof(edges) / sizeof(edges[0]);

    // Initialize union-find
    for (int i = 0; i < NODES; ++i)
    {
        parent[i] = i;
        rankv[i] = 0;
    }

    // Sort edges by weight
    qsort(edges, m, sizeof(Edge), cmp_edge);

    Edge mst[NODES - 1];
    int mst_edges = 0;
    int total_cost = 0;

    for (int i = 0; i < m && mst_edges < NODES - 1; ++i)
    {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;
        if (find_set(u) != find_set(v))
        {
            union_set(u, v);
            mst[mst_edges++] = edges[i];
            total_cost += w;
        }
    }

    printf("\nSelected MST edges (hub1 - hub2 : cost):\n");
    for (int i = 0; i < mst_edges; ++i)
    {
        printf(" %c - %c : %d\n", names[mst[i].u], names[mst[i].v], mst[i].w);
    }
    printf("\nTotal installation cost (millions): %d\n", total_cost);

    // --- Validation checks ---
    // 1) MST should have NODES-1 edges
    if (mst_edges != NODES - 1)
    {
        printf("\n[ERROR] MST does not have %d edges (has %d)\n", NODES - 1, mst_edges);
    }
    else
    {
        printf("\n[OK] MST has %d edges.\n", mst_edges);
    }

    // 2) All nodes should be connected (single component)
    int root = find_set(0);
    int connected = 1;
    for (int i = 1; i < NODES; ++i)
        if (find_set(i) != root)
            connected = 0;
    if (!connected)
        printf("[ERROR] MST does not connect all nodes.\n");
    else
        printf("[OK] All nodes are connected in MST.\n");

    // 3) Compute MST via Prim's algorithm (for comparison)
    int inMST[NODES];
    int key[NODES];
    int parentp[NODES];
    for (int i = 0; i < NODES; ++i)
    {
        inMST[i] = 0;
        key[i] = 1e9;
        parentp[i] = -1;
    }
    key[0] = 0;
    for (int cnt = 0; cnt < NODES; ++cnt)
    {
        int u = -1;
        int best = 1e9;
        for (int v = 0; v < NODES; ++v)
            if (!inMST[v] && key[v] < best)
            {
                best = key[v];
                u = v;
            }
        if (u == -1)
            break;
        inMST[u] = 1;
        for (int v = 0; v < NODES; ++v)
        {
            if (adj[u][v] != 0 && !inMST[v] && adj[u][v] < key[v])
            {
                key[v] = adj[u][v];
                parentp[v] = u;
            }
        }
    }
    int prim_cost = 0;
    Edge prim_edges[NODES - 1];
    int prim_e = 0;
    for (int v = 1; v < NODES; ++v)
    {
        if (parentp[v] != -1)
        {
            prim_edges[prim_e++] = (Edge){parentp[v], v, adj[parentp[v]][v]};
            prim_cost += adj[parentp[v]][v];
        }
    }

    printf("\nPrim's MST edges (for verification):\n");
    for (int i = 0; i < prim_e; ++i)
        printf(" %c - %c : %d\n", names[prim_edges[i].u], names[prim_edges[i].v], prim_edges[i].w);
    printf("Prim total cost: %d\n", prim_cost);

    if (prim_cost == total_cost)
        printf("\n[OK] Kruskal and Prim agree: MST cost = %d\n", total_cost);
    else
        printf("\n[WARNING] Different totals: Kruskal=%d Prim=%d\n", total_cost, prim_cost);

    return 0;
}
