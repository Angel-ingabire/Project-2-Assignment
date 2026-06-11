#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Nodes: A B C D E F G => indices 0..6
#define NODES 7

int main(void)
{
    const char *names = "ABCDEFG";

    // Adjacency matrix (weights = data transfer time in minutes). 0 = no direct session
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

    printf("Enter suspected compromised workstation (A-G): ");
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin))
        return 0;
    // find first alpha
    char c = 0;
    for (size_t i = 0; i < strlen(buf); ++i)
        if (isalpha((unsigned char)buf[i]))
        {
            c = toupper((unsigned char)buf[i]);
            break;
        }
    if (c < 'A' || c > 'G')
    {
        printf("Invalid node. Please enter a letter A-G.\n");
        return 1;
    }
    int src = c - 'A';

    // BFS to compute distances
    int dist[NODES];
    for (int i = 0; i < NODES; ++i)
        dist[i] = -1;
    int queue[NODES];
    int qhead = 0, qtail = 0;
    dist[src] = 0;
    queue[qtail++] = src;
    while (qhead < qtail)
    {
        int u = queue[qhead++];
        for (int v = 0; v < NODES; ++v)
        {
            if (adj[u][v] != 0 && dist[v] == -1)
            {
                dist[v] = dist[u] + 1;
                queue[qtail++] = v;
            }
        }
    }

    // Collect direct neighbors (one-hop)
    int found = 0;
    int maxNeighbor = -1;
    int maxWeight = -1;
    printf("\nDirectly connected workstations to %c:\n", names[src]);
    for (int v = 0; v < NODES; ++v)
    {
        if (v == src)
            continue;
        if (dist[v] == 1)
        {
            printf(" - %c (data transfer time: %d minutes)\n", names[v], adj[src][v]);
            found = 1;
            if (adj[src][v] > maxWeight)
            {
                maxWeight = adj[src][v];
                maxNeighbor = v;
            }
        }
    }
    if (!found)
    {
        printf(" None (no direct sessions found).\n");
        return 0;
    }

    printf("\nMost at-risk direct contact: %c (data transfer time = %d minutes)\n", names[maxNeighbor], maxWeight);

    return 0;
}
